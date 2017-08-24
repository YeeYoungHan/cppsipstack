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
#include <stdio.h>
#include "TimeString.h"
#include "StringUtility.h"
#include "MemoryDebug.h"

/**
 * @ingroup SipPlatform
 * @brief 현재 시간을 년,월,일,시,분,초 구조체에 저장한다.
 * @param iTime 현재 시간
 * @param sttTm 년,월,일,시,분,초 구조체
 */
void LocalTime( time_t iTime, struct tm & sttTm )
{
#ifdef WIN32
	localtime_s( &sttTm, &iTime );
#else
	localtime_r( &iTime, &sttTm );	
#endif
}

/**
 * @ingroup SipPlatform
 * @brief 시간 변수를 시간 문자열 변수에 저장한다.
 * @param iTime			시간 변수
 * @param pszTime		시간 문자열이 저장될 변수
 * @param iTimeSize pszTime 변수의 크기
 */
void GetDateTimeString( time_t iTime, char * pszTime, int iTimeSize )
{
	struct tm	sttTm;

	LocalTime( iTime, sttTm );

	snprintf( pszTime, iTimeSize, "%04d%02d%02d%02d%02d%02d", sttTm.tm_year + 1900, sttTm.tm_mon + 1, sttTm.tm_mday
		, sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec );
}

/**
 * @ingroup SipPlatform
 * @brief 년월일시분초 문자열을 저장한다.
 * @param pszTime		년월일시분초 문자열 저장 변수
 * @param iTimeSize pszTime 변수의 크기
 */
void GetDateTimeString( char * pszTime, int iTimeSize )
{
	time_t		iTime;

	time( &iTime );

	GetDateTimeString( iTime, pszTime, iTimeSize );
}

/**
 * @ingroup SipPlatform
 * @brief 년월일 문자열을 저장한다.
 * @param iTime			시간
 * @param pszDate		년월일 문자열을 저장 변수
 * @param iDateSize pszDate 변수의 크기
 */
void GetDateString( time_t iTime, char * pszDate, int iDateSize )
{
	struct tm	sttTm;

	LocalTime( iTime, sttTm );

	snprintf( pszDate, iDateSize, "%04d%02d%02d", sttTm.tm_year + 1900, sttTm.tm_mon + 1, sttTm.tm_mday );
}

/**
 * @ingroup SipPlatform
 * @brief 년월일 문자열을 저장한다.
 * @param pszDate		년월일 문자열을 저장 변수
 * @param iDateSize pszDate 변수의 크기
 */
void GetDateString( char * pszDate, int iDateSize )
{
	time_t		iTime;

	time( &iTime );

	GetDateString( iTime, pszDate, iDateSize );
}

/**
 * @ingroup SipPlatform
 * @brief 시분초 문자열을 저장한다.
 * @param iTime			시간
 * @param pszTime		시분초 문자열 저장 변수
 * @param iTimeSize pszTime 변수의 크기
 */
void GetTimeString( time_t iTime, char * pszTime, int iTimeSize )
{
	struct tm	sttTm;

	LocalTime( iTime, sttTm );

	snprintf( pszTime, iTimeSize, "%02d%02d%02d", sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec );
}

/**
 * @ingroup SipPlatform
 * @brief 시분초 문자열을 저장한다.
 * @param pszTime		시분초 문자열 저장 변수
 * @param iTimeSize pszTime 변수의 크기
 */
void GetTimeString( char * pszTime, int iTimeSize )
{
	time_t		iTime;

	time( &iTime );

	GetTimeString( iTime, pszTime, iTimeSize );
}

/**
 * @ingroup SipPlatform
 * @brief 년월일시분초 문자열을 time_t 로 변환한다.
 * @param pszTime 년월일시분초 문자열
 * @returns 성공하면 time_t 값이 리턴되고 실패하면 0 이 리턴된다.
 */
time_t ParseDateTimeString( const char * pszTime )
{
	struct tm	sttTm;
	int iLen = (int)strlen( pszTime );

	if( iLen < 14 ) return 0;

	memset( &sttTm, 0, sizeof(sttTm) );

	sttTm.tm_year = GetInt( pszTime, 4 ) - 1900;
	sttTm.tm_mon = GetInt( pszTime+4, 2 ) - 1;
	sttTm.tm_mday = GetInt( pszTime+6, 2 );
	sttTm.tm_hour = GetInt( pszTime+8, 2 );
	sttTm.tm_min = GetInt( pszTime+10, 2 );
	sttTm.tm_sec = GetInt( pszTime+12, 2 );

	return mktime( &sttTm );
}
