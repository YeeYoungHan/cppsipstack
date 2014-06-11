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
 * @brief TCP/TLS ���� ���ῡ ���� callback �޼ҵ带 ȣ���Ѵ�.
 * @param pszIp IP �ּ�
 * @param iPort ��Ʈ ��ȣ
 * @param eTransport ��������
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
 * @brief ������ ���� �̺�Ʈ�� �����Ѵ�.
 * @param iThreadId ������ ���̵� ( 0 ���� ������ ���� )
 */
void CSipStack::ThreadEnd( int iThreadId )
{
	SIP_STACK_CALLBACK_LIST::iterator itList;

	for( itList = m_clsCallBackList.begin(); itList != m_clsCallBackList.end(); ++itList )
	{
		(*itList)->ThreadEnd( iThreadId );
	}
}
