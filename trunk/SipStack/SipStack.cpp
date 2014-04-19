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
#include "SipDeleteQueue.h"
#include "Log.h"
#include "MemoryDebug.h"

#include "SipStackComm.hpp"

/**
 * @ingroup SipStack
 * @brief 생성자 - 내부 변수를 초기화시키고 transaction list 와 SIP stack 을 연결시킨다.
 */
CSipStack::CSipStack()
{
	m_bStopEvent = false;
	m_bStackThreadRun = false;
	m_hUdpSocket = INVALID_SOCKET;
	m_hTcpSocket = INVALID_SOCKET;

#ifdef USE_TLS
	m_hTlsSocket = INVALID_SOCKET;
#endif

	m_bStarted = false;
	m_iUdpThreadRunCount = 0;
	m_iTcpThreadRunCount = 0;

	m_clsICT.SetSipStack( this );
	m_clsNICT.SetSipStack( this );
	m_clsIST.SetSipStack( this );
	m_clsNIST.SetSipStack( this );

	m_pclsSecurityCallBack = NULL;
}

/**
 * @ingroup SipStack
 * @brief 소멸자
 */
CSipStack::~CSipStack()
{
}

/**
 * @ingroup SipStack
 * @brief SIP stack 을 시작한다. SIP stack 쓰레드와 network 수신 쓰레드를 시작한다.
 * @param clsSetup SIP stack 설정 항목 저장 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipStack::Start( CSipStackSetup & clsSetup )
{
	if( m_bStarted || m_bStopEvent ) return false;
	if( clsSetup.Check() == false ) return false;

	m_clsSetup = clsSetup;

	InitNetwork();

	m_hUdpSocket = UdpListen( m_clsSetup.m_iLocalUdpPort, NULL );
	if( m_hUdpSocket == INVALID_SOCKET ) return false;

	if( m_clsSetup.m_iLocalTcpPort > 0 )
	{
		m_hTcpSocket = TcpListen( m_clsSetup.m_iLocalTcpPort, 255, NULL );
		if( m_hTcpSocket == INVALID_SOCKET ) 
		{
			_Stop();
			return false;
		}

		m_clsTcpThreadList.SetMaxSocketPerThread( m_clsSetup.m_iTcpMaxSocketPerThread );
		if( m_clsTcpThreadList.Init( m_clsSetup.m_iTcpThreadCount, m_clsSetup.m_iTcpThreadCount, SipTcpThread, this ) == false )
		{
			_Stop();
			return false;
		}

		if( StartSipTcpListenThread( this ) == false )
		{
			_Stop();
			return false;
		}
	}

#ifdef USE_TLS
	if( m_clsSetup.m_iLocalTlsPort > 0 )
	{
		if( SSLServerStart( m_clsSetup.m_strCertFile.c_str(), "" ) == false )
		{
			_Stop();
			return false;
		}

		m_hTlsSocket = TcpListen( m_clsSetup.m_iLocalTlsPort, 255, NULL );
		if( m_hTlsSocket == INVALID_SOCKET ) 
		{
			_Stop();
			return false;
		}

		m_clsTlsThreadList.SetMaxSocketPerThread( m_clsSetup.m_iTcpMaxSocketPerThread );
		if( m_clsTlsThreadList.Init( m_clsSetup.m_iTcpThreadCount, m_clsSetup.m_iTcpThreadCount, SipTlsThread, this ) == false )
		{
			_Stop();
			return false;
		}

		if( StartSipTlsListenThread( this ) == false )
		{
			_Stop();
			return false;
		}
	}
#endif

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
 * @ingroup SipStack
 * @brief SIP stack 을 중지시킨다.
 * @returns 성공하면 true 를 리턴하고 SIP stack 이 실행되지 않았거나 종료 이벤트 처리 중이면 false 를 리턴한다.
 */
bool CSipStack::Stop( )
{
	if( m_bStarted == false || m_bStopEvent ) return false;

	_Stop();

	m_clsCallBackList.clear();

	m_bStarted = false;

	return true;
}

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

/**
 * @ingroup SipStack
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
 * @ingroup SipStack
 * @brief UDP SIP 메시지 수신 쓰레드 개수를 감소시킨다.
 */
void CSipStack::DecreateUdpThreadCount()
{
	m_clsMutex.acquire();
	--m_iUdpThreadRunCount;
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief TCP SIP 메시지 수신 쓰레드 개수를 증가시킨다.
 * @param iThreadId UDP SIP 메시지 수신 쓰레드 개수를 증가시키기 전에 UDP SIP 메시지 수신 쓰레드 개수를 저장할 변수
 */
void CSipStack::IncreateTcpThreadCount( int & iThreadId )
{
	m_clsMutex.acquire();
	iThreadId = m_iTcpThreadRunCount;
	++m_iTcpThreadRunCount;
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief TCP SIP 메시지 수신 쓰레드 개수를 감소시킨다.
 */
void CSipStack::DecreateTcpThreadCount()
{
	m_clsMutex.acquire();
	--m_iTcpThreadRunCount;
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief Transaction List 의 정보를 문자열에 저장한다.
 * @param strBuf		문자열 변수
 */
void CSipStack::GetString( std::string & strBuf )
{
	char	szBuf[255];

	snprintf( szBuf, sizeof(szBuf), "%d%s%d%s%d%s%d%s%d%s", m_clsICT.GetSize(), MR_COL_SEP, m_clsNICT.GetSize(), MR_COL_SEP
		, m_clsIST.GetSize(), MR_COL_SEP, m_clsNIST.GetSize(), MR_COL_SEP, gclsSipDeleteQueue.GetSize(), MR_ROW_SEP );
	strBuf = szBuf;
}

/**
 * @ingroup SipStack
 * @brief Invite Client Transaction 정보를 문자열에 저장한다.
 * @param strBuf 문자열 변수
 */
void CSipStack::GetICTString( std::string & strBuf )
{
	m_clsICT.GetString( strBuf );
}

/**
 * @ingroup SipStack
 * @brief 프로세스가 종료될 때에 최종적으로 실행하여서 openssl 메모리 누수를 출력하지 않는다. 
 */
void CSipStack::Final()
{
#ifdef USE_TLS
	SSLFinal();
#endif
}

/**
 * @ingroup SipStack
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
	while( m_iUdpThreadRunCount > 0 || m_iTcpThreadRunCount > 0 || m_bStackThreadRun )
	{
		MiliSleep( 20 );
	}

	if( m_hUdpSocket != INVALID_SOCKET )
	{
		closesocket( m_hUdpSocket );
		m_hUdpSocket = INVALID_SOCKET;
	}

	if( m_hTcpSocket != INVALID_SOCKET )
	{
		closesocket( m_hTcpSocket );
		m_hTcpSocket = INVALID_SOCKET;
	}

	m_clsTcpThreadList.Final();

#ifdef USE_TLS
	if( m_hTlsSocket != INVALID_SOCKET )
	{
		closesocket( m_hTlsSocket );
		m_hTlsSocket = INVALID_SOCKET;
	}

	m_clsTlsThreadList.Final();
#endif

	m_clsICT.DeleteAll();
	m_clsNICT.DeleteAll();
	m_clsIST.DeleteAll();
	m_clsNIST.DeleteAll();

	gclsSipDeleteQueue.DeleteAll();

	m_bStopEvent = false;

	return true;
}

