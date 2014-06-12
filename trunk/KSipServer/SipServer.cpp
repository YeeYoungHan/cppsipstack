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
#include "MemoryDebug.h"

CSipServer gclsSipServer;
CSipUserAgent gclsUserAgent;

#include "SipServerRegister.hpp"
#include "SipServerUserAgent.hpp"
#include "SipServerSecurity.hpp"
#include "SipServerPickUp.hpp"

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
	gclsSipServerMap.SetSipUserAgentRegisterInfo();

	if( gclsUserAgent.Start( clsSetup, this, this ) == false ) return false;

	gclsUserAgent.m_clsSipStack.AddCallBack( this );

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
	if( pclsMessage->IsMethod( SIP_METHOD_REGISTER ) )
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
 * @brief SIP �޽��� ���� timeout callback method
 * @param iThreadId		SIP stack �� UDP ������ ���̵�
 * @param pclsMessage ���ŵ� SIP ���� �޽���
 * @returns SIP ���� �޽����� ó���� ��� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipServer::SendTimeout( int iThreadId, CSipMessage * pclsMessage )
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

	gclsUserAgent.m_clsSipStack.SendSipMessage( pclsResponse );

	return true;
}

/**
 * @ingroup KSipServer
 * @brief CDR �� �����Ѵ�.
 * @param pszCallId		SIP Call-ID
 * @param iSipStatus	INVITE ���� �ڵ�
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
}

/**
 * @ingroup KSipServer
 * @brief ��ȭ�� �����Ų��.
 * @param pszCallId		SIP Call-ID
 * @param iSipStatus	���� �ڵ�
 */
void CSipServer::StopCall( const char * pszCallId, int iResponseCode )
{
	SaveCdr( pszCallId, iResponseCode );
	gclsUserAgent.StopCall( pszCallId, iResponseCode );
}
