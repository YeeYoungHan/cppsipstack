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
 * @brief ������ - ���� ������ �ʱ�ȭ��Ű�� transaction list �� SIP stack �� �����Ų��.
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
 * @brief SIP stack �� �����Ѵ�. SIP stack ������� network ���� �����带 �����Ѵ�.
 * @param clsSetup SIP stack ���� �׸� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief SIP stack �� ������Ų��.
 * @returns �����ϸ� true �� �����ϰ� SIP stack �� ������� �ʾҰų� ���� �̺�Ʈ ó�� ���̸� false �� �����Ѵ�.
 */
bool CSipStack::Stop( )
{
	if( m_bStarted == false || m_bStopEvent ) return false;

	_Stop();

	m_bStarted = false;

	return true;
}

/**
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
 * @brief SIP stack �� SIP �޽����� �������� ������ SIP stack �� SIP �޽����� �����ϰ� SIP �޽����� ��Ʈ��ũ�� �����Ѵ�.
 * @param pclsMessage SIP �޽��� ���� ����ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief ��Ʈ��ũ���� ���ŵ� SIP �޽����� SIP stack �� �����ϰ� callback �޼ҵ带 ȣ���Ͽ� ���뿡 �˷� �ش�.
 *				�� �޼ҵ忡�� true �� �����ϸ� ���������� CSipMessage �޸𸮸� �����ϰ� �׷��� ������ ȣ���� �κп��� CSipMessage �޸𸮸� ������ �־�� �Ѵ�.
 * @param iThreadId		������ ���̵� ( 0 ���� ������ ���� )
 * @param pclsMessage SIP �޽��� ���� ����ü
 * @returns SIP stack �� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
			// INVITE �޽����� ���� CANCEL �޽����� �����ϸ� �̸� SIP stack ���� �����Ѵ�.
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
 * @brief SIP �޽����� ��Ʈ��ũ�� �����Ѵ�.
 * @param pclsMessage SIP �޽��� ���� ����ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
		CSipMessage * psttResponse = SipCreateResponse( &m_clsSetup, pclsMessage, SIP_NOT_IMPLEMENTED );
		if( psttResponse )
		{
			SendSipMessage( psttResponse );
		}
	}
}

/**
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
 * @brief UDP SIP �޽��� ���� ������ ������ ���ҽ�Ų��.
 */
void CSipStack::DecreateUdpThreadCount()
{
	m_clsMutex.acquire();
	--m_iUdpThreadRunCount;
	m_clsMutex.release();
}

/**
 * @brief Transaction List �� ũ�⸦ ���ڿ��� �����Ѵ�.
 * @param pszBuf		���ڿ� ����
 * @param iBufSize	���ڿ� ������ ũ��
 */
void CSipStack::GetString( std::string & strBuf )
{
}

void CSipStack::GetICTString( std::string & strBuf )
{
	m_clsICT.GetString( strBuf );
}

/**
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
	while( m_iUdpThreadRunCount > 0 || m_bStackThreadRun )
	{
		MiliSleep( 20 );
	}

	closesocket( m_iUdpSocket );
	m_iUdpSocket = INVALID_SOCKET;

	m_bStopEvent = false;

	return true;
}
