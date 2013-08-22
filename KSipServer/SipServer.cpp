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
#include "SipServerMap.h"
#include "RtpMap.h"
#include "Log.h"
#include "SipMd5.h"
#include "XmlUser.h"
#include "Directory.h"
#include "TimeString.h"
#include "KSipServer.h"

CSipServer gclsSipServer;
CSipUserAgent gclsUserAgent;

#include "SipServerRegister.hpp"
#include "SipServerUserAgent.hpp"

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
	gclsSipServerMap.SetSipUserAgentRegisterInfo();

	gclsUserAgent.m_pclsCallBack = this;
	if( gclsUserAgent.Start( clsSetup ) == false ) return false;

	gclsSipStack.AddCallBack( this );

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

bool CSipServer::SendTimeout( int iThreadId, CSipMessage * pclsMessage )
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

/**
 * @ingroup KSipServer
 * @brief CDR 을 저장한다.
 * @param pszCallId		SIP Call-ID
 * @param iSipStatus	INVITE 응답 코드
 */
void CSipServer::SaveCdr( const char * pszCallId, int iSipStatus )
{
	CSipCdr	clsCdr;

	if( gclsUserAgent.GetCdr( pszCallId, &clsCdr ) )
	{
		char	szInviteTime[15], szStartTime[15], szEndTime[15];

		if( clsCdr.m_sttInviteTime.tv_sec )
		{
			GetDateTimeString( clsCdr.m_sttInviteTime.tv_sec, szInviteTime, sizeof(szInviteTime) );
		}
		else
		{
			szInviteTime[0] = '\0';
		}

		if( clsCdr.m_sttStartTime.tv_sec )
		{
			GetDateTimeString( clsCdr.m_sttStartTime.tv_sec, szStartTime, sizeof(szStartTime) );
		}
		else
		{
			szStartTime[0] = '\0';
		}

		if( clsCdr.m_sttEndTime.tv_sec )
		{
			GetDateTimeString( clsCdr.m_sttEndTime.tv_sec, szEndTime, sizeof(szEndTime) );
		}
		else
		{
			GetDateTimeString( szEndTime, sizeof(szEndTime) );
		}

		if( gclsSetup.m_eType == E_DT_XML )
		{
			std::string	strFileName = gclsSetup.m_strCdrFolder;
			char	szFileName[20];
			FILE	* fd;

			GetDateString( szFileName, sizeof(szFileName) );
			strcat( szFileName, ".csv" );

			CDirectory::AppendName( strFileName, szFileName );

			m_clsMutex.acquire();
			fd = fopen( strFileName.c_str(), "a" );
			if( fd )
			{
				fprintf( fd, "%s,%s,%s,%s,%s,%d,%s\n", clsCdr.m_strFromId.c_str(), clsCdr.m_strToId.c_str(), szInviteTime, szStartTime, szEndTime, iSipStatus, clsCdr.m_strCallId.c_str() );
				fclose( fd );
			}
			m_clsMutex.release();
		}
		else if( gclsSetup.m_eType == E_DT_MYSQL )
		{
			char	szSQL[1024];

			snprintf( szSQL, sizeof(szSQL), "INSERT INTO SipCdr( FromId, ToId, CallId, InviteTime, StartTime, EndTime, SipStatus ) VALUES( '%s', '%s', '%s', '%s', '%s', '%s', %d )"
				, clsCdr.m_strFromId.c_str(), clsCdr.m_strToId.c_str(), clsCdr.m_strCallId.c_str(), szInviteTime, szStartTime, szEndTime, iSipStatus );

			DbInsert( szSQL );
		}
	}
}
