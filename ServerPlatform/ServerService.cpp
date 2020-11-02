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

#include "SipPlatformDefine.h"
#include "ServerServicePrivate.h"

#ifdef WIN32

#include "Directory.h"
#include <windows.h>
#include <stdio.h>
#include "MemoryDebug.h"

static SERVICE_STATUS_HANDLE	ghServiceStatus;
static DWORD					giNowState;

/**
 * @ingroup ServerPlatform
 * @brief change service current status 
 * @param dwState		status
 * @param dwAccept	afforded status
 */
void ServiceSetStatus( DWORD dwState, DWORD dwAccept = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE )
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
	giNowState = dwState;
	SetServiceStatus( ghServiceStatus, &ss );
}

/**
 * @ingroup ServerPlatform
 * @brief handler function. This function handle command from service control window.
 * @param fdwControl request status
 */
void ServiceHandler( DWORD fdwControl )
{
	// if current status is equal input status, there is nothing to do.
	if( fdwControl == giNowState ) return;

	switch( fdwControl ) 
	{
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		ServiceSetStatus( SERVICE_STOP_PENDING, 0 );
		gbStop = true;
		break;
	case SERVICE_CONTROL_INTERROGATE:
	default:
		ServiceSetStatus( giNowState );
		break;
	}
}

/**
 * @ingroup ServerPlatform
 * @brief MS 윈도우에서 서비스 실행시 호출되는 메소드. KSipServer 를 시작한다.
 * @param  
 * @param  
 */
void ServiceMain( DWORD , LPTSTR * )
{
	// regist service handler.
	ghServiceStatus = RegisterServiceCtrlHandler( gclsService.m_strName.c_str(), (LPHANDLER_FUNCTION)ServiceHandler );
	if( ghServiceStatus == 0 ) 
	{
		return;
	}

	// set status that service is starting.
	ServiceSetStatus(SERVICE_START_PENDING);
	
	// set status that service is working.
	ServiceSetStatus(SERVICE_RUNNING);

	gpServerFunc();

	// QQQ : if this process is terminated with signal, below code must be executed.
	ServiceSetStatus( SERVICE_STOPPED );
}

/**
 * @ingroup ServerPlatform
 * @brief MS 윈도우용 서비스로 KSipServer 를 시작한다.
 */
void ServiceStart()
{
	SERVICE_TABLE_ENTRY ste[]={
		{ (LPSTR)gclsService.m_strName.c_str(), (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL        , NULL}
	};

	StartServiceCtrlDispatcher(ste);
}

/**
 * @ingroup ServerPlatform
 * @brief 설정 파일 이름을 리턴한다.
 * @returns 설정 파일 이름을 리턴한다.
 */
const char * GetConfigFileName()
{
	static char szConfigFileName[1024];

	// full path 로 설정파일이름을 입력한 경우
	if( gclsService.m_strConfigFileName.length() > 3 &&	!strncmp( gclsService.m_strConfigFileName.c_str() + 1, ":\\", 2 ) )
	{
		return gclsService.m_strConfigFileName.c_str();
	}

	snprintf( szConfigFileName, sizeof(szConfigFileName), "%s\\%s", CDirectory::GetProgramDirectory(), gclsService.m_strConfigFileName.c_str() );

	return szConfigFileName;
}

#else

const char * GetConfigFileName()
{
	return gclsService.m_strConfigFileName.c_str();
}

#endif
