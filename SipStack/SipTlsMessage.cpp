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

#include "SipStackDefine.h"
#include "SipTlsMessage.h"
#include "Log.h"
#include "MemoryDebug.h"

#ifdef USE_TLS

/**
 * @brief SIP 메시지를 TCP 세션에 적합하게 수정한 후, TCP 세션으로 전송한다.
 * @param hSocket TCP 소켓
 * @param psttSsl SSL 세션
 * @param pszIp		목적지 IP 주소
 * @param iPort		목적지 포트 번호
 * @param pclsMessage SIP 메시지
 * @param iLocalTlsPort 로컬 TLS 포트 번호. 로컬 TLS listen port 번호로 Contact 을 사용하는 경우에 입력한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool SipTlsSend( Socket hSocket, SSL * psttSsl, const char * pszIp, int iPort, CSipMessage * pclsMessage, int iLocalTlsPort )
{
	std::string	strTcpIp;
	int iTcpPort;
	bool bRes = false;

	if( iLocalTlsPort > 0 )
	{
		if( pclsMessage->IsRequest() )
		{
			// LG IP-PBX 에 REGISTER 한 후, INVITE 를 수신하기 위해서는 listen port 를 사용해야 되어서 수정함.
			if( pclsMessage->SetTopViaTransPort( E_SIP_TLS, iLocalTlsPort ) )
			{
				pclsMessage->MakePacket();
			}
		}
	}
	else
	{
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
				bVia = pclsMessage->SetTopViaIpPort( strTcpIp.c_str(), iTcpPort, E_SIP_TLS );
			}

			bContact = pclsMessage->SetTopContactIpPort( strTcpIp.c_str(), iTcpPort, E_SIP_TLS );

			if( bVia || bContact )
			{
				pclsMessage->MakePacket();
			}
		}
	}

	int iPacketLen = (int)pclsMessage->m_strPacket.length();

	if( SSLSend( psttSsl, pclsMessage->m_strPacket.c_str(), iPacketLen ) == iPacketLen )
	{
		CLog::Print( LOG_NETWORK, "TlsSend(%s:%d) [%s]", pszIp, iPort, pclsMessage->m_strPacket.c_str() );
		bRes = true;
	}
	else
	{
		CLog::Print( LOG_NETWORK, "TlsSend(%s:%d) [%s] error(%d)", pszIp, iPort, pclsMessage->m_strPacket.c_str(), GetError() );
	}

	return bRes;
}

#endif
