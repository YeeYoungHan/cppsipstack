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
 * @brief SIP ������ �����Ѵ�.
 * @param clsSetup SIP stack ���� �׸��� ������ ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipServer::Start( CSipStackSetup & clsSetup )
{
	// QQQ: Call Routeing �� IP-PBX ������ ����� �־�� �Ѵ�.

	gclsUserAgent.m_pclsCallBack = this;
	if( gclsUserAgent.Start( clsSetup ) == false ) return false;

	gclsSipStack.AddCallBack( this );
	gclsSipStack.AddNetworkLog( this );

	return true;
}

/**
 * @ingroup KSipServer
 * @brief SIP ��û �޽��� ���� �̺�Ʈ �ڵ鷯
 * @param iThreadId		������ ���̵�
 * @param pclsMessage SIP ��û �޽���
 * @returns SIP ��û �޽����� ó���ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief SIP ���� �޽��� ���� �̺�Ʈ �ڵ鷯
 * @param iThreadId		������ ���̵�
 * @param pclsMessage SIP ���� �޽���
 * @returns SIP ���� �޽����� ó���ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipServer::RecvResponse( int iThreadId, CSipMessage * pclsMessage )
{
	return false;
}

/**
 * @ingroup KSipServer
 * @brief SIP ���� �޽����� �����Ͽ��� �����Ѵ�.
 * @param pclsMessage SIP ��û �޽���
 * @param iStatusCode SIP ���� �ڵ�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipServer::SendResponse( CSipMessage * pclsMessage, int iStatusCode )
{
	CSipMessage * pclsResponse = pclsMessage->CreateResponseWithToTag( iStatusCode );
	if( pclsResponse == NULL ) return false;

	gclsSipStack.SendSipMessage( pclsResponse );

	return true;
}
