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
#include "CallMap.h"

CSipStack	gclsSipStack;
CSipServer gclsSipServer;

CSipServer::CSipServer()
{
}

CSipServer::~CSipServer()
{
}

/**
 * @ingroup McuEmulator
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
 * @ingroup McuEmulator
 * @brief SIP 서버를 종료한다.
 */
void CSipServer::Stop( )
{
	gclsSipStack.Stop();
	gclsSipStack.Final();
}

/**
 * @ingroup McuEmulator
 * @brief SIP 요청 메시지 수신 이벤트 핸들러
 * @param iThreadId		쓰레드 아이디
 * @param pclsMessage SIP 요청 메시지
 * @returns SIP 요청 메시지를 처리하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServer::RecvRequest( int iThreadId, CSipMessage * pclsMessage )
{
	if( pclsMessage->IsMethod( "REGISTER" ) )
	{
		//CSipMessage * pclsResponse = pclsMessage->CreateResponseWithToTag( SIP_NOT_IMPLEMENTED );
		CSipMessage * pclsResponse = pclsMessage->CreateResponseWithToTag( SIP_OK );
		if( pclsResponse )
		{
			return gclsSipStack.SendSipMessage( pclsResponse );
		}
	}
	else if( pclsMessage->IsMethod( SIP_METHOD_INVITE ) )
	{
		gclsCallMap.Insert( pclsMessage );

		CSipMessage * pclsResponse = pclsMessage->CreateResponseWithToTag( SIP_OK );
		if( pclsResponse )
		{
			pclsResponse->m_clsContentType = pclsMessage->m_clsContentType;
			pclsResponse->m_strBody = pclsMessage->m_strBody;
			pclsResponse->m_iContentLength = pclsMessage->m_iContentLength;

			return gclsSipStack.SendSipMessage( pclsResponse );
		}
	}
	else if( pclsMessage->IsMethod( "UPDATE" ) || pclsMessage->IsMethod( "INFO" ) )
	{
		CSipMessage * pclsResponse = pclsMessage->CreateResponseWithToTag( SIP_OK );
		if( pclsResponse )
		{
			return gclsSipStack.SendSipMessage( pclsResponse );
		}
	}
	else if( pclsMessage->IsMethod( "BYE" ) )
	{
		gclsCallMap.Delete( pclsMessage );

		CSipMessage * pclsResponse = pclsMessage->CreateResponseWithToTag( SIP_OK );
		if( pclsResponse )
		{
			return gclsSipStack.SendSipMessage( pclsResponse );
		}
	}

	return false;
}

/**
 * @ingroup McuEmulator
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
 * @ingroup McuEmulator
 * @brief SIP 메시지 전송 timeout callback method
 * @param iThreadId		SIP stack 의 UDP 쓰레드 아이디
 * @param pclsMessage 수신된 SIP 응답 메시지
 * @returns SIP 응답 메시지를 처리한 경우 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServer::SendTimeout( int iThreadId, CSipMessage * pclsMessage )
{
	return false;
}
