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
 * @brief 리눅스에서 일반 사용자로 수정한 후, core dump 파일 생성 가능하게 한다.
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
 * @brief 데몬 모드로 동작한다.
 * @param bIsFork 데몬 모드로 동작하면 true 를 넣어주고 그렇지 않으면 false 를 넣어준다.
 * @returns true 를 리턴한다.
 */
bool Fork( bool bIsFork )
{
#ifndef WIN32
	if( bIsFork )
	{
	  if( fork() != 0 ) exit(0);
	  
	  // 1번째 자식 프로세스 시작 -> 세션 리더로 변경한다.
	  setsid();
	  signal( SIGHUP, SIG_IGN );
	  if( fork() != 0 ) exit(1);
	  
	  // 2번째 자식 프로세스 시작 -> 작업 디렉토리를 변경한다.
	  if( chdir( "/tmp" ) == -1 )
		{
			CLog::Print( LOG_ERROR, "chdir(/tmp) error" );
		}
	}
#endif

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief 프로그램 실행 사용자를 수정한다.
 * @param pszUserId 프로그램 실행 사용자
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool ChangeExecuteUser( const char * pszUserId )
{
#ifndef WIN32
	struct passwd sttPassWord, *psttPassWord = NULL;
	char szError[255], szPassWord[1024];

	if( getpwnam_r( pszUserId, &sttPassWord, szPassWord, sizeof(szPassWord), &psttPassWord ) != 0 || psttPassWord == NULL )
	{
		CLog::Print( LOG_ERROR, "getpwnam(%s) error(%d) - %s", pszUserId, errno, strerror_r( errno, szError, sizeof(szError) ) );
		return false;
	}

	if( setuid( psttPassWord->pw_uid ) != 0 )
	{
		CLog::Print( LOG_ERROR, "seteuid(%s) error(%d) - %s", pszUserId, errno, strerror_r( errno, szError, sizeof(szError) ) );
		return false;
	}

	if( psttPassWord->pw_dir )
	{
		if( chdir( psttPassWord->pw_dir ) == -1 )
		{
			CLog::Print( LOG_ERROR, "chdir(%s) error", psttPassWord->pw_dir );
		}
#ifndef __APPLE__
		prctl(PR_SET_DUMPABLE, 1);
#endif
	}
#endif
	
	return true;
}

/**
 * @ingroup SipPlatform
 * @brief 쓰레드를 시작한다.
 * @param pszName					쓰레드 이름
 * @param lpStartAddress	쓰레드 함수
 * @param lpParameter			쓰레드 인자
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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
