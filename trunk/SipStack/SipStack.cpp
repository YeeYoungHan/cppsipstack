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

#include "SipStack.h"
#include "SipStackThread.h"
#include "SipStack.hpp"
#include "SipCreateMessage.h"

/**
 * @brief 생성자 - 내부 변수를 초기화시키고 transaction list 와 SIP stack 을 연결시킨다.
 */
CSipStack::CSipStack()
{
	m_bStopEvent = false;
	m_bStackThreadRun = false;
	m_iUdpSocket = INVALID_SOCKET;
	m_bStarted = false;
	m_iUdpThreadRunCount = 0;

	m_clsICT.SetSipStack( this );
	m_clsNICT.SetSipStack( this );
	m_clsIST.SetSipStack( this );
	m_clsNIST.SetSipStack( this );
}

CSipStack::~CSipStack()
{
}

/**
 * @brief SIP stack 을 시작한다. SIP stack 쓰레드와 network 수신 쓰레드를 시작한다.
 * @param clsSetup SIP stack 설정 항목 저장 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipStack::Start( CSipStackSetup & clsSetup )
{
	if( m_bStarted || m_bStopEvent ) return false;
	if( clsSetup.Check() == false ) return false;

	m_clsSetup.Copy( clsSetup );

	m_iUdpSocket = UdpListen( m_clsSetup.m_iLocalUdpPort, NULL );
	if( m_iUdpSocket == INVALID_SOCKET ) return false;

	if( StartSipUdpThread( this ) == false )
	{
		_Stop();
		return false;
	}

	if( StartSipStackThread( this ) == false )
	{
		_Stop();
		return false;
	}

	m_bStarted = true;

	return true;
}

/**
 * @brief SIP stack 을 중지시킨다.
 * @returns 성공하면 true 를 리턴하고 SIP stack 이 실행되지 않았거나 종료 이벤트 처리 중이면 false 를 리턴한다.
 */
bool CSipStack::Stop( )
{
	if( m_bStarted == false || m_bStopEvent ) return false;

	_Stop();

	m_bStarted = false;

	return true;
}

/**
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
 * @brief SIP stack 에 SIP 메시지가 존재하지 않으면 SIP stack 에 SIP 메시지를 저장하고 SIP 메시지를 네트워크로 전송한다.
 * @param pclsMessage SIP 메시지 저장 구조체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipStack::SendSipMessage( CSipMessage * pclsMessage )
{
	if( pclsMessage == NULL ) return false;

	++pclsMessage->m_iUseCount;

	if( pclsMessage->IsRequest() )
	{
		if( pclsMessage->IsMethod( "INVITE" ) || pclsMessage->IsMethod( "ACK" ) )
		{
			if( m_clsICT.Insert( pclsMessage ) )
			{
				Send( pclsMessage );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
		else
		{
			if( m_clsNICT.Insert( pclsMessage ) )
			{
				Send( pclsMessage );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
	}
	else
	{
		if( pclsMessage->IsMethod( "INVITE" ) )
		{
			if( m_clsIST.Insert( pclsMessage ) )
			{
				Send( pclsMessage );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
		else
		{
			if( m_clsNIST.Insert( pclsMessage ) )
			{
				Send( pclsMessage );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
	}

	return false;
}

/**
 * @brief 네트워크에서 수신된 SIP 메시지를 SIP stack 에 저장하고 callback 메소드를 호출하여 응용에 알려 준다.
 *				본 메소드에서 true 를 리턴하면 내부적으로 CSipMessage 메모리를 관리하고 그렇지 않으면 호출한 부분에서 CSipMessage 메모리를 삭제해 주어야 한다.
 * @param iThreadId		쓰레드 아이디 ( 0 부터 쓰레드 개수 )
 * @param pclsMessage SIP 메시지 저장 구조체
 * @returns SIP stack 에 저장하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipStack::RecvSipMessage( int iThreadId, CSipMessage * pclsMessage )
{
	++pclsMessage->m_iUseCount;

	if( pclsMessage->IsRequest() )
	{
		if( pclsMessage->IsMethod( "INVITE" ) || pclsMessage->IsMethod( "ACK" ) )
		{
			if( m_clsIST.Insert( pclsMessage ) )
			{
				RecvRequest( iThreadId, pclsMessage );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
		else
		{
			if( m_clsNIST.Insert( pclsMessage ) )
			{
				RecvRequest( iThreadId, pclsMessage );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
	}
	else
	{
		if( pclsMessage->IsMethod( "INVITE" ) )
		{
			// INVITE 메시지에 대한 CANCEL 메시지가 존재하면 이를 SIP stack 에서 삭제한다.
			if( pclsMessage->m_iStatusCode >= 200 )
			{
				m_clsNICT.DeleteCancel( pclsMessage );
			}

			if( m_clsICT.Insert( pclsMessage ) )
			{
				RecvResponse( iThreadId, pclsMessage );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
		else
		{
			if( m_clsNICT.Insert( pclsMessage ) )
			{
				RecvResponse( iThreadId, pclsMessage );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
	}

	return false;
}

/**
 * @brief SIP stack 을 실행한다.
 *				SIP stack 이 관리하는 Transaction List 를 주기적으로 점검하여서 Re-Transmit 또는 Timeout 등을 처리하기 위해서 본 함수를 20ms 간격으로 호출해 주어야 한다.
 * @param psttTime 현재 시간
 * @returns true 를 리턴한다.
 */
