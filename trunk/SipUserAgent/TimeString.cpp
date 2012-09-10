/*----------------------------------------------------------------------------
 * TimeString.cpp
 *----------------------------------------------------------------------------
 * programmer : Yee Young Han ( websearch@naver.com )
 *            : http://blog.naver.com/websearch/
 *----------------------------------------------------------------------------
 * start date : 2011/3/29
 *----------------------------------------------------------------------------
 * comment    : �ð� ���� ���ڿ� ���� �޼ҵ� ����
 *----------------------------------------------------------------------------
 */

#include "SipParserDefine.h"
#include <stdio.h>
#include "TimeString.h"

/**
 * @brief ���� �ð��� ��,��,��,��,��,�� ����ü�� �����Ѵ�.
 * @param iTime ���� �ð�
 * @param sttTm ��,��,��,��,��,�� ����ü
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
 * @brief �ð� ������ �ð� ���ڿ� ������ �����Ѵ�.
 * @param iTime			�ð� ����
 * @param pszTime		�ð� ���ڿ��� ����� ����
 * @param iTimeSize pszTime ������ ũ��
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
