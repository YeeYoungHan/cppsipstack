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

#include "SipTcpMessage.h"
#include "Log.h"
#include "MemoryDebug.h"

/**
 * @brief SIP 메시지를 TCP 세션에 적합하게 수정한 후, TCP 세션으로 전송한다.
 * @param hSocket TCP 소켓
 * @param pszIp		목적지 IP 주소
 * @param iPort		목적지 포트 번호
 * @param pclsMessage SIP 메시지
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool SipTcpSend( Socket hSocket, const char * pszIp, int iPort, CSipMessage * pclsMessage )
{
	std::string	strTcpIp;
	int iTcpPort;
	bool bRes = false;

	if( GetLocalIpPort( hSocket, strTcpIp, iTcpPort ) == false )
	{
		CLog::Print( LOG_ERROR, "GetLocalIpPort(%d) error(%d)", hSocket, GetError() );
		return false;	
	}
	else
	{
		bool bVia = false, bContact = false;
		
		if( pclsMessage->IsRequest() )
		{
			bVia = pclsMessage->SetTopViaIpPort( strTcpIp.c_str(), iTcpPort, E_SIP_TCP );
		}

		bContact = pclsMessage->SetTopContactIpPort( strTcpIp.c_str(), iTcpPort, E_SIP_TCP );

		if( bVia || bContact )
		{
			pclsMessage->MakePacket();
		}
	}

	int iPacketLen = (int)pclsMessage->m_strPacket.length();

	if( TcpSend( hSocket, pclsMessage->m_strPacket.c_str(), iPacketLen ) == iPacketLen )
	{
		CLog::Print( LOG_NETWORK, "TcpSend(%s:%d) [%s]", pszIp, iPort, pclsMessage->m_strPacket.c_str() );
		bRes = true;
	}
	else
	{
		CLog::Print( LOG_NETWORK, "TcpSend(%s:%d) [%s] error(%d)", pszIp, iPort, pclsMessage->m_strPacket.c_str(), GetError() );
	}

	return bRes;
}
