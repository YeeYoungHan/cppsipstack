/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#include "SipPlatformDefine.h"
#include "SipCallDump.h"
#include "SipCallDumpSetup.h"
#include "ServerService.h"
#include "ServerUtility.h"
#include "Log.h"
#include "pcap.h"
#include "PacketHeader.h"
#include "SipCallMap.h"
#include "RtpMap.h"
#include "IpFragmentMap.h"
#include "MemoryDebug.h"

/**
 * @ingroup SipCallDump
 * @brief 패킷 덤프 쓰레드
 * @param lpParameter 
 * @returns 0 을 리턴한다.
 */
THREAD_API PacketDumpThread( LPVOID lpParameter )
{
	pcap_t	* psttPcap = NULL;
	char		szErrBuf[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr * psttHeader;
	const		u_char * pszData;
	int			n, iIpHeaderLen, iUdpBodyLen, iIpPos;
	char		* pszUdpBody;

	Ip4Header		* psttIp4Header;	// IPv4 Header
	UdpHeader		* psttUdpHeader;	// UDP Header

	bool bEnd;
	bool bPcapFile = false;
	
	CLog::Print( LOG_INFO, "%s is started", __FUNCTION__ );

	if( gstrPcapFileName.empty() == false )
	{
		CLog::Print( LOG_DEBUG, "pcap_open_offline(%s)", gstrPcapFileName.c_str() );
		psttPcap = pcap_open_offline( gstrPcapFileName.c_str(), szErrBuf );
		bPcapFile = true;
	}
	else
	{
		psttPcap = pcap_open_live( gclsSetup.m_strPacketDevice.c_str(), gclsSetup.m_iPacketSnapLen, 1, gclsSetup.m_iPacketReadTimeout, szErrBuf );
	}

	if( psttPcap == NULL )
	{
		CLog::Print( LOG_ERROR, "pcap_open_live(%s) error(%d)", gclsSetup.m_strPacketDevice.c_str(), errno );
		goto FUNC_END;
	}

	while( gbStop == false )
	{
		n = pcap_next_ex( psttPcap, &psttHeader, &pszData );
		if( n > 0 )
		{
			if( psttHeader->caplen < MIN_PACKET_SIZE ) continue;

			if( pszData[12] == 0x81 )
			{
				iIpPos = 18;		// VLAN
			}
			else if( pszData[12] == 0x08 )
			{
				iIpPos = 14;		// IP
			}
			else
			{
				continue;
			}

			psttIp4Header = ( Ip4Header * )( pszData + iIpPos );

			// IPv4 만 검사한다.
			if( ( psttIp4Header->ver_ihl & 0xF0 ) != 0x40 ) continue;

			iIpHeaderLen = GetIpHeaderLength( psttIp4Header );
			if( iIpHeaderLen == 0 ) continue;

			// UDP 만 검사한다.
			if( IsUdpPacket( psttIp4Header ) == false ) continue;

			psttUdpHeader = (UdpHeader *)( pszData + iIpPos + iIpHeaderLen );
			pszUdpBody = (char *)( pszData + iIpPos + iIpHeaderLen + 8 );
			iUdpBodyLen = psttHeader->caplen - ( iIpPos + iIpHeaderLen + 8 );
			if( iUdpBodyLen <= MIN_UDP_BODY_SIZE ) continue;

			if( IsRtpPacket( pszUdpBody, iUdpBodyLen ) )
			{
				gclsRtpMap.Insert( psttHeader, pszData, psttIp4Header, psttUdpHeader );
			}
			else
			{
				// fragment 처리
				if( gclsIpFragmentMap.Insert( psttIp4Header, (char *)pszData + iIpPos + iIpHeaderLen, psttHeader->caplen - ( iIpPos + iIpHeaderLen ), bEnd ) )
				{
					if( bEnd == false ) continue;

					CIpPacket clsPacket;

					if( gclsIpFragmentMap.Delete( psttIp4Header, &clsPacket ) == false ) 
					{
						CLog::Print( LOG_ERROR, "%s gclsIpFragmentMap.Delete() error", __FUNCTION__ );
						continue;
					}

					pszUdpBody = (char *)( clsPacket.m_pszPacket + 20 + 8 );
					iUdpBodyLen = clsPacket.m_iPacketLen - ( 20 + 8 );

					if( IsSipPacket( pszUdpBody, iUdpBodyLen ) )
					{
						u_char * pszNewData = (u_char *)malloc( iIpPos + clsPacket.m_iPacketLen );
						if( pszNewData == NULL )
						{
							CLog::Print( LOG_ERROR, "%s new data malloc error", __FUNCTION__ );
						}
						else
						{
							memcpy( pszNewData, pszData, iIpPos );
							memcpy( pszNewData + iIpPos, clsPacket.m_pszPacket, clsPacket.m_iPacketLen );
							psttHeader->caplen = iIpPos + clsPacket.m_iPacketLen;
							psttHeader->len = psttHeader->caplen;
							gclsCallMap.Insert( psttPcap, psttHeader, pszNewData, pszUdpBody, iUdpBodyLen );
							free( pszNewData );
						}
					}
				}
				else if( IsSipPacket( pszUdpBody, iUdpBodyLen ) )
				{
					gclsCallMap.Insert( psttPcap, psttHeader, pszData, pszUdpBody, iUdpBodyLen );
				}
			}
		}
		else if( bPcapFile )
		{
			gbStop = true;
			break;
		}
	}

FUNC_END:
	if( psttPcap ) pcap_close( psttPcap );

	CLog::Print( LOG_INFO, "%s is stoped", __FUNCTION__ );

	return 0;
}

/**
 * @ingroup SipCallDump
 * @brief 패킷 덤프 쓰레드를 시작한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool StartPacketDumpThread( )
{
	return StartThread( "PacketDumpThread", PacketDumpThread, NULL );
}
