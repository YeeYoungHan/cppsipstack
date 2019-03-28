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
#include "EchoSipServerSetup.h"
#include "Log.h"
#include "MemoryDebug.h"

CSipServer gclsSipServer;
CSipUserAgent gclsUserAgent;

CSipServer::CSipServer()
{
}

CSipServer::~CSipServer()
{
}

/**
 * @ingroup EchoSipServer
 * @brief SIP 서버를 시작한다.
 * @param clsSetup SIP stack 설정 항목을 저장한 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipServer::Start( CSipStackSetup & clsSetup )
{
	if( gclsUserAgent.Start( clsSetup, this, NULL ) == false ) return false;

	return true;
}

void CSipServer::EventRegister( CSipServerInfo * pclsInfo, int iStatus )
{
}

bool CSipServer::EventIncomingRequestAuth( CSipMessage * pclsMessage )
{
	return true;
}

void CSipServer::EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
{

}

void CSipServer::EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{

}

void CSipServer::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{

}

void CSipServer::EventCallEnd( const char * pszCallId, int iSipStatus )
{

}
