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
#include "SipParameter.h"
#include "MemoryDebug.h"

CSipParameter::CSipParameter()
{
}

CSipParameter::~CSipParameter()
{
}

/**
 * @ingroup SipParser
 * @brief Parameter 문자열을 파싱하여 CSipParameter 클래스의 멤버 변수에 저장한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipParameter::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos, iValuePos = -1;
	bool	bStartQuote = false;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == '"' )
		{
			bStartQuote = bStartQuote ? false : true;
		}
		else if( pszText[iPos] == '=' )
		{
			if( bStartQuote ) continue;
			m_strName.append( pszText, iPos );
			iValuePos = iPos + 1;
		}
		else if( pszText[iPos] == ',' || pszText[iPos] == ';' || pszText[iPos] == '&' || pszText[iPos] == '?' )
		{
			if( bStartQuote ) continue;
			break;
		}
		else if( pszText[iPos] == '\r' )
		{
			break;
		}
	}

	if( iPos > 0 )
	{
		if( iValuePos != -1 )
		{
			m_strValue.append( pszText + iValuePos, iPos - iValuePos );
		}
		else
		{
			m_strName.append( pszText, iPos );
		}
		return iPos;
	}

	return -1;
}

/**
 * @ingroup SipParser
 * @brief SIP 메시지에 포함된 문자열을 작성한다.
 * @param pszText		SIP 헤더의 값을 저장할 문자열 변수
 * @param iTextSize	pszText 변수의 크기
 * @returns 성공하면 작성한 문자열 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipParameter::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	if( m_strName.empty() ) return 0;

	if( m_strValue.empty() )
	{
		return snprintf( pszText, iTextSize, "%s", m_strName.c_str() );
	}

	return snprintf( pszText, iTextSize, "%s=%s", m_strName.c_str(), m_strValue.c_str() );
}

/**
 * @ingroup SipParser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CSipParameter::Clear()
{
	m_strName.clear();
	m_strValue.clear();
}

#if 0
/**
 * @ingroup SipParser
 * @brief parameter 리스트 문자열을 파싱하여서 parameter 리스트 객체에 저장한다.
 * @param clsList		parameter 리스트 객체
 * @param pszText		parameter 리스트 문자열
 * @param iTextLen	parameter 리스트 문자열의 길이
 * @returns 성공하면 파싱한 문자열의 길이를 리턴하고 그렇지 않으면 -1 을 리턴한다.
 */
int ParseSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszText, int iTextLen )
{
	CSipParameter clsParam;

	int iPos = clsParam.Parse( pszText, iTextLen );
	if( iPos == -1 ) return -1;

	clsList.push_back( clsParam );

	return iPos;
}

/**
 * @ingroup SipParser
 * @brief parameter 리스트에서 parameter 이름에 대한 값을 검색한다.
 * @param clsList		parameter 리스트 객체
 * @param pszName		parameter 이름
 * @param strValue	parameter 값을 저장할 변수
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool SearchSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszName, std::string & strValue )
{
	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		if( !strcasecmp( itList->m_strName.c_str(), pszName ) )
		{
			strValue = itList->m_strValue;
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipParser
 * @brief parameter 리스트에서 parameter 이름에 대한 값을 검색한다.
 * @param clsList		parameter 리스트 객체
 * @param pszName		parameter 이름
 * @returns 성공하면 parameter 값을 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
const char * SearchSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszName )
{
	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		if( !strcasecmp( itList->m_strName.c_str(), pszName ) )
		{
			return itList->m_strValue.c_str();
		}
	}

	return NULL;
}

/**
 * @ingroup SipParser
 * @brief parameter 리스트에 paramter 를 추가한다.
 * @param clsList		parameter 리스트 객체
 * @param pszName		parameter 이름
 * @param pszValue	parameter 값
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool InsertSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszName, const char * pszValue )
{
	if( pszName == NULL ) return false;

	CSipParameter clsParam;

	clsParam.m_strName = pszName;
	if( pszValue ) clsParam.m_strValue = pszValue;

	clsList.push_back( clsParam );

	return true;
}

/**
 * @ingroup SipParser
 * @brief parameter 리스트에 paramter 를 수정한다.
 * @param clsList		parameter 리스트 객체
 * @param pszName		parameter 이름
 * @param pszValue	parameter 값
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool UpdateSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszName, const char * pszValue )
{
	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		if( !strcasecmp( itList->m_strName.c_str(), pszName ) )
		{
			itList->m_strValue = pszValue;

			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipParser
 * @brief parameter 리스트 객체를 parameter 리스트 문자열로 제작한다.
 * @param clsList		parameter 리스트 객체
 * @param pszText		parameter 리스트 문자열을 저장할 변수
 * @param iTextSize parameter 리스트 문자열의 크기
 * @returns parameter 리스트 문자열의 길이를 리턴한다.
 */
int MakeSipParameterString( SIP_PARAMETER_LIST & clsList, char * pszText, int iTextSize )
{
	SIP_PARAMETER_LIST::iterator	itList;
	int iLen = 0, iPos;

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		if( iLen >= iTextSize ) return -1;
		pszText[iLen++] = ';';
		iPos = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( iPos == -1 ) return -1;
		iLen += iPos;
	}

	return iLen;
}

#endif
