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
#include "ServerUtility.h"
#include "Log.h"
#include <stdio.h>
#include <string.h>

#ifndef WIN32
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef __APPLE__
#include <sys/prctl.h>
#endif

#include <signal.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <pwd.h>
#include <pthread.h>

#endif

#include "MemoryDebug.h"

/**
 * @ingroup SipPlatform
 * @brief ���������� �Ϲ� ����ڷ� ������ ��, core dump ���� ���� �����ϰ� �Ѵ�.
 */
void SetCoreDumpEnable()
{
#ifndef WIN32
#ifndef __APPLE__
	prctl(PR_SET_DUMPABLE, 1);
#endif
#endif
}

/**
 * @ingroup SipPlatform
 * @brief ���� ���� �����Ѵ�.
 * @param bIsFork ���� ���� �����ϸ� true �� �־��ְ� �׷��� ������ false �� �־��ش�.
 * @returns true �� �����Ѵ�.
 */
bool Fork( bool bIsFork )
{
#ifndef WIN32
	if( bIsFork )
	{
	  if( fork() != 0 ) exit(0);
	  
	  // 1��° �ڽ� ���μ��� ���� -> ���� ������ �����Ѵ�.
	  setsid();
	  signal( SIGHUP, SIG_IGN );
	  if( fork() != 0 ) exit(1);
	  
	  // 2��° �ڽ� ���μ��� ���� -> �۾� ���丮�� �����Ѵ�.
	  chdir( "/tmp" );
	}
#endif

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief ���α׷� ���� ����ڸ� �����Ѵ�.
 * @param pszUserId ���α׷� ���� �����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool ChangeExecuteUser( const char * pszUserId )
{
#ifndef WIN32
	struct passwd * psttPassWord = getpwnam( pszUserId );
	if( psttPassWord == NULL )
	{
		CLog::Print( LOG_ERROR, "getpwnam(%s) error(%d) - %s", pszUserId, errno, strerror(errno) );
		return false;
	}

	if( setuid( psttPassWord->pw_uid ) != 0 )
	{
		CLog::Print( LOG_ERROR, "seteuid(%s) error(%d) - %s", pszUserId, errno, strerror(errno) );
		return false;
	}

	if( psttPassWord->pw_dir )
	{
		chdir( psttPassWord->pw_dir );
#ifndef __APPLE__
		prctl(PR_SET_DUMPABLE, 1);
#endif
	}
#endif
	
	return true;
}

/**
 * @ingroup SipPlatform
 * @brief �����带 �����Ѵ�.
 * @param pszName					������ �̸�
 * @param lpStartAddress	������ �Լ�
 * @param lpParameter			������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
#ifdef WIN32
bool StartThread( const char * pszName, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter )
{
	DWORD		dwThreadId;
	HANDLE	hThread;

	hThread = CreateThread( NULL, 0, lpStartAddress, lpParameter, 0, &dwThreadId );
	if( hThread == NULL )
	{
		return false;
	}

	CloseHandle( hThread );
	
	return true;
}
#else
bool StartThread( const char * pszName, void *(*lpStartAddress)(void*), void * lpParameter )
{
	pthread_t	iThread;

	if( pthread_create( &iThread, NULL, lpStartAddress, lpParameter ) != 0 )
	{
		return false;
	}

	pthread_detach( iThread );

	return true;
}
#endif
