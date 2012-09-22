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
#include "KSipServer.h"
#include <windows.h>
#include <stdio.h>

/**
 * @ingroup KSipServer
 * @brief 현재 프로그램이 서비스에 등록되어 있는지 확인한다.
 * @returns 현재 프로그램이 서비스에 등록되어 있으면 true 를 리턴한다.
 *					그렇지 않으면 false 를 리턴한다.
 */
bool IsExistService()
{
	SC_HANDLE	hScm, hSrv;
	bool		fisResult = false;

	hScm = OpenSCManager( NULL, NULL, SC_MANAGER_CREATE_SERVICE );
	if( hScm )
	{
		hSrv = OpenService( hScm, SERVICE_NAME, SERVICE_ALL_ACCESS );
		if( hSrv ) 
		{
			CloseServiceHandle( hSrv );
			fisResult = true;
		}

		CloseServiceHandle(hScm);
	}

	return fisResult;
}

/**
 * @ingroup KSipServer
 * @brief 현재 프로그램을 서비스로 등록한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool InstallService( )
{
	if( IsExistService() )
	{
		printf( "KSipServer is already installed.\n" );
		return false;
	}

	SC_HANDLE hScm, hSrv;
	char SrvPath[MAX_PATH];
	char szDisplayName[255];
	SERVICE_DESCRIPTION lpDes;

	snprintf( szDisplayName, sizeof(szDisplayName), "%s", SERVICE_DISPLAY_NAME );

	// SCM을 연다
	hScm=OpenSCManager( NULL, NULL, SC_MANAGER_CREATE_SERVICE );
	if (hScm==NULL) 
	{
		printf( "Can not open SCM. Start dos with administrator authority\n" );
		return false;
	}

	// 등록할 서비스 파일이 있는지 조사해 보고 경로를 구한다.
	::GetModuleFileName( NULL, SrvPath, sizeof(SrvPath) );

	// 서비스를 등록한다.
	hSrv=CreateService( hScm
		        , SERVICE_NAME
					  , szDisplayName
					  , SERVICE_PAUSE_CONTINUE | SERVICE_CHANGE_CONFIG
					  ,	SERVICE_WIN32_OWN_PROCESS
					  , SERVICE_AUTO_START
					  , SERVICE_ERROR_IGNORE
					  , SrvPath
					  ,	NULL,NULL,NULL,NULL,NULL);
	
	if( hSrv == NULL ) 
	{
		printf( "Install is failed.\n" );
	} 
	else 
	{
		// 설명을 등록한다.
		lpDes.lpDescription = SERVICE_DESCRIPTION_STRING;
		ChangeServiceConfig2( hSrv, SERVICE_CONFIG_DESCRIPTION, &lpDes );

		printf( "Installed.\n" );
		CloseServiceHandle(hSrv);
	}

	CloseServiceHandle(hScm);

	return true;
}

/**
 * @ingroup KSipServer
 * @brief 현재 프로그램을 서비스에서 등록 해제한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool UninstallService( )
{
	SC_HANDLE hScm, hSrv;
	SERVICE_STATUS	ss;

	// SCM을 연다
	hScm = OpenSCManager( NULL, NULL, SC_MANAGER_CREATE_SERVICE );
	if( hScm == NULL ) 
	{
		printf( "Can not open SCM.\n" );
		return false;
	}

	// 서비스의 핸들을 구한다.
	hSrv = OpenService( hScm, SERVICE_NAME, SERVICE_ALL_ACCESS );
	if( hSrv == NULL ) 
	{
		CloseServiceHandle(hScm);
		printf( "KSipServer is not installed.\n" );
		return false;
	}

	// 실행중이면 중지시킨다.
	QueryServiceStatus( hSrv, &ss );
	if( ss.dwCurrentState != SERVICE_STOPPED ) 
	{
		ControlService( hSrv, SERVICE_CONTROL_STOP, &ss );
		Sleep( 2000 );
	}

	// 서비스 제거
	if( DeleteService( hSrv ) ) 
	{
		printf( "Service is deleted.\n" );
	} 
	else 
	{
		printf( "Deleting service is failed.\n" );
	}

	CloseServiceHandle( hSrv );
	CloseServiceHandle( hScm );

	return true;
}

#endif
