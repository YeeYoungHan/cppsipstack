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

#include "TimeUtility.h"
#include "MemoryDebug.h"

#ifdef WIN32
#include <sys/timeb.h>

#pragma comment( lib, "Winmm" ) 

int gettimeofday( struct timeval *tv, struct timezone *tz )
{
  struct _timeb timebuffer;

  _ftime( &timebuffer );
  tv->tv_sec = (long) timebuffer.time;
  tv->tv_usec = timebuffer.millitm * 1000;

  return 0;
}

#endif

/**
 * @ingroup SipPlatform
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
 * @ingroup SipPlatform
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
	req.tv_nsec = (int)( iMiliSecond % 1000 ) * 1000000;

	nanosleep( &req, &rem );
#endif
}

/**
 * @ingroup SipPlatform
 * @brief 현재 시간을 ms 단위로 리턴한다.
 * @returns 현재 시간을 ms 단위로 리턴한다.
 */
uint64_t GetCurrentMiliSecond()
{
#ifdef WIN32
  return timeGetTime();
#else 
  struct timeval mtv;

  gettimeofday(&mtv, NULL);

  return (uint64_t)(mtv.tv_sec) * 1000 + (uint64_t)(mtv.tv_usec) / 1000;
#endif
}
