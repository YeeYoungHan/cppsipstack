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
#include "MemoryDebug.h"

/**
 * @ingroup SipPlatform
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
 * @ingroup SipPlatform
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

/**
 * @ingroup SipPlatform
 * @brief ����Ͻú��� ���ڿ��� �����Ѵ�.
 * @param pszTime		����Ͻú��� ���ڿ� ���� ����
 * @param iTimeSize pszTime ������ ũ��
 */
void GetDateTimeString( char * pszTime, int iTimeSize )
{
	time_t		iTime;

	time( &iTime );

	GetDateTimeString( iTime, pszTime, iTimeSize );
}

/**
 * @ingroup SipPlatform
 * @brief ����� ���ڿ��� �����Ѵ�.
 * @param iTime			�ð�
 * @param pszDate		����� ���ڿ��� ���� ����
 * @param iDateSize pszDate ������ ũ��
 */
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

/**
 * @ingroup SipPlatform
 * @brief ����� ���ڿ��� �����Ѵ�.
 * @param pszDate		����� ���ڿ��� ���� ����
 * @param iDateSize pszDate ������ ũ��
 */
void GetDateString( char * pszDate, int iDateSize )
{
	time_t		iTime;

	time( &iTime );

	GetDateString( iTime, pszDate, iDateSize );
}

/**
 * @ingroup SipPlatform
 * @brief �ú��� ���ڿ��� �����Ѵ�.
 * @param iTime			�ð�
 * @param pszTime		�ú��� ���ڿ� ���� ����
 * @param iTimeSize pszTime ������ ũ��
 */
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

/**
 * @ingroup SipPlatform
 * @brief �ú��� ���ڿ��� �����Ѵ�.
 * @param pszTime		�ú��� ���ڿ� ���� ����
 * @param iTimeSize pszTime ������ ũ��
 */
void GetTimeString( char * pszTime, int iTimeSize )
{
	time_t		iTime;

	time( &iTime );

	GetTimeString( iTime, pszTime, iTimeSize );
}
