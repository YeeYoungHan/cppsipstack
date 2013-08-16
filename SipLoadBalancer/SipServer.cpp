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
 * @brief SIP 서버를 시작한다.
 * @param clsSetup SIP stack 설정 항목을 저장한 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipServer::Start( CSipStackSetup & clsSetup )
{
	gclsSipStack.AddCallBack( this );

	if( gclsSipStack.Start( clsSetup ) == false ) return false;

	return true;
}

/**
 * @ingroup SipLoadBalancer
 * @brief SIP 요청 메시지 수신 이벤트 핸들러
 * @param iThreadId		쓰레드 아이디
 * @param pclsMessage SIP 요청 메시지
 * @returns SIP 요청 메시지를 처리하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
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
			const char * pszBranch = itVia->SelectParamValue( "branch" );
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
				}
				else
				{
					pclsRequest->AddRoute( clsUserInfo.m_strSipServerIp.c_str(), clsUserInfo.m_iSipServerPort );
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
 * @brief SIP 응답 메시지 수신 이벤트 핸들러
 * @param iThreadId		쓰레드 아이디
 * @param pclsMessage SIP 응답 메시지
 * @returns SIP 응답 메시지를 처리하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServer::RecvResponse( int iThreadId, CSipMessage * pclsMessage )
{
	CSipMessage * pclsResponse = new CSipMessage();
	if( pclsResponse )
	{
		// SIP 요청 메시지를 전송한 호스트로 전달한다.
		*pclsResponse = *pclsMessage;
		pclsResponse->m_clsViaList.pop_front();

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
 * @brief SIP 메시지 전송 timeout callback method
 * @param iThreadId		SIP stack 의 UDP 쓰레드 아이디
 * @param pclsMessage 수신된 SIP 응답 메시지
 * @returns SIP 응답 메시지를 처리한 경우 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServer::SendTimeout( int iThreadId, CSipMessage * pclsMessage )
{
	// QQQ: SIP 서버로 전송한 요청 메시지에 대한 응답 메시지가 없으면 해당 SIP 서버를 사용하지 않으므로 수정한다.

	return false;
}
