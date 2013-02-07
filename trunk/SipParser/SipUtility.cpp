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
#include "SipMd5.h"
#include <string>
#include <time.h>
#include <stdlib.h>

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

	if( iBranchSize > ( iLen + (int)sizeof(sttTime) * 8 / 6 ) )
	{
		SipMakePrintString( ( unsigned char *)&sttTime, sizeof(sttTime), pszBranch + iLen, iBranchSize - iLen );
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

	if( iCallIdSize > ( iLen + (int)sizeof(sttTime) * 8 / 6 ) )
	{
		SipMakePrintString( ( unsigned char *)&sttTime, sizeof(sttTime), pszCallId + iLen, iCallIdSize - iLen );
	}
}

/**
 * @ingroup SipParser
 * @brief 입력 문자열을 출력할 수 있는 64개의 문자들로 변환하여서 출력 문자열에 저장한다.
 * @param pszInput		입력 문자열
 * @param iInputSize	입력 문자열 크기
 * @param pszOutput		출력 문자열
 * @param iOutputSize	출력 문자열 크기
 */
bool SipMakePrintString( const unsigned char * pszInput, int iInputSize, char * pszOutput, int iOutputSize )
{
	if( garrChar[0] == '\0' )
	{
		InitRandomString();
	}

	char	cType = 0;
	unsigned char	cValue = 0, cNextValue = 0;
	int		iOutputLen = 0;

	--iOutputSize;

	for( int i = 0; i < iInputSize; ++i )
	{
		if( iOutputLen >= iOutputSize ) return false;

		switch( cType )
		{
		case 0:
			cValue = pszInput[i] >> 2;
			cNextValue = pszInput[i] & 0x03;
			pszOutput[iOutputLen++] = garrChar[ cValue ];
			++cType;
			break;
		case 1:
			cValue = cNextValue << 4 | pszInput[i] >> 4;
			cNextValue = pszInput[i] & 0x0F;
			pszOutput[iOutputLen++] = garrChar[ cValue ];
			++cType;
			break;
		case 2:
			cValue = cNextValue << 2 | pszInput[i] >> 6;
			cNextValue = pszInput[i] & 0x3F;
			pszOutput[iOutputLen++] = garrChar[ cValue ];

			if( iOutputLen >= iOutputSize ) return false;

			pszOutput[iOutputLen++] = garrChar[ cNextValue ];
			cType = 0;
			break;
		}
	}

	pszOutput[iOutputLen] = '\0';

	return true;
}

/**
 * @ingroup SipUserAgent
 * @brief 평문을 MD5 문자열로 변환한다.
 * @param pszInput 평문
 * @param szResult MD5 문자열 저장 변수
 */
void SipMd5String21( char * pszInput, char szResult[22] )
{
	unsigned char szDigest[16];

	SipMd5Byte( pszInput, szDigest );

	SipMakePrintString( szDigest, 16, szResult, 22 );
}
