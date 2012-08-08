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
#include "SipCallId.h"
#include "SipUtility.h"

CSipCallId::CSipCallId()
{
}

CSipCallId::~CSipCallId()
{
}

/**
 * @ingroup SipParser
 * @brief SIP 헤더 문자열을 파싱하여 CSipCallId 클래스의 멤버 변수에 저장한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipCallId::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int iPos;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == '@' )
		{
			m_strName.append( pszText, iPos );
			m_strHost.append( pszText + iPos + 1 );
			break;
		}
	}

	if( m_strName.empty() )
	{
		m_strName.append( pszText );
	}

	return iTextLen;
}

/**
 * @ingroup SipParser
 * @brief Call-ID 문자열을 작성한다.
 * @param pszText		SIP 헤더의 값을 저장할 문자열 변수
 * @param iTextSize	pszText 변수의 크기
 * @returns 성공하면 작성한 문자열 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipCallId::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	if( m_strHost.empty() )
	{
		return snprintf( pszText, iTextSize, "%s", m_strName.c_str() );
	}
	else
	{
		return snprintf( pszText, iTextSize, "%s@%s", m_strName.c_str(), m_strHost.c_str() );
	}
}

/**
 * @ingroup SipParser
 * @brief Call-ID 문자열을 작성한다.
 * @param strText		SIP 헤더의 값을 저장할 문자열 변수
 */
void CSipCallId::ToString( std::string & strText )
{
	strText = m_strName;

	if( m_strHost.empty() == false )
	{
		strText.append( "@" );
		strText.append( m_strHost );
	}
}

/**
 * @ingroup SipParser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CSipCallId::Clear()
{
	m_strName.clear();
	m_strHost.clear();
}

/**
 * @ingroup SipParser
 * @brief 멤버변수가 저장되어 있지 않으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 * @return 멤버변수가 저장되어 있지 않으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipCallId::Empty()
{
	return m_strName.empty();
}

/**
 * @ingroup SipParser
 * @brief Call-ID 가 동일한지 검사한다.
 * @param pclsCallId 비교할 Call-ID
 * @returns Call-ID 가 동일하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipCallId::IsEqual( CSipCallId * pclsCallId )
{
	if( pclsCallId == NULL ) return false;

	if( !strcmp( m_strName.c_str(), pclsCallId->m_strName.c_str() ) && !strcmp( m_strHost.c_str(), pclsCallId->m_strHost.c_str() ) ) return true;

	return false;
}

/**
 * @ingroup SipParser
 * @brief 새로운 Call-ID 를 생성한다.
 * @param pszHost 호스트 이름 또는 IP 주소
 */
void CSipCallId::Make( const char * pszHost )
{
	char	szName[SIP_CALL_ID_NAME_MAX_SIZE];

	SipMakeCallIdName( szName, sizeof(szName) );

	m_strName = szName;

	if( pszHost ) m_strHost = pszHost;
}
