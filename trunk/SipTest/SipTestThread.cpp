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

#include "stdafx.h"
#include "SipStackDefine.h"
#include "SipUserAgent.h"
#include "Setup.h"
#include "TimeUtility.h"
#include "ServerUtility.h"
#include "TestInfo.h"

static bool gbStopTestThread = false;
static bool gbTestThreadRun = false;
static HWND ghWnd = NULL;

void SendLog( const char * fmt, ... )
{
	va_list		ap;
	char			szBuf[8192];

	va_start( ap, fmt );
	vsnprintf( szBuf, sizeof(szBuf)-1, fmt, ap );
	va_end( ap );

	SendMessage( ghWnd, SIP_TEST_ID, WM_TEST_MSG, (LPARAM)szBuf );
}

/**
 * @brief �׽�Ʈ ������
 * @param lpParameter 
 * @returns 0 �� �����Ѵ�.
 */
DWORD WINAPI SipTestThread( LPVOID lpParameter )
{
	CSipCallRtp clsRtp;
	CSipCallRoute clsRoute;
	std::string strCallId;

	gbTestThreadRun = true;

	if( gclsTestInfo.CreateRtp() == false ) 
	{
		SendLog( "gclsTestInfo.CreateRtp error" );
		goto FUNC_END;
	}

	clsRtp.m_strIp = gclsSipUserAgent.m_clsSipStack.m_clsSetup.m_strLocalIp;
	clsRtp.m_iPort = gclsTestInfo.m_clsCallerRtp.m_iPort;
	clsRtp.m_iCodec = 0;

	clsRoute.m_strDestIp = gclsSetup.m_strSipServerIp;
	clsRoute.m_iDestPort = gclsSetup.m_iSipServerPort;

	// ��ȭ ���� �׽�Ʈ
	if( gclsSipUserAgent.StartCall( gclsSetup.m_strCallerId.c_str(), gclsSetup.m_strCalleeId.c_str(), &clsRtp, &clsRoute, strCallId ) == false )
	{
		SendLog( "gclsSipUserAgent.StartCall error" );
		goto FUNC_END;
	}

	gclsTestInfo.m_strCallerCallId = strCallId;

	// ��� ��ȭ�� ����� ������ ����Ѵ�.
	while( gclsSipUserAgent.GetCallCount() > 0 )
	{
		Sleep(20);

		if( gbStopTestThread )
		{
			gclsSipUserAgent.StopCallAll();
		}
	}

	gclsTestInfo.CloseRtp();

FUNC_END:
	gbTestThreadRun = false;

	SendMessage( ghWnd, SIP_TEST_ID, WM_TEST_END, 0 );

	return 0;
}

/**
 * @brief �׽�Ʈ �����带 �����Ѵ�.
 * @param hWnd �׽�Ʈ ����� ������ ������ �ڵ�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool StartTestThread( HWND hWnd )
{
	if( gbTestThreadRun ) return false;

	gbStopTestThread = false;
	ghWnd = hWnd;

	if( StartThread( "SipTestThread", SipTestThread, NULL ) == false ) return false;

	return true;
}

/**
 * @brief �׽�Ʈ ������ ���� ���� ��û�Ѵ�.
 */
void StopTestThread()
{
	gbStopTestThread = true;
}

/**
 * @brief �׽�Ʈ �����尡 ����ǰ� �ִ°�?
 * @returns �׽�Ʈ �����尡 ����ǰ� ������ true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool IsTestThreadRun()
{
	return gbTestThreadRun;
}
