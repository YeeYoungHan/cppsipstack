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

#include "SipServer.h"
#include "SipUtility.h"
#include "SipServerMap.h"
#include "UserMap.h"
#include "Log.h"
#include "MemoryDebug.h"

CSipStack	gclsSipStack;
CSipServer gclsSipServer;

CSipServer::CSipServer()
{
}

CSipServer::~CSipServer()
{
}

/**
 * @ingroup SipLoadBalancer
 * @brief SIP ������ �����Ѵ�.
 * @param clsSetup SIP stack ���� �׸��� ������ ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipServer::Start( CSipStackSetup & clsSetup )
{
	gclsSipStack.AddCallBack( this );

	if( gclsSipStack.Start( clsSetup ) == false ) return false;

	return true;
}

/**
 * @ingroup SipLoadBalancer
 * @brief SIP ��û �޽��� ���� �̺�Ʈ �ڵ鷯
 * @param iThreadId		������ ���̵�
 * @param pclsMessage SIP ��û �޽���
 * @returns SIP ��û �޽����� ó���ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipServer::RecvRequest( int iThreadId, CSipMessage * pclsMessage )
{
	CUserInfo clsUserInfo;
	bool bSipServerSent = false;

	if( gclsSipServerMap.Select( pclsMessage->m_strClientIp.c_str(), pclsMessage->m_iClientPort ) )
	{
		bSipServerSent = true;
	}
	else
	{
		gclsUserMap.Insert( pclsMessage );
	}

	if( gclsUserMap.Select( pclsMessage, clsUserInfo ) )
	{
		SIP_VIA_LIST::iterator itVia = pclsMessage->m_clsViaList.begin();
		if( itVia != pclsMessage->m_clsViaList.end() )
		{
			const char * pszBranch = itVia->SelectParamValue( SIP_BRANCH );
			std::string strBranch;

			if( pszBranch )
			{
				strBranch = pszBranch;
				strBranch.append( "slb" );
			}

			CSipMessage * pclsRequest = new CSipMessage();
			if( pclsRequest )
			{
				*pclsRequest = *pclsMessage;
				
				pclsRequest->AddVia( gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.m_iLocalUdpPort, strBranch.c_str() );

				if( bSipServerSent )
				{
					pclsRequest->AddRoute( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort, clsUserInfo.m_eTransport );
					
					pclsRequest->AddRecordRoute( pclsRequest->m_strClientIp.c_str(), pclsRequest->m_iClientPort );
					pclsRequest->AddRecordRoute( gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.m_iLocalUdpPort, clsUserInfo.m_eTransport );
				}
				else
				{
					if( pclsRequest->m_clsRouteList.size() > 0 )
					{
						pclsRequest->m_clsRouteList.pop_front();
					}

					if( pclsRequest->m_clsRouteList.size() == 0 )
					{
						pclsRequest->AddRoute( clsUserInfo.m_strSipServerIp.c_str(), clsUserInfo.m_iSipServerPort );
					}
				}

				SIP_FROM_LIST::iterator itContact = pclsRequest->m_clsContactList.begin();
				if( itContact != pclsRequest->m_clsContactList.end() )
				{
					itContact->m_clsUri.m_strHost = gclsSipStack.m_clsSetup.m_strLocalIp;
					itContact->m_clsUri.m_iPort = gclsSipStack.m_clsSetup.m_iLocalUdpPort;
				}

				gclsSipStack.SendSipMessage( pclsRequest );
				return true;
			}
		}
		else
		{
			CLog::Print( LOG_ERROR, "Via list count is 0" );
		}
	}
	else
	{
		CLog::Print( LOG_ERROR, "user is not found" );
	}

	return false;
}

/**
 * @ingroup SipLoadBalancer
 * @brief SIP ���� �޽��� ���� �̺�Ʈ �ڵ鷯
 * @param iThreadId		������ ���̵�
 * @param pclsMessage SIP ���� �޽���
 * @returns SIP ���� �޽����� ó���ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipServer::RecvResponse( int iThreadId, CSipMessage * pclsMessage )
{
	if( pclsMessage->m_iStatusCode == SIP_TRYING && pclsMessage->IsMethod( SIP_METHOD_INVITE ) )
	{
		// SIP stack ���� 100 Trying �� �����Ͽ����Ƿ� �߰��� 100 Trying �� �������� �ʴ´�.
		return true;
	}

	CSipMessage * pclsResponse = new CSipMessage();
	if( pclsResponse )
	{
		// SIP ��û �޽����� ������ ȣ��Ʈ�� �����Ѵ�.
		*pclsResponse = *pclsMessage;
		pclsResponse->m_clsViaList.pop_front();

		pclsResponse->m_clsRecordRouteList.clear();

		SIP_FROM_LIST::iterator itContact = pclsResponse->m_clsContactList.begin();
		if( itContact != pclsResponse->m_clsContactList.end() )
		{
			itContact->m_clsUri.m_strHost = gclsSipStack.m_clsSetup.m_strLocalIp;
			itContact->m_clsUri.m_iPort = gclsSipStack.m_clsSetup.m_iLocalUdpPort;
		}

		gclsSipStack.SendSipMessage( pclsResponse );
		return true;
	}

	return false;
}

/**
 * @ingroup SipLoadBalancer
 * @brief SIP �޽��� ���� timeout callback method
 * @param iThreadId		SIP stack �� UDP ������ ���̵�
 * @param pclsMessage ���ŵ� SIP ���� �޽���
 * @returns SIP ���� �޽����� ó���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipServer::SendTimeout( int iThreadId, CSipMessage * pclsMessage )
{
	// QQQ: SIP ������ ������ ��û �޽����� ���� ���� �޽����� ������ �ش� SIP ������ ������� �����Ƿ� �����Ѵ�.

	return false;
}
