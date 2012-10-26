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

#include "SipParserDefine.h"
#include "SipUtility.h"
#include "SipMutex.h"
#include <string>
#include <time.h>

static CSipMutex gclsMutex;
static int	giTag;
static int	giBranch;
static int	giCallId;
static std::string gstrSystemId;
static char garrChar[64];

void SipSetSystemId( const char * pszId )
{
	gstrSystemId = pszId;
}

static void InitRandomString()
{
	struct timeval sttTime;

	gettimeofday( &sttTime, NULL );
	srand( sttTime.tv_sec * sttTime.tv_usec );

	int i, iPos = 0;

	for( i = 0; i < 26; ++i )
	{
		garrChar[iPos++] = 'a' + i;
		garrChar[iPos++] = 'A' + i;
	}

	for( i = 0; i < 10; ++i )
	{
		garrChar[iPos++] = '0' + i;
	}

	garrChar[iPos++] = '_';
	garrChar[iPos++] = '-';
}

#ifdef WIN32
#include <sys/timeb.h>

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
 * @ingroup SipParser
 * @brief SIP From/To tag 에 사용할 문자열을 작성한다.
 * @param pszTag		SIP From/To tag 에 사용할 문자열을 저장할 변수
 * @param iTagSize	pszTag 변수의 크기
 */
void SipMakeTag( char * pszTag, int iTagSize )
{
	int		iTag;

	gclsMutex.acquire();
	if( garrChar[0] == '\0' )
	{
		InitRandomString();
	}

	if( giTag <= 0 || giTag > 2000000000 )
	{
		giTag = rand();
	}
	else
	{
		++giTag;
	}

	iTag = giTag;
	gclsMutex.release();

	snprintf( pszTag, iTagSize, "%d", iTag );
}

/**
 * @ingroup SipParser
 * @brief SIP Via branch 에 사용할 문자열을 작성한다.
 * @param pszBranch		SIP Via branch 에 사용할 문자열을 저장할 변수
 * @param iBranchSize	pszBranch 변수의 크기
 */
void SipMakeBranch( char * pszBranch, int iBranchSize )
{
	int		iBranch;
	struct timeval sttTime;

	gclsMutex.acquire();
	if( garrChar[0] == '\0' )
	{
		InitRandomString();
	}

	if( giBranch <= 0 || giBranch > 2000000000 )
	{
		giBranch = rand();
	}
	else
	{
		++giBranch;
	}

	iBranch = giBranch;
	gclsMutex.release();

	int iLen = 0;

	gettimeofday( &sttTime, NULL );

	if( gstrSystemId.empty() )
	{
		iLen = snprintf( pszBranch, iBranchSize, "%sWCSS%x", VIA_PREFIX, iBranch );
	}
	else
	{
		iLen = snprintf( pszBranch, iBranchSize, "%sWCSS%s%x", VIA_PREFIX, gstrSystemId.c_str(), iBranch );
	}

	if( iBranchSize > iLen + 8 )
	{
		pszBranch[iLen++] = garrChar[ ( sttTime.tv_sec >> 24 ) & 63 ];
		pszBranch[iLen++] = garrChar[ ( sttTime.tv_sec >> 16 ) & 63 ];
		pszBranch[iLen++] = garrChar[ ( sttTime.tv_sec >> 8 ) & 63 ];
		pszBranch[iLen++] = garrChar[ ( sttTime.tv_sec & 0xFF ) & 63 ];
		pszBranch[iLen++] = garrChar[ ( sttTime.tv_usec >> 24 ) & 63 ];
		pszBranch[iLen++] = garrChar[ ( sttTime.tv_usec >> 16 ) & 63 ];
		pszBranch[iLen++] = garrChar[ ( sttTime.tv_usec >> 8 ) & 63 ];
		pszBranch[iLen++] = garrChar[ ( sttTime.tv_usec & 0xFF ) & 63 ];
		pszBranch[iLen] = '\0';
	}
}

/**
 * @ingroup SipParser
 * @brief SIP Call-ID 에 사용할 name 문자열을 작성한다.
 * @param pszCallId		SIP Call-ID 에 사용할 name 문자열을 저장할 변수
 * @param iCallIdSize	pszCallId 변수의 크기
 */
void SipMakeCallIdName( char * pszCallId, int iCallIdSize )
{
	int		iCallId;
	struct timeval sttTime;

	gettimeofday( &sttTime, NULL );

	gclsMutex.acquire();
	if( garrChar[0] == '\0' )
	{
		InitRandomString();
	}

	if( giCallId <= 0 || giCallId > 2000000000 )
	{
		giCallId = rand();
	}
	else
	{
		++giCallId;
	}

	iCallId = giCallId;
	gclsMutex.release();

	int iLen = 0;

	if( gstrSystemId.empty() )
	{
		iLen = snprintf( pszCallId, iCallIdSize, "WCSS%x", iCallId );
	}
	else
	{
		iLen = snprintf( pszCallId, iCallIdSize, "WCSS%s%x", gstrSystemId.c_str(), iCallId );
	}

	if( iCallIdSize > iLen + 8 )
	{
		pszCallId[iLen++] = garrChar[ ( sttTime.tv_sec >> 24 ) & 63 ];
		pszCallId[iLen++] = garrChar[ ( sttTime.tv_sec >> 16 ) & 63 ];
		pszCallId[iLen++] = garrChar[ ( sttTime.tv_sec >> 8 ) & 63 ];
		pszCallId[iLen++] = garrChar[ ( sttTime.tv_sec & 0xFF ) & 63 ];
		pszCallId[iLen++] = garrChar[ ( sttTime.tv_usec >> 24 ) & 63 ];
		pszCallId[iLen++] = garrChar[ ( sttTime.tv_usec >> 16 ) & 63 ];
		pszCallId[iLen++] = garrChar[ ( sttTime.tv_usec >> 8 ) & 63 ];
		pszCallId[iLen++] = garrChar[ ( sttTime.tv_usec & 0xFF ) & 63 ];
		pszCallId[iLen] = '\0';
	}
}

/**
 * @ingroup SipParser
 * @brief 입력 문자열을 출력할 수 있는 64개의 문자들로 변환하여서 출력 문자열에 저장한다.
 * @param pszInput		입력 문자열
 * @param iInputSize	입력 문자열 크기
 * @param pszOutput		출력 문자열
 */
void SipMakePrintString( const char * pszInput, int iInputSize, char * pszOutput )
{
	if( garrChar[0] == '\0' )
	{
		InitRandomString();
	}

	for( int i = 0; i < iInputSize; ++i )
	{
		pszOutput[i] = garrChar[ pszInput[i] & 63 ];
	}
}
