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
#include "SipCredential.h"
#include "SipChallenge.h"
#include "StringUtility.h"
#include "MemoryDebug.h"

CSipCredential::CSipCredential()
{
}

CSipCredential::~CSipCredential()
{
}

/**
 * @ingroup SipParser
 * @brief SIP 헤더 문자열을 파싱하여 CSipCredential 클래스의 멤버 변수에 저장한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipCredential::Parse( const char * pszText, int iTextLen )
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

		if( !strcmp( pszName, "username" ) )
		{
			DeQuoteString( itList->m_strValue, m_strUserName );
		}
		else if( !strcmp( pszName, "realm" ) )
		{
			DeQuoteString( itList->m_strValue, m_strRealm );
		}
		else if( !strcmp( pszName, "nonce" ) )
		{
			DeQuoteString( itList->m_strValue, m_strNonce );
		}
		else if( !strcmp( pszName, "uri" ) )
		{
			DeQuoteString( itList->m_strValue, m_strUri );
		}
		else if( !strcmp( pszName, "response" ) )
		{
			DeQuoteString( itList->m_strValue, m_strResponse );
		}
		else if( !strcmp( pszName, "algorithm" ) )
		{
			m_strAlgorithm = itList->m_strValue;
		}
		else if( !strcmp( pszName, "cnonce" ) )
		{
			DeQuoteString( itList->m_strValue, m_strCnonce );
		}
		else if( !strcmp( pszName, "opaque" ) )
		{
			DeQuoteString( itList->m_strValue, m_strOpaque );
		}
		else if( !strcmp( pszName, "qop" ) )
		{
			m_strQop = itList->m_strValue;
		}
		else if( !strcmp( pszName, "nc" ) )
		{
			m_strNonceCount = itList->m_strValue;
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
int CSipCredential::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	int iLen = 0;

	iLen = snprintf( pszText, iTextSize, "%s ", m_strType.c_str() );
	char * pszData = pszText + iLen;
	int	iPos = 0;
	iTextSize -= iLen;

	if( CSipChallenge::SetQuoteString( pszData, iPos, iTextSize, "username", m_strUserName ) == false ) return -1;
	if( CSipChallenge::SetQuoteString( pszData, iPos, iTextSize, "realm", m_strRealm ) == false ) return -1;
	if( CSipChallenge::SetQuoteString( pszData, iPos, iTextSize, "nonce", m_strNonce ) == false ) return -1;
	if( CSipChallenge::SetQuoteString( pszData, iPos, iTextSize, "uri", m_strUri ) == false ) return -1;
	if( CSipChallenge::SetQuoteString( pszData, iPos, iTextSize, "response", m_strResponse ) == false ) return -1;
	if( CSipChallenge::SetString( pszData, iPos, iTextSize, "algorithm", m_strAlgorithm ) == false ) return -1;
	if( CSipChallenge::SetQuoteString( pszData, iPos, iTextSize, "cnonce", m_strCnonce ) == false ) return -1;
	if( CSipChallenge::SetQuoteString( pszData, iPos, iTextSize, "opaque", m_strOpaque ) == false ) return -1;
	if( CSipChallenge::SetString( pszData, iPos, iTextSize, "qop", m_strQop ) == false ) return -1;
	if( CSipChallenge::SetString( pszData, iPos, iTextSize, "nc", m_strNonceCount ) == false ) return -1;

	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
	{
		if( CSipChallenge::SetString( pszData, iPos, iTextSize, itList->m_strName.c_str(), itList->m_strValue ) == false ) return -1;
	}

	return iLen + iPos;
}

/**
 * @ingroup SipParser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CSipCredential::Clear()
{
	m_strType.clear();
	m_strUserName.clear();
	m_strRealm.clear();
	m_strNonce.clear();
	m_strUri.clear();
	m_strResponse.clear();
	m_strAlgorithm.clear();
	m_strCnonce.clear();
	m_strOpaque.clear();
	m_strQop.clear();
	m_strNonceCount.clear();
}

/**
 * @brief SIP credential 문자열을 파싱하여서 credential 리스트에 저장한다.
 * @param clsList credential 리스트
 * @param pszText credential 문자열
 * @param iTextLen credential 문자열 길이 
 * @returns 성공하면 파싱한 문자열 길이를 리턴하고 그렇지 않으면 -1 을 리턴한다.
 */
int ParseSipCredential( SIP_CREDENTIAL_LIST & clsList, const char * pszText, int iTextLen )
{
	int iPos;
	CSipCredential	clsCredential;

	iPos = clsCredential.Parse( pszText, iTextLen );
	if( iPos == -1 ) return -1;

	clsList.push_back( clsCredential );
	return iPos;
}
