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

/**
 * @ingroup SipStack
 * @brief 두 개의 시간의 차이를 리턴한다.
 * @param psttOld 시작 시간
 * @param psttNew 종료 시간
 * @returns 두 개의 시간의 차이를 리턴한다.
 */
int DiffTimeval( struct timeval * psttOld, struct timeval * psttNew )
{
	int	iRet;

	if( psttNew->tv_usec >= psttOld->tv_usec )
	{
		iRet = ( psttNew->tv_usec - psttOld->tv_usec ) / 1000;
		iRet += ( psttNew->tv_sec - psttOld->tv_sec ) * 1000;
	}
	else
	{
		iRet = 1000 + ( psttNew->tv_usec - psttOld->tv_usec ) / 1000;
		iRet += ( psttNew->tv_sec - psttOld->tv_sec - 1 ) * 1000;
	}

	return iRet;
}

/**
 * @ingroup SipStack
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

/**
 * @ingroup SipStack
 * @brief ms 동안 대기한다.
 * @param iMiliSecond ms
 */
void MiliSleep( int iMiliSecond )
{
#ifdef WIN32
	Sleep( iMiliSecond );
#else
	struct timespec req;
	struct timespec rem;

	req.tv_sec = (int) iMiliSecond / 1000;
	req.tv_nsec = (int)( iMiliSecond % 1000 ) * 1000;

	nanosleep( &req, &rem );
#endif
}
