/*----------------------------------------------------------------------------
 * TimeString.cpp
 *----------------------------------------------------------------------------
 * programmer : Yee Young Han ( websearch@naver.com )
 *            : http://blog.naver.com/websearch/
 *----------------------------------------------------------------------------
 * start date : 2011/3/29
 *----------------------------------------------------------------------------
 * comment    : 시간 관련 문자열 생성 메소드 정의
 *----------------------------------------------------------------------------
 */

#include "SipParserDefine.h"
#include <stdio.h>
#include "TimeString.h"

/**
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
 * @brief 시간 변수를 시간 문자열 변수에 저장한다.
 * @param iTime			시간 변수
 * @param pszTime		시간 문자열이 저장될 변수
 * @param iTimeSize pszTime 변수의 크기
 */
void GetDateTimeString( time_t iTime, char * pszTime, int iTimeSize )
{
	struct tm	sttTm;

#ifdef WIN32
	localtime_s( &sttTm, &iTime );
#else
	localtime_r( &iTime, &sttTm );	
#endif

	snprintf( pszTime, iTimeSize, "%04d%02d%02d%02d%02d%02d", sttTm.tm_year + 1900, sttTm.tm_mon + 1, sttTm.tm_mday
		, sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec );
}

void GetDateTimeString( char * pszTime, int iTimeSize )
{
	time_t		iTime;

	time( &iTime );

	GetDateTimeString( iTime, pszTime, iTimeSize );
}

void GetDateString( time_t iTime, char * pszDate, int iDateSize )
{
	struct tm	sttTm;

#ifdef WIN32
	localtime_s( &sttTm, &iTime );
#else
	localtime_r( &iTime, &sttTm );	
#endif

	snprintf( pszDate, iDateSize, "%04d%02d%02d", sttTm.tm_year + 1900, sttTm.tm_mon + 1, sttTm.tm_mday );
}

void GetDateString( char * pszDate, int iDateSize )
{
	time_t		iTime;

	time( &iTime );

	GetDateString( iTime, pszDate, iDateSize );
}

void GetTimeString( time_t iTime, char * pszTime, int iTimeSize )
{
	struct tm	sttTm;

#ifdef WIN32
	localtime_s( &sttTm, &iTime );
#else
	localtime_r( &iTime, &sttTm );	
#endif

	snprintf( pszTime, iTimeSize, "%02d%02d%02d", sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec );
}

void GetTimeString( char * pszTime, int iTimeSize )
{
	time_t		iTime;

	time( &iTime );

	GetTimeString( iTime, pszTime, iTimeSize );
}
