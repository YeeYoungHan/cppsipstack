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
#include "TcpMap.h"
#include "IpFragmentMap.h"
#include "SipCallDumpSetup.h"
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

	uint16_t sSrcPort = ntohs( psttTcpHeader->sport );
	uint16_t sDstPort = ntohs( psttTcpHeader->dport );

	// 설정 파일에 설정된 SIP 포트 번호가 아니면 무시한다.
	if( gclsSetup.IsTcpSipPort( sSrcPort ) == false && gclsSetup.IsTcpSipPort( sDstPort ) == false ) return;

	int iTcpHeaderLen = GetTcpHeaderLength( psttTcpHeader );
	int iTcpBodyPos = iIpPos + iIpHeaderLen + iTcpHeaderLen;
	char * pszTcpBody = (char *)( pszData + iTcpBodyPos );
	int iTcpBodyLen = psttHeader->caplen - iTcpBodyPos;

	// TCP body len 이 0 이면 무시한다.
	if( iTcpBodyLen <= 0 ) return;

	// IP fragment 된 TCP packet 은 현재 버전에서는 무시한다.

	if( IsSipPacket( pszTcpBody, iTcpBodyLen ) )
	{
		gclsTcpMap.Insert( psttHeader, pszData, psttIp4Header, psttTcpHeader, iTcpBodyPos, iTcpBodyLen );
	}
	else
	{
		gclsTcpMap.Update( psttHeader, pszData, psttIp4Header, psttTcpHeader, iTcpBodyPos, iTcpBodyLen );
	}
}
