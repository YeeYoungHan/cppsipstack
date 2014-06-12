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
 * @brief SIP ��û �޽��� ���� callback method
 * @param iThreadId		SIP stack �� UDP ������ ���̵�
 * @param pclsMessage ���ŵ� SIP ��û �޽���
 * @returns SIP ��û �޽����� ó���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
	else if( pclsMessage->IsMethod( SIP_METHOD_PRACK ) )
	{
		return RecvPrackRequest( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( SIP_METHOD_REFER ) )
	{
		return RecvReferRequest( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( SIP_METHOD_NOTIFY ) )
	{
		return RecvNotifyRequest( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( SIP_METHOD_MESSAGE ) )
	{
		return RecvMessageRequest( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( SIP_METHOD_OPTIONS ) )
	{
		return RecvOptionsRequest( iThreadId, pclsMessage );
	}

	return false;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP ���� �޽��� ���� callback method
 * @param iThreadId		SIP stack �� UDP ������ ���̵�
 * @param pclsMessage ���ŵ� SIP ���� �޽���
 * @returns SIP ���� �޽����� ó���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
	else if( pclsMessage->IsMethod( SIP_METHOD_PRACK ) )
	{
		return true;
	}
	else if( pclsMessage->IsMethod( SIP_METHOD_REFER ) )
	{
		return RecvReferResponse( iThreadId, pclsMessage );
	}
	else if( pclsMessage->IsMethod( SIP_METHOD_NOTIFY ) )
	{
		return true;
	}
	else if( pclsMessage->IsMethod( SIP_METHOD_MESSAGE ) )
	{
		return true;
	}
	else if( pclsMessage->IsMethod( SIP_METHOD_OPTIONS ) )
	{
		return true;
	}

	return false;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP �޽��� ���� timeout callback method
 * @param iThreadId		SIP stack �� UDP ������ ���̵�
 * @param pclsMessage ���ŵ� SIP ���� �޽���
 * @returns SIP ���� �޽����� ó���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief TCP/TLS ���� ���� �̺�Ʈ �ڵ鷯
 * @param pszIp IP �ּ�
 * @param iPort ��Ʈ ��ȣ
 * @param eTransport ��������
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
 * @brief SIP ������ ���� �̺�Ʈ �ڵ鷯
 * @param iThreadId ������ ��ȣ
 */
void CSipUserAgent::ThreadEnd( int iThreadId )
{
	if( m_pclsCallBack ) m_pclsCallBack->EventThreadEnd( iThreadId );
}
