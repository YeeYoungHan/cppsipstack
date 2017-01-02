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
#include "SipTcpMessage.h"
#include "SipTlsMessage.h"
#include "SipQueue.h"
#include "TimeUtility.h"
#include "Log.h"
#include "MemoryDebug.h"

#include "SipStackCallBack.hpp"
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
	m_clsICT.SetTimerD( m_clsSetup.m_iTimerD );
	m_clsNIST.SetTimerJ( m_clsSetup.m_iTimerJ );
	m_clsTcpConnectMap.SetStateful( m_clsSetup.m_bStateful );
	m_clsTlsConnectMap.SetStateful( m_clsSetup.m_bStateful );

	InitNetwork();

	if( m_clsSetup.m_iLocalUdpPort > 0 )
	{
		m_hUdpSocket = UdpListen( m_clsSetup.m_iLocalUdpPort, NULL, m_clsSetup.m_bIpv6 );
		if( m_hUdpSocket == INVALID_SOCKET ) return false;
	}

	if( m_clsSetup.m_iLocalTcpPort > 0 )
	{
		m_hTcpSocket = TcpListen( m_clsSetup.m_iLocalTcpPort, 255, NULL, m_clsSetup.m_bIpv6 );
		if( m_hTcpSocket == INVALID_SOCKET ) 
		{
			CLog::Print( LOG_ERROR, "TcpListen(%d) error", m_clsSetup.m_iLocalTcpPort );
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
		if( SSLServerStart( m_clsSetup.m_strCertFile.c_str(), m_clsSetup.m_strCaCertFile.c_str() ) == false )
		{
			_Stop();
			return false;
		}

		m_hTlsSocket = TcpListen( m_clsSetup.m_iLocalTlsPort, 255, NULL, m_clsSetup.m_bIpv6 );
		if( m_hTlsSocket == INVALID_SOCKET ) 
		{
				CLog::Print( LOG_ERROR, "TcpListen(%d) error", m_clsSetup.m_iLocalTlsPort );
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

	if( m_clsSetup.m_iLocalUdpPort > 0 )
	{
		if( StartSipUdpThread( this ) == false )
		{
			_Stop();
			return false;
		}
	}

	if( m_clsSetup.m_bStateful )
	{
		if( StartSipStackThread( this ) == false )
		{
			_Stop();
			return false;
		}
	}

	if( m_clsSetup.m_iTcpCallBackThreadCount > 0 )
	{
		for( int i = 0; i < m_clsSetup.m_iTcpCallBackThreadCount; ++i )
		{
			if( StartSipQueueThread( this ) == false )
			{
				_Stop();
				return false;
			}
		}
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
void CSipStack::GetString( CMonitorString & strBuf )
{
	strBuf.Clear();

	strBuf.AddCol( m_clsICT.GetSize() );
	strBuf.AddCol( m_clsNICT.GetSize() );
	strBuf.AddCol( m_clsIST.GetSize() );
	strBuf.AddCol( m_clsNIST.GetSize() );
	strBuf.AddRow( gclsSipDeleteQueue.GetSize() );
}

/**
 * @ingroup SipStack
 * @brief Invite Client Transaction 정보를 문자열에 저장한다.
 * @param strBuf 문자열 변수
 */
void CSipStack::GetICTString( CMonitorString & strBuf )
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
 * @brief 모든 SIP transaction 을 삭제한다.
 */
void CSipStack::DeleteAllTransaction()
{
	m_clsICT.DeleteAll();
	m_clsNICT.DeleteAll();
	m_clsIST.DeleteAll();
	m_clsNIST.DeleteAll();

	gclsSipDeleteQueue.DeleteAll();
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

	if( m_clsSetup.m_iLocalUdpPort > 0 )
	{
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
	}

	gclsSipQueue.BroadCast();

	// 모든 쓰레드가 종료할 때까지 대기한다.
	while( m_iUdpThreadRunCount > 0 || m_iTcpThreadRunCount > 0 || m_bStackThreadRun || GetTcpConnectingCount() > 0 )
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
	m_clsTcpSocketMap.DeleteAll();

#ifdef USE_TLS
	if( m_hTlsSocket != INVALID_SOCKET )
	{
		closesocket( m_hTlsSocket );
		m_hTlsSocket = INVALID_SOCKET;
	}

	m_clsTlsThreadList.Final();
	m_clsTlsSocketMap.DeleteAll();
	SSLServerStop();
#endif

	DeleteAllTransaction();

	m_bStopEvent = false;

	return true;
}

/**
 * @ingroup SipStack
 * @brief TCP/TLS 연결 진행중인 쓰레드 개수를 리턴한다.
 * @returns TCP/TLS 연결 진행중인 쓰레드 개수를 리턴한다.
 */
int CSipStack::GetTcpConnectingCount( )
{
	int iCount = m_clsTcpConnectMap.GetSize();

#ifdef USE_TLS
	iCount += m_clsTlsConnectMap.GetSize();
#endif

	return iCount;
}
