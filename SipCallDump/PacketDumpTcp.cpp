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
#include "Log.h"
#include "SipCallMap.h"
#include "RtpMap.h"
#include "IpFragmentMap.h"
#include "MemoryDebug.h"

/**
 * @ingroup SipCallDump
 * @brief TCP 패킷을 분석한다.
 * @param psttPcap			pcap_t 포인터
 * @param psttHeader		pcap_pkthdr 포인터
 * @param pszData				패킷
 * @param psttIp4Header IPv4 헤더
 * @param iIpPos				IP 헤더 위치
 * @param iIpHeaderLen	IP 헤더 길이
 */
void PacketDumpTcp( pcap_t * psttPcap, struct pcap_pkthdr * psttHeader, const u_char * pszData, Ip4Header * psttIp4Header, int iIpPos, int iIpHeaderLen )
{
	TcpHeader * psttTcpHeader = (TcpHeader *)( pszData + iIpPos + iIpHeaderLen );

	if( ( psttTcpHeader->flags & TCP_SYN ) && !( psttTcpHeader->flags & TCP_ACK ) )
	{
		return;
	}
	else if( ( psttTcpHeader->flags & TCP_FIN ) || ( psttTcpHeader->flags & TCP_RST ) )
	{
		return;
	}

	int iTcpHeaderLen = GetTcpHeaderLength( psttTcpHeader );
	char * pszTcpBody = (char *)( pszData + iIpPos + iIpHeaderLen + iTcpHeaderLen );
	int iTcpBodyLen = psttHeader->caplen - ( iIpPos + iIpHeaderLen + iTcpHeaderLen );
	if( iTcpBodyLen <= 0 ) return;

	bool bEnd;

	// fragment 처리
	if( gclsIpFragmentMap.Insert( psttIp4Header, (char *)pszData + iIpPos + iIpHeaderLen, psttHeader->caplen - ( iIpPos + iIpHeaderLen ), bEnd ) )
	{
		if( bEnd == false ) return;

		CIpPacket clsPacket;

		if( gclsIpFragmentMap.Delete( psttIp4Header, &clsPacket ) == false ) 
		{
			CLog::Print( LOG_ERROR, "%s gclsIpFragmentMap.Delete() error", __FUNCTION__ );
			return;
		}

		// QQQ: TCP 패킷 큐에 저장한다.
	}
	else
	{
		// QQQ: TCP 패킷 큐에 저장한다.
	}
}
