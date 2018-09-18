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
#include "SipChallenge.h"
#include "StringUtility.h"
#include "MemoryDebug.h"

CSipChallenge::CSipChallenge()
{
}

CSipChallenge::~CSipChallenge()
{
}

/**
 * @ingroup SipParser
 * @brief SIP 헤더 문자열을 파싱하여 CSipChallenge 클래스의 멤버 변수에 저장한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipChallenge::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int iPos, iCurPos;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == ' ' || pszText[iPos] == '\t' )
		{
			m_strType.append( pszText, iPos );
			++iPos;
			break;
		}
	}

	if( m_strType.empty() ) return -1;
	iCurPos = iPos;

	SIP_PARAMETER_LIST	clsList;
	SIP_PARAMETER_LIST::iterator	itList;
	
	while( iCurPos < iTextLen )
	{
		if( pszText[iCurPos] == ' ' || pszText[iCurPos] == '\t' || pszText[iCurPos] == ',' )
		{
			++iCurPos;
			continue;
		}

		iPos = ParseSipParameter( clsList, pszText + iCurPos, iTextLen - iCurPos );
		if( iPos == -1 ) return -1;
		iCurPos += iPos;
	}

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		const char * pszName = itList->m_strName.c_str();

		if( !strcmp( pszName, "realm" ) )
		{
			DeQuoteString( itList->m_strValue, m_strRealm );
		}
		else if( !strcmp( pszName, "domain" ) )
		{
			DeQuoteString( itList->m_strValue, m_strDomain );
		}
		else if( !strcmp( pszName, "nonce" ) )
		{
			DeQuoteString( itList->m_strValue, m_strNonce );
		}
		else if( !strcmp( pszName, "opaque" ) )
		{
			DeQuoteString( itList->m_strValue, m_strOpaque );
		}
		else if( !strcmp( pszName, "stale" ) )
		{
			m_strStale = itList->m_strValue;
		}
		else if( !strcmp( pszName, "algorithm" ) )
		{
			m_strAlgorithm = itList->m_strValue;
		}
		else if( !strcmp( pszName, "qop" ) )
		{
			DeQuoteString( itList->m_strValue, m_strQop );
		}
		else
		{
			m_clsParamList.push_back( *itList );
		}
	}

	return iCurPos;
}

/**
 * @ingroup SipParser
 * @brief SIP 메시지에 포함된 문자열을 작성한다.
 * @param pszText		SIP 헤더의 값을 저장할 문자열 변수
 * @param iTextSize	pszText 변수의 크기
 * @returns 성공하면 작성한 문자열 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipChallenge::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	int iLen = 0;

	iLen = snprintf( pszText, iTextSize, "%s ", m_strType.c_str() );
	char * pszData = pszText + iLen;
	int	iPos = 0;
	iTextSize -= iLen;

	if( SetQuoteString( pszData, iPos, iTextSize, "realm", m_strRealm ) == false ) return -1;
	if( SetQuoteString( pszData, iPos, iTextSize, "domain", m_strDomain ) == false ) return -1;
	if( SetQuoteString( pszData, iPos, iTextSize, "nonce", m_strNonce ) == false ) return -1;
	if( SetQuoteString( pszData, iPos, iTextSize, "opaque", m_strOpaque ) == false ) return -1;
	if( SetString( pszData, iPos, iTextSize, "stale", m_strStale ) == false ) return -1;
	if( SetString( pszData, iPos, iTextSize, "algorithm", m_strAlgorithm ) == false ) return -1;
	if( SetString( pszData, iPos, iTextSize, "qop", m_strQop ) == false ) return -1;

	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
	{
		if( SetString( pszData, iPos, iTextSize, itList->m_strName.c_str(), itList->m_strValue ) == false ) return -1;
	}

	return iLen + iPos;
}

/**
 * @ingroup SipParser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CSipChallenge::Clear()
{
	m_strType.clear();
	m_strRealm.clear();
	m_strDomain.clear();
	m_strNonce.clear();
	m_strOpaque.clear();
	m_strStale.clear();
	m_strAlgorithm.clear();
	m_strQop.clear();
}

/**
 * @ingroup SipParser
 * @brief SIP challenge 문자열에 이름, 값을 저장한다.
 * @param pszText		SIP chanllenge 문자열
 * @param iTextPos	SIP chanllenge 문자열 변수의 쓰기 위치
 * @param iTextSize SIP chanllenge 문자열 변수 크기
 * @param pszName		이름
 * @param strValue	값
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipChallenge::SetString( char * pszText, int & iTextPos, int iTextSize, const char * pszName, std::string & strValue )
{
	if( strValue.empty() ) return true;

	int iLen = 0;

	if( iTextPos > 0 )
	{
		iLen = snprintf( pszText + iTextPos, iTextSize, ", " );
		if( iLen != 2 ) return false;
		iTextPos += iLen;
	}

	iLen = snprintf( pszText + iTextPos, iTextSize, "%s=%s", pszName, strValue.c_str() );
	if( iLen <= 0 ) return false;
	iTextPos += iLen;

	return true;
}

/**
 * @ingroup SipParser
 * @brief SIP challenge 문자열에 이름, " 가 포함된 값을 저장한다.
 * @param pszText		SIP chanllenge 문자열
 * @param iTextPos	SIP chanllenge 문자열 변수의 쓰기 위치
 * @param iTextSize SIP chanllenge 문자열 변수 크기
 * @param pszName		이름
 * @param strValue	값
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipChallenge::SetQuoteString( char * pszText, int & iTextPos, int iTextSize, const char * pszName, std::string & strValue )
{
	if( strValue.empty() ) return true;

	int iLen = 0;

	if( iTextPos > 0 )
	{
		iLen = snprintf( pszText + iTextPos, iTextSize, ", " );
		if( iLen != 2 ) return false;
		iTextPos += iLen;
	}

	iLen = snprintf( pszText + iTextPos, iTextSize, "%s=\"%s\"", pszName, strValue.c_str() );
	if( iLen <= 0 ) return false;
	iTextPos += iLen;

	return true;
}

/**
 * @ingroup SipParser
 * @brief SIP challenge 문자열을 파싱하여서 challenge 리스트에 저장한다.
 * @param clsList		challenge 리스트
 * @param pszText		SIP challenge 문자열
 * @param iTextLen	SIP challenge 문자열 길이 
 * @returns 성공하면 파싱한 문자열 길이를 리턴하고 그렇지 않으면 -1 을 리턴한다.
 */
int ParseSipChallenge( SIP_CHALLENGE_LIST & clsList, const char * pszText, int iTextLen )
{
	int iPos;
	CSipChallenge	clsChallenge;

	iPos = clsChallenge.Parse( pszText, iTextLen );
	if( iPos == -1 ) return -1;

	clsList.push_back( clsChallenge );
	return iPos;
}
