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

/**
 * @ingroup SipUserAgent
 * @brief SIP 요청 메시지 수신 callback method
 * @param iThreadId		SIP stack 의 UDP 쓰레드 아이디
 * @param pclsMessage 수신된 SIP 요청 메시지
 * @returns SIP 요청 메시지를 처리한 경우 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipUserAgent::RecvRequest( int iThreadId, CSipMessage * pclsMessage )
{
	if( pclsMessage->IsMethod( SIP_METHOD_INVITE ) )
	{
		return RecvInviteRequest( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( SIP_METHOD_BYE ) )
	{
		return RecvByeRequest( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( SIP_METHOD_CANCEL ) )
	{
		return RecvCancelRequest( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( "PRACK" ) )
	{
		return RecvPrackRequest( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( "REFER" ) )
	{
		return RecvReferRequest( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( "NOTIFY" ) )
	{
		return RecvNotifyRequest( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( "MESSAGE" ) )
	{
		return RecvMessageRequest( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( "OPTIONS" ) )
	{
		return RecvOptionsRequest( iThreadId, pclsMessage );
	}

	return false;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP 응답 메시지 수신 callback method
 * @param iThreadId		SIP stack 의 UDP 쓰레드 아이디
 * @param pclsMessage 수신된 SIP 응답 메시지
 * @returns SIP 응답 메시지를 처리한 경우 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipUserAgent::RecvResponse( int iThreadId, CSipMessage * pclsMessage )
{
	if( pclsMessage->IsMethod( SIP_METHOD_REGISTER ) )
	{
		return RecvRegisterResponse( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( SIP_METHOD_INVITE ) )
	{
		return RecvInviteResponse( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( SIP_METHOD_BYE ) )
	{
		return true;
	}
	else if( pclsMessage->IsMethod( SIP_METHOD_CANCEL ) )
	{
		return true;
	}
	else if( pclsMessage->IsMethod( "PRACK" ) )
	{
		return true;
	}
	else if( pclsMessage->IsMethod( "REFER" ) )
	{
		return RecvReferResponse( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( "NOTIFY" ) )
	{
		return true;
	}
	else if( pclsMessage->IsMethod( "MESSAGE" ) )
	{
		return true;
	}
	else if( pclsMessage->IsMethod( "OPTIONS" ) )
	{
		return true;
	}

	return false;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP 메시지 전송 timeout callback method
 * @param iThreadId		SIP stack 의 UDP 쓰레드 아이디
 * @param pclsMessage 수신된 SIP 응답 메시지
 * @returns SIP 응답 메시지를 처리한 경우 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipUserAgent::SendTimeout( int iThreadId, CSipMessage * pclsMessage )
{
	if( pclsMessage->IsMethod( SIP_METHOD_ACK ) )
	{
		std::string	strCallId;

		pclsMessage->GetCallId( strCallId );

		if( m_pclsCallBack ) m_pclsCallBack->EventCallEnd( strCallId.c_str(), SIP_GONE );

		Delete( strCallId.c_str() );
	}

	return false;
}

/**
 * @ingroup SipUserAgent
 * @brief TCP/TLS 세션 종료 이벤트 핸들러
 * @param pszIp IP 주소
 * @param iPort 포트 번호
 * @param eTransport 프로토콜
 */
void CSipUserAgent::TcpSessionEnd( const char * pszIp, int iPort, ESipTransport eTransport )
{
	SIP_SERVER_INFO_LIST::iterator	it;

	m_clsRegisterMutex.acquire();
	for( it = m_clsRegisterList.begin(); it != m_clsRegisterList.end(); ++it )
	{
		if( it->Equal( pszIp, iPort, eTransport ) )
		{
			it->ClearLogin();
		}
	}
	m_clsRegisterMutex.release();
}

/**
 * @ingroup SipUserAgent
 * @brief SIP 쓰레드 종료 이벤트 핸들러
 * @param iThreadId 쓰레드 번호
 */
void CSipUserAgent::ThreadEnd( int iThreadId )
{
	if( m_pclsCallBack ) m_pclsCallBack->EventThreadEnd( iThreadId );
}
