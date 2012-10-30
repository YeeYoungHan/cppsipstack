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
#include "SipDeleteQueue.h"

/**
 * @ingroup SipStack
 * @brief ������ - ���� ������ �ʱ�ȭ��Ű�� transaction list �� SIP stack �� �����Ų��.
 */
CSipStack::CSipStack()
{
	m_bStopEvent = false;
	m_bStackThreadRun = false;
	m_hUdpSocket = INVALID_SOCKET;
	m_bStarted = false;
	m_iUdpThreadRunCount = 0;

	m_clsICT.SetSipStack( this );
	m_clsNICT.SetSipStack( this );
	m_clsIST.SetSipStack( this );
	m_clsNIST.SetSipStack( this );
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

	m_hTcpSocket = TcpListen( m_clsSetup.m_iLocalTcpPort, 255, NULL );

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
 * @brief SIP ����/�߽� �޽��� callback �������̽��� �߰��Ѵ�.
 * @param pclsLog SIP ����/�߽� �޽��� callback �������̽�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipStack::AddNetworkLog( ISipNetworkLog * pclsLog )
{
	if( pclsLog == NULL ) return false;

	SIP_NETWORK_LOG_LIST::iterator it;
	bool	bFound = false;

	for( it = m_clsNetLogList.begin(); it != m_clsNetLogList.end(); ++it )
	{
		if( *it == pclsLog )
		{
			bFound = true;
			break;
		}
	}

	if( bFound == false )
	{
		m_clsNetLogList.push_back( pclsLog );
	}

	return true;
}

/**
 * @ingroup SipStack
 * @brief SIP stack �� SIP �޽����� �������� ������ SIP stack �� SIP �޽����� �����ϰ� SIP �޽����� ��Ʈ��ũ�� �����Ѵ�.
 * @param pclsMessage SIP �޽��� ���� ����ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipStack::SendSipMessage( CSipMessage * pclsMessage )
{
	if( pclsMessage == NULL ) return false;

	CheckSipMessage( pclsMessage );
	++pclsMessage->m_iUseCount;

	if( pclsMessage->IsRequest() )
	{
		if( pclsMessage->IsMethod( "INVITE" ) || pclsMessage->IsMethod( "ACK" ) )
		{
			if( m_clsICT.Insert( pclsMessage ) )
			{
				Send( pclsMessage, false );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
		else
		{
			if( m_clsNICT.Insert( pclsMessage ) )
			{
				Send( pclsMessage, false );
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
				Send( pclsMessage, false );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
		else
		{
			if( m_clsNIST.Insert( pclsMessage ) )
			{
				Send( pclsMessage, false );
				--pclsMessage->m_iUseCount;
				return true;
			}
		}
	}

	delete pclsMessage;

	return false;
}

/**
 * @ingroup SipStack
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
 * @brief SIP �޽����� ��Ʈ��ũ�� �����Ѵ�.
 * @param pclsMessage		SIP �޽��� ���� ����ü
 * @param bCheckMessage	SIP �޽����� �˻��Ͽ��� �ʼ� ����� �߰�/�����ϴ°�?
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipStack::Send( CSipMessage * pclsMessage, bool bCheckMessage )
{
	const char * pszIp = NULL;
	int iPort = -1;

	if( bCheckMessage )
	{
		CheckSipMessage( pclsMessage );
	}

	if( pclsMessage->IsRequest() )
	{
		SIP_FROM_LIST::iterator itList = pclsMessage->m_clsRouteList.begin();
		if( itList == pclsMessage->m_clsRouteList.end() )
		{
			if( pclsMessage->m_clsReqUri.m_strHost.empty() ) return false;

			pszIp = pclsMessage->m_clsReqUri.m_strHost.c_str();
			iPort = pclsMessage->m_clsReqUri.m_iPort;
		}
		else
		{
			pszIp = itList->m_clsUri.m_strHost.c_str();
			iPort = itList->m_clsUri.m_iPort;
		}
	}
	else
	{
		SIP_VIA_LIST::iterator itViaList = pclsMessage->m_clsViaList.begin();
		if( itViaList == pclsMessage->m_clsViaList.end() ) return false;

		const char * pszTemp;

		pszTemp = SearchSipParameter( itViaList->m_clsParamList, "rport" );
		if( pszTemp )
		{
			iPort = atoi( pszTemp );
		}
		else
		{
			iPort = itViaList->m_iPort;
		}

		pszIp = SearchSipParameter( itViaList->m_clsParamList, "received" );
		if( pszIp == NULL )
		{
			pszIp = itViaList->m_strHost.c_str();
		}
	}

	if( iPort <= 0 ) iPort = 5060;

	if( pszIp[0] == '\0' ) return false;

	if( pclsMessage->m_strPacket.empty() )
	{
		char	szPacket[SIP_MESSAGE_MAX_LEN];

		if( pclsMessage->ToString( szPacket, sizeof(szPacket) ) == -1 ) return false;
		pclsMessage->m_strPacket = szPacket;
	}

	m_clsUdpSendMutex.acquire();
	bool bRes = UdpSend( m_hUdpSocket, pclsMessage->m_strPacket.c_str(), (int)pclsMessage->m_strPacket.length(), pszIp, iPort );
	m_clsUdpSendMutex.release();

	NetworkLog( true, pclsMessage->m_strPacket.c_str(), pszIp, iPort );

	return bRes;
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
 * @brief SIP ����/�߽� �޽��� callback �������̽��� SIP ����/�߽� ������ �����Ѵ�.
 * @param bSend			�߽��̸� true, �����̸� false
 * @param pszPacket SIP �޽���
 * @param pszIp			�߽� IP �Ǵ� ���� IP
 * @param iPort			�߽� ��Ʈ �Ǵ� ���� ��Ʈ
 */
void CSipStack::NetworkLog( bool bSend, const char * pszPacket, const char * pszIp, int iPort )
{
	SIP_NETWORK_LOG_LIST::iterator	itList;

	for( itList = m_clsNetLogList.begin(); itList != m_clsNetLogList.end(); ++itList )
	{
		(*itList)->SipLog( bSend, pszPacket, pszIp, iPort );
	}
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
	while( m_iUdpThreadRunCount > 0 || m_bStackThreadRun )
	{
		MiliSleep( 20 );
	}

	closesocket( m_hUdpSocket );
	m_hUdpSocket = INVALID_SOCKET;

	m_bStopEvent = false;

	return true;
}

/**
 * @ingroup SipStack
 * @brief ������ SIP �޽������� �ʿ��� ����� �������� ���� ��� default ����� �����Ѵ�.
 * @param pclsMessage ������ SIP �޽���
 */
void CSipStack::CheckSipMessage( CSipMessage * pclsMessage )
{
	if( pclsMessage->IsRequest() )
	{
		if( pclsMessage->m_clsViaList.size() == 0 )
		{
			pclsMessage->AddVia( m_clsSetup.m_strLocalIp.c_str(), m_clsSetup.m_iLocalUdpPort );
		}
	}

	if( pclsMessage->m_strSipVersion.empty() )
	{
		pclsMessage->m_strSipVersion = SIP_VERSION;
	}

	if( pclsMessage->m_clsContactList.size() == 0 )
	{
		CSipFrom clsContact;

		clsContact.m_clsUri.m_strProtocol = "sip";
		clsContact.m_clsUri.m_strUser = pclsMessage->m_clsFrom.m_clsUri.m_strUser;
		clsContact.m_clsUri.m_strHost = m_clsSetup.m_strLocalIp;
		clsContact.m_clsUri.m_iPort = m_clsSetup.m_iLocalUdpPort;

		pclsMessage->m_clsContactList.push_back( clsContact );
	}

	if( m_clsSetup.m_strUserAgent.empty() )
	{
		pclsMessage->m_strUserAgent = SIP_USER_AGENT;
	}
	else
	{
		pclsMessage->m_strUserAgent = m_clsSetup.m_strUserAgent;
	}

	if( pclsMessage->m_iMaxForwards == -1 )
	{
		pclsMessage->m_iMaxForwards = SIP_MAX_FORWARDS;
	}
}
