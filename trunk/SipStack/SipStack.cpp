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
 * @brief ������ - ���� ������ �ʱ�ȭ��Ű�� transaction list �� SIP stack �� �����Ų��.
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
 * @brief �Ҹ���
 */
CSipStack::~CSipStack()
{
}

/**
 * @ingroup SipStack
 * @brief SIP stack �� �����Ѵ�. SIP stack ������� network ���� �����带 �����Ѵ�.
 * @param clsSetup SIP stack ���� �׸� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief SIP stack �� ������Ų��.
 * @returns �����ϸ� true �� �����ϰ� SIP stack �� ������� �ʾҰų� ���� �̺�Ʈ ó�� ���̸� false �� �����Ѵ�.
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
 * @brief SIP stack �� callback �������̽��� �߰��Ѵ�.
 * @param pclsCallBack SIP stack �� callback �������̽�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief SIP stack �� callback �������̽��� �����Ѵ�.
 * @param pclsCallBack SIP stack �� callback �������̽�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief SIP stack �� ���� ����� ������ callback �������̽��� ����Ѵ�.
 * @param pclsSecurityCallBack 
 */
void CSipStack::SetSecurityCallBack( ISipStackSecurityCallBack * pclsSecurityCallBack )
{
	m_pclsSecurityCallBack = pclsSecurityCallBack;
}

/**
 * @ingroup SipStack
 * @brief SIP stack �� �����Ѵ�.
 *				SIP stack �� �����ϴ� Transaction List �� �ֱ������� �����Ͽ��� Re-Transmit �Ǵ� Timeout ���� ó���ϱ� ���ؼ� �� �Լ��� 20ms �������� ȣ���� �־�� �Ѵ�.
 * @param psttTime ���� �ð�
 * @returns true �� �����Ѵ�.
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
 * @brief ���ŵ� ��û SIP �޽����� ���� callback �޼ҵ带 ȣ���Ѵ�.
 *				���� ��û SIP �޽����� ó���� callback �� �������� ������ 501 ���� �޽����� �����Ѵ�.
 * @param iThreadId		������ ���̵� ( 0 ���� ������ ���� )
 * @param pclsMessage SIP �޽��� ���� ����ü
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
 * @brief ���ŵ� ���� SIP �޽����� ���� callback �޼ҵ带 ȣ���Ѵ�.
 * @param iThreadId		������ ���̵� ( 0 ���� ������ ���� )
 * @param pclsMessage SIP �޽��� ���� ����ü
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
 * @brief ���� SIP �޽����� ���� timeout callback �޼ҵ带 ȣ���Ѵ�.
 * @param iThreadId		������ ���̵� ( 0 ���� ������ ���� )
 * @param pclsMessage SIP �޽��� ���� ����ü
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
 * @brief ������ ���� �̺�Ʈ�� �����Ѵ�.
 * @param iThreadId ������ ���̵� ( 0 ���� ������ ���� )
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
 * @brief UDP SIP �޽��� ���� ������ ������ ������Ų��.
 * @param iThreadId UDP SIP �޽��� ���� ������ ������ ������Ű�� ���� UDP SIP �޽��� ���� ������ ������ ������ ����
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
 * @brief UDP SIP �޽��� ���� ������ ������ ���ҽ�Ų��.
 */
void CSipStack::DecreateUdpThreadCount()
{
	m_clsMutex.acquire();
	--m_iUdpThreadRunCount;
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief TCP SIP �޽��� ���� ������ ������ ������Ų��.
 * @param iThreadId UDP SIP �޽��� ���� ������ ������ ������Ű�� ���� UDP SIP �޽��� ���� ������ ������ ������ ����
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
 * @brief TCP SIP �޽��� ���� ������ ������ ���ҽ�Ų��.
 */
void CSipStack::DecreateTcpThreadCount()
{
	m_clsMutex.acquire();
	--m_iTcpThreadRunCount;
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief Transaction List �� ������ ���ڿ��� �����Ѵ�.
 * @param strBuf		���ڿ� ����
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
 * @brief Invite Client Transaction ������ ���ڿ��� �����Ѵ�.
 * @param strBuf ���ڿ� ����
 */
void CSipStack::GetICTString( std::string & strBuf )
{
	m_clsICT.GetString( strBuf );
}

/**
 * @ingroup SipStack
 * @brief ���μ����� ����� ���� ���������� �����Ͽ��� openssl �޸� ������ ������� �ʴ´�. 
 */
void CSipStack::Final()
{
#ifdef USE_TLS
	SSLFinal();
#endif
}

/**
 * @ingroup SipStack
 * @brief UDP SIP �޽��� ���� �����忡 ���� SIP �޽����� �����ϰ� SIP stack �����忡 ���� �̺�Ʈ�� ������ ��, ��� �����尡 ������ ������ ����� ��,
 *				���� �ڵ��� �����Ų��.
 * @returns true �� �����Ѵ�.
 */
bool CSipStack::_Stop( )
{
	m_bStopEvent = true;

	// SIP �޽��� ���� �����尡 N �� ����ǹǷ� N �� ����ϴ� ���� �����ϱ� ���� �ڵ��̴�.
	Socket hSocket = UdpSocket();

	if( hSocket != INVALID_SOCKET )
	{
		for( int i = 0; i < m_clsSetup.m_iUdpThreadCount; ++i )
		{
			UdpSend( hSocket, "\r\n", 2, "127.0.0.1", m_clsSetup.m_iLocalUdpPort );
		}

		closesocket( hSocket );
	}

	// ��� �����尡 ������ ������ ����Ѵ�.
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

