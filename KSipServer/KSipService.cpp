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

#ifdef WIN32

#include "SipParserDefine.h"
#include <windows.h>
#include <stdio.h>
#include "KSipServer.h"

SERVICE_STATUS_HANDLE	g_hSrv;
DWORD					g_iNowState;

extern int ServerMain( );

/**
 * @ingroup KSipServer
 * @brief change service current status 
 * @param dwState		status
 * @param dwAccept	afforded status
 */
void KSipServiceSetStatus( DWORD dwState, DWORD dwAccept = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE )
{
	SERVICE_STATUS ss;

	ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ss.dwCurrentState = dwState;
	ss.dwControlsAccepted = dwAccept;
	ss.dwWin32ExitCode = 0;
	ss.dwServiceSpecificExitCode = 0;
	ss.dwCheckPoint = 0;
	ss.dwWaitHint = 0;

	// save current status.
	g_iNowState = dwState;
	SetServiceStatus( g_hSrv, &ss );
}

/**
 * @ingroup KSipServer
 * @brief handler function. This function handle command from service control window.
 * @param fdwControl request status
 */
void KSipServiceHandler( DWORD fdwControl )
{
	// if current status is equal input status, there is nothing to do.
	if( fdwControl == g_iNowState ) return;

	switch( fdwControl ) 
	{
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_STOP:
		KSipServiceSetStatus( SERVICE_STOP_PENDING, 0 );
		gbStop = true;
		break;
	case SERVICE_CONTROL_INTERROGATE:
	default:
		KSipServiceSetStatus( g_iNowState );
		break;
	}
}

/**
 * @ingroup KSipServer
 * @brief MS 윈도우에서 서비스 실행시 호출되는 메소드. KSipServer 를 시작한다.
 * @param  
 * @param  
 */
void KSipServiceMain( DWORD , LPTSTR * )
{
	// regist service handler.
	g_hSrv = RegisterServiceCtrlHandler( SERVICE_NAME, (LPHANDLER_FUNCTION)KSipServiceHandler );
	if( g_hSrv == 0 ) 
	{
		return;
	}

	// set status that service is starting.
	KSipServiceSetStatus(SERVICE_START_PENDING);
	
	// set status that service is working.
	KSipServiceSetStatus(SERVICE_RUNNING);

	ServerMain();

	// QQQ : if this process is terminated with signal, below code must be executed.
	KSipServiceSetStatus( SERVICE_STOPPED );
}

/**
 * @ingroup KSipServer
 * @brief MS 윈도우용 서비스로 KSipServer 를 시작한다.
 */
void KSipServiceStart()
{
	SERVICE_TABLE_ENTRY ste[]={
		{ SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)KSipServiceMain },
		{ NULL        , NULL}
	};

	StartServiceCtrlDispatcher(ste);
}

/**
 * @ingroup KSipServer
 * @brief 프로그램 실행 폴더를 리턴한다.
 * @returns 프로그램 실행 폴더를 리턴한다.
 */
char * GetProgramDirectory( )
{
	static char	szDir[1024];

	if( strlen(szDir) == 0 )
	{
		int		i;
		HMODULE	hThis;

		hThis = GetModuleHandle( NULL );

		GetModuleFileName( hThis, szDir, sizeof(szDir));
		for( i = (int)strlen( szDir) - 1; i >= 0; i-- )
		{
			if( szDir[i] == '\\' ) 
			{
				szDir[i] = '\0';
				break;
			}
		}
	}

	return szDir;
}

/**
 * @ingroup KSipServer
 * @brief 설정 파일 이름을 리턴한다.
 * @returns 설정 파일 이름을 리턴한다.
 */
char * GetConfigFileName()
{
	static char szConfigFileName[1024];

	snprintf( szConfigFileName, sizeof(szConfigFileName), "%s\\%s", GetProgramDirectory(), CONFIG_FILENAME );

	return szConfigFileName;
}

#endif
