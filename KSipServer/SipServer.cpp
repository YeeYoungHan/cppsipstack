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
#include "SipServerSetup.h"
#include "SipUtility.h"
#include "UserMap.h"
#include "NonceMap.h"
#include "CallMap.h"
#include "Log.h"
#include "Md5.h"
#include "XmlUser.h"

CSipServer gclsSipServer;
CSipUserAgent gclsUserAgent;

#include "SipServerRegister.hpp"
#include "SipServerUserAgent.hpp"
#include "SipServerLog.hpp"

CSipServer::CSipServer()
{
}

CSipServer::~CSipServer()
{
}

/**
 * @ingroup KSipServer
 * @brief SIP 서버를 시작한다.
 * @param clsSetup SIP stack 설정 항목을 저장한 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipServer::Start( CSipStackSetup & clsSetup )
{
	// QQQ: Call Routeing 할 IP-PBX 정보를 등록해 주어야 한다.

	gclsUserAgent.m_pclsCallBack = this;
	if( gclsUserAgent.Start( clsSetup ) == false ) return false;

	gclsSipStack.AddCallBack( this );
	gclsSipStack.AddNetworkLog( this );

	return true;
}

/**
 * @ingroup KSipServer
 * @brief SIP 요청 메시지 수신 이벤트 핸들러
 * @param iThreadId		쓰레드 아이디
 * @param pclsMessage SIP 요청 메시지
 * @returns SIP 요청 메시지를 처리하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServer::RecvRequest( int iThreadId, CSipMessage * pclsMessage )
{
	if( pclsMessage->IsMethod( "REGISTER" ) )
	{
		return RecvRequestRegister( iThreadId, pclsMessage );
	}

	return false;
}

/**
 * @ingroup KSipServer
 * @brief SIP 응답 메시지 수신 이벤트 핸들러
 * @param iThreadId		쓰레드 아이디
 * @param pclsMessage SIP 응답 메시지
 * @returns SIP 응답 메시지를 처리하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServer::RecvResponse( int iThreadId, CSipMessage * pclsMessage )
{
	return false;
}

/**
 * @ingroup KSipServer
 * @brief SIP 응답 메시지를 생성하여서 전송한다.
 * @param pclsMessage SIP 요청 메시지
 * @param iStatusCode SIP 응답 코드
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipServer::SendResponse( CSipMessage * pclsMessage, int iStatusCode )
{
	CSipMessage * pclsResponse = pclsMessage->CreateResponseWithToTag( iStatusCode );
	if( pclsResponse == NULL ) return false;

	gclsSipStack.SendSipMessage( pclsResponse );

	return true;
}
