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
 * @ingroup SipStack
 * @brief SIP stack 에 callback 인터페이스를 추가한다.
 * @param pclsCallBack SIP stack 의 callback 인터페이스
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipStack::AddCallBack( ISipStackCallBack * pclsCallBack )
{
	if( pclsCallBack == NULL ) return false;

	SIP_STACK_CALLBACK_LIST::iterator	it;
	bool	bFound = false;

	for( it = m_clsCallBackList.begin(); it != m_clsCallBackList.end(); ++it )
	{
		if( *it == pclsCallBack )
		{
			bFound = true;
			break;
		}
	}

	if( bFound == false )
	{
		m_clsCallBackList.push_back( pclsCallBack );
	}

	return true;
}

/**
 * @ingroup SipStack
 * @brief SIP stack 에 callback 인터페이스를 삭제한다.
 * @param pclsCallBack SIP stack 의 callback 인터페이스
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipStack::DeleteCallBack( ISipStackCallBack * pclsCallBack )
{
	SIP_STACK_CALLBACK_LIST::iterator	it;
	bool	bFound = false;

	for( it = m_clsCallBackList.begin(); it != m_clsCallBackList.end(); ++it )
	{
		if( *it == pclsCallBack )
		{
			m_clsCallBackList.erase( it );
			bFound = true;
			break;
		}
	}

	return bFound;
}

/**
 * @ingroup SipStack
 * @brief SIP stack 의 보안 기능을 수행할 callback 인터페이스를 등록한다.
 * @param pclsSecurityCallBack 
 */
void CSipStack::SetSecurityCallBack( ISipStackSecurityCallBack * pclsSecurityCallBack )
{
	m_pclsSecurityCallBack = pclsSecurityCallBack;
}

/**
 * @ingroup SipStack
 * @brief 수신된 요청 SIP 메시지에 대한 callback 메소드를 호출한다.
 *				만약 요청 SIP 메시지를 처리할 callback 이 존재하지 않으면 501 응답 메시지를 전송한다.
 * @param iThreadId		쓰레드 아이디 ( 0 부터 쓰레드 개수 )
 * @param pclsMessage SIP 메시지 저장 구조체
 */
void CSipStack::RecvRequest( int iThreadId, CSipMessage * pclsMessage )
{
	SIP_STACK_CALLBACK_LIST::iterator itList;
	bool	bSendResponse = false;

	for( itList = m_clsCallBackList.begin(); itList != m_clsCallBackList.end(); ++itList )
	{
		if( (*itList)->RecvRequest( iThreadId, pclsMessage ) )
		{
			bSendResponse = true;
			break;
		}
	}

	if( bSendResponse == false )
	{
		CSipMessage * psttResponse = pclsMessage->CreateResponseWithToTag( SIP_NOT_IMPLEMENTED );
		if( psttResponse )
		{
			SendSipMessage( psttResponse );
		}
	}
}

/**
 * @ingroup SipStack
 * @brief 수신된 응답 SIP 메시지에 대한 callback 메소드를 호출한다.
 * @param iThreadId		쓰레드 아이디 ( 0 부터 쓰레드 개수 )
 * @param pclsMessage SIP 메시지 저장 구조체
 */
void CSipStack::RecvResponse( int iThreadId, CSipMessage * pclsMessage )
{
	SIP_STACK_CALLBACK_LIST::iterator itList;

	for( itList = m_clsCallBackList.begin(); itList != m_clsCallBackList.end(); ++itList )
	{
		if( (*itList)->RecvResponse( iThreadId, pclsMessage ) ) break;
	}
}

/**
 * @ingroup SipStack
 * @brief 전송 SIP 메시지에 대한 timeout callback 메소드를 호출한다.
 * @param iThreadId		쓰레드 아이디 ( 0 부터 쓰레드 개수 )
 * @param pclsMessage SIP 메시지 저장 구조체
 */
void CSipStack::SendTimeout( int iThreadId, CSipMessage * pclsMessage )
{
	SIP_STACK_CALLBACK_LIST::iterator itList;

	for( itList = m_clsCallBackList.begin(); itList != m_clsCallBackList.end(); ++itList )
	{
		if( (*itList)->SendTimeout( iThreadId, pclsMessage ) ) break;
	}
}

/**
 * @ingroup SipStack
 * @brief TCP/TLS 세션 종료에 대한 callback 메소드를 호출한다.
 * @param pszIp IP 주소
 * @param iPort 포트 번호
 * @param eTransport 프로토콜
 */
void CSipStack::TcpSessionEnd( const char * pszIp, int iPort, ESipTransport eTransport )
{
	SIP_STACK_CALLBACK_LIST::iterator itList;

	for( itList = m_clsCallBackList.begin(); itList != m_clsCallBackList.end(); ++itList )
	{
		(*itList)->TcpSessionEnd( pszIp, iPort, eTransport );
	}
}

/**
 * @ingroup SipStack
 * @brief 쓰레드 종료 이벤트를 전달한다.
 * @param iThreadId 쓰레드 아이디 ( 0 부터 쓰레드 개수 )
 */
void CSipStack::CallBackThreadEnd( int iThreadId )
{
	SIP_STACK_CALLBACK_LIST::iterator itList;

	for( itList = m_clsCallBackList.begin(); itList != m_clsCallBackList.end(); ++itList )
	{
		(*itList)->CallBackThreadEnd( iThreadId );
	}
}
