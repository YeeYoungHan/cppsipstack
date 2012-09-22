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
 * @brief ���� ���α׷��� ���񽺿� ��ϵǾ� �ִ��� Ȯ���Ѵ�.
 * @returns ���� ���α׷��� ���񽺿� ��ϵǾ� ������ true �� �����Ѵ�.
 *					�׷��� ������ false �� �����Ѵ�.
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
 * @brief ���� ���α׷��� ���񽺷� ����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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

	// SCM�� ����
	hScm=OpenSCManager( NULL, NULL, SC_MANAGER_CREATE_SERVICE );
	if (hScm==NULL) 
	{
		printf( "Can not open SCM. Start dos with administrator authority\n" );
		return false;
	}

	// ����� ���� ������ �ִ��� ������ ���� ��θ� ���Ѵ�.
	::GetModuleFileName( NULL, SrvPath, sizeof(SrvPath) );

	// ���񽺸� ����Ѵ�.
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
		// ������ ����Ѵ�.
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
 * @brief ���� ���α׷��� ���񽺿��� ��� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool UninstallService( )
{
	SC_HANDLE hScm, hSrv;
	SERVICE_STATUS	ss;

	// SCM�� ����
	hScm = OpenSCManager( NULL, NULL, SC_MANAGER_CREATE_SERVICE );
	if( hScm == NULL ) 
	{
		printf( "Can not open SCM.\n" );
		return false;
	}

	// ������ �ڵ��� ���Ѵ�.
	hSrv = OpenService( hScm, SERVICE_NAME, SERVICE_ALL_ACCESS );
	if( hSrv == NULL ) 
	{
		CloseServiceHandle(hScm);
		printf( "KSipServer is not installed.\n" );
		return false;
	}

	// �������̸� ������Ų��.
	QueryServiceStatus( hSrv, &ss );
	if( ss.dwCurrentState != SERVICE_STOPPED ) 
	{
		ControlService( hSrv, SERVICE_CONTROL_STOP, &ss );
		Sleep( 2000 );
	}

	// ���� ����
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