bool CSipStack::Execute( struct timeval * psttTime )
{
	m_clsICT.Execute( psttTime );
	m_clsIST.Execute( psttTime );
	m_clsNICT.Execute( psttTime );
	m_clsNIST.Execute( psttTime );

	return true;
}

/**
 * @brief SIP 메시지를 네트워크로 전송한다.
 * @param pclsMessage SIP 메시지 저장 구조체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipStack::Send( CSipMessage * pclsMessage )
{
	const char * pszIp = NULL;
	int iPort = -1;

	if( pclsMessage->IsRequest() )
	{
		SIP_FROM_LIST::iterator itList = pclsMessage->m_clsRouteList.begin();
		if( itList == pclsMessage->m_clsRouteList.end() ) return false;

		pszIp = itList->m_clsUri.m_strHost.c_str();
		iPort = itList->m_clsUri.m_iPort;
		if( iPort == -1 ) iPort = 5060;
	}
	else
	{
		SIP_VIA_LIST::iterator itList = pclsMessage->m_clsViaList.begin();
		if( itList == pclsMessage->m_clsViaList.end() ) return false;

		pszIp = itList->m_strHost.c_str();
		iPort = itList->m_iPort;
	}

	if( pszIp[0] == '\0' ) return false;

	if( pclsMessage->m_strPacket.empty() )
	{
		char	szPacket[SIP_MESSAGE_MAX_LEN];

		if( pclsMessage->ToString( szPacket, sizeof(szPacket) ) == -1 ) return false;
		pclsMessage->m_strPacket = szPacket;
	}

	m_clsUdpSendMutex.acquire();
	bool bRes = UdpSend( m_iUdpSocket, pclsMessage->m_strPacket.c_str(), pclsMessage->m_strPacket.length(), pszIp, iPort );
	m_clsUdpSendMutex.release();

	return bRes;
}

/**
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
		CSipMessage * psttResponse = SipCreateResponse( &m_clsSetup, pclsMessage, SIP_NOT_IMPLEMENTED );
		if( psttResponse )
		{
			SendSipMessage( psttResponse );
		}
	}
}

/**
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
 * @brief UDP SIP 메시지 수신 쓰레드 개수를 증가시킨다.
 * @param iThreadId UDP SIP 메시지 수신 쓰레드 개수를 증가시키기 전에 UDP SIP 메시지 수신 쓰레드 개수를 저장할 변수
 */
void CSipStack::IncreateUdpThreadCount( int & iThreadId )
{
	m_clsMutex.acquire();
	iThreadId = m_iUdpThreadRunCount;
	++m_iUdpThreadRunCount;
	m_clsMutex.release();
}

/**
 * @brief UDP SIP 메시지 수신 쓰레드 개수를 감소시킨다.
 */
void CSipStack::DecreateUdpThreadCount()
{
	m_clsMutex.acquire();
	--m_iUdpThreadRunCount;
	m_clsMutex.release();
}

/**
 * @brief Transaction List 의 크기를 문자열에 저장한다.
 * @param pszBuf		문자열 변수
 * @param iBufSize	문자열 변수의 크기
 */
void CSipStack::GetString( std::string & strBuf )
{
}

void CSipStack::GetICTString( std::string & strBuf )
{
	m_clsICT.GetString( strBuf );
}

/**
 * @brief UDP SIP 메시지 수신 쓰레드에 종료 SIP 메시지를 전송하고 SIP stack 쓰레드에 종료 이벤트를 설정한 후, 모든 쓰레드가 종료할 때까지 대기한 후,
 *				소켓 핸들을 종료시킨다.
 * @returns true 를 리턴한다.
 */
bool CSipStack::_Stop( )
{
	m_bStopEvent = true;

	// SIP 메시지 수신 쓰레드가 N 개 실행되므로 N 초 대기하는 것을 방지하기 위한 코드이다.
	Socket hSocket = UdpSocket();

	if( hSocket != INVALID_SOCKET )
	{
		for( int i = 0; i < m_clsSetup.m_iUdpThreadCount; ++i )
		{
			UdpSend( hSocket, "\r\n", 2, "127.0.0.1", m_clsSetup.m_iLocalUdpPort );
		}

		closesocket( hSocket );
	}

	// 모든 쓰레드가 종료할 때까지 대기한다.
	while( m_iUdpThreadRunCount > 0 || m_bStackThreadRun )
	{
		MiliSleep( 20 );
	}

	closesocket( m_iUdpSocket );
	m_iUdpSocket = INVALID_SOCKET;

	m_bStopEvent = false;

	return true;
}
