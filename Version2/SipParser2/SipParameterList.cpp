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
#include "SipParameterList.h"
#include "SipUtility.h"
#include "MemoryDebug.h"

CSipParameterList::CSipParameterList()
{
}

CSipParameterList::~CSipParameterList()
{
	ClearParam();
}

/**
 * @ingroup SipParser
 * @brief SIP Header 가 , 로 구분되어서 N 개 저장되는 SIP Header 의 parameter list 를 파싱한다.
 * @param pszText		parameter 리스트 문자열
 * @param iTextLen	parameter 리스트 문자열의 길이
 * @returns 성공하면 파싱한 문자열의 길이를 리턴하고 그렇지 않으면 -1 을 리턴한다.
 */
int CSipParameterList::HeaderListParamParse( const char * pszText, int iTextLen )
{
	int iCurPos = 0, iPos;

	while( iCurPos < iTextLen )
	{
		if( pszText[iCurPos] == ' ' || pszText[iCurPos] == '\t' || pszText[iCurPos] == ';' )
		{
			++iCurPos;
			continue;
		}
		else if( pszText[iCurPos] == ',' )
		{
			break;
		}

		iPos = ParamParse( pszText + iCurPos, iTextLen - iCurPos );
		if( iPos == -1 ) return -1;
		iCurPos += iPos;
	}

	return iCurPos;
}

/**
 * @ingroup SipParser
 * @brief parameter 리스트 문자열을 파싱하여서 parameter 리스트 객체에 저장한다.
 * @param pszText		parameter 리스트 문자열
 * @param iTextLen	parameter 리스트 문자열의 길이
 * @returns 성공하면 파싱한 문자열의 길이를 리턴하고 그렇지 않으면 -1 을 리턴한다.
 */
int CSipParameterList::ParamParse( const char * pszText, int iTextLen )
{
	CSipParameter * pclsParam = new CSipParameter();
	if( pclsParam == NULL ) return -1;

	int iPos = pclsParam->Parse( pszText, iTextLen );
	if( iPos == -1 ) 
	{
		delete pclsParam;
		return -1;
	}

	m_clsParamList.push_back( pclsParam );

	return iPos;
}

/**
 * @ingroup SipParser
 * @brief parameter 리스트 객체를 parameter 리스트 문자열로 제작한다.
 * @param pszText		parameter 리스트 문자열을 저장할 변수
 * @param iTextSize parameter 리스트 문자열의 크기
 * @param cBeginSep 최초 구분 문자
 * @param cSep			parameter 간의 구분 문자
 * @returns parameter 리스트 문자열의 길이를 리턴한다.
 */
int CSipParameterList::ParamToString( char * pszText, int iTextSize, char cBeginSep, char cSep )
{
	SIP_PARAMETER_LIST::iterator	itList;
	int iLen = 0, iPos;

	for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
	{
		if( iLen >= iTextSize ) return -1;

		if( itList == m_clsParamList.begin() )
		{
			pszText[iLen++] = cBeginSep;
		}
		else
		{
			pszText[iLen++] = cSep;
		}

		iPos = (*itList)->ToString( pszText + iLen, iTextSize - iLen );
		if( iPos == -1 ) return -1;
		iLen += iPos;
	}

	return iLen;
}

/**
 * @ingroup SipParser
 * @brief parameter list 에서 입력된 이름과 값을 저장한다.
 * @param pszName		parameter 이름
 * @param pszValue	parameter 값
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipParameterList::InsertParam( const char * pszName, const char * pszValue )
{
	if( pszName == NULL ) return false;

	CSipParameter * pclsParam = new CSipParameter();
	if( pclsParam == NULL ) return false;

	pclsParam->m_strName = pszName;
	if( pszValue ) pclsParam->m_strValue = pszValue;

	m_clsParamList.push_back( pclsParam );

	return true;
}

/**
 * @ingroup SipParser
 * @brief parameter list 에서 입력된 이름에 대한 값을 수정한다.
 * @param pszName		parameter 이름
 * @param pszValue	parameter 값
 * @returns parameter 이름이 존재하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipParameterList::UpdateParam( const char * pszName, const char * pszValue )
{
	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
	{
		if( !strcasecmp( (*itList)->m_strName.c_str(), pszName ) )
		{
			(*itList)->m_strValue = pszValue;
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipParser
 * @brief parameter list 에서 입력된 이름을 검색한다.
 * @param pszName		parameter 이름
 * @param strValue	parameter 값을 저장할 변수
 * @returns parameter 이름이 존재하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipParameterList::SelectParam( const char * pszName, std::string & strValue )
{
	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
	{
		if( !strcasecmp( (*itList)->m_strName.c_str(), pszName ) )
		{
			strValue = (*itList)->m_strValue;
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipParser
 * @brief parameter list 에서 입력된 이름을 검색한다.
 * @param pszName parameter 이름
 * @returns parameter 이름이 존재하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipParameterList::SelectParam( const char * pszName )
{
	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
	{
		if( !strcasecmp( (*itList)->m_strName.c_str(), pszName ) )
		{
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipParser
 * @brief parameter list 에서 입력된 이름을 검색한다.
 * @param pszName parameter 이름
 * @returns parameter 이름이 존재하면 해당 값을 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
const char * CSipParameterList::SelectParamValue( const char * pszName )
{
	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
	{
		if( !strcasecmp( (*itList)->m_strName.c_str(), pszName ) )
		{
			return (*itList)->m_strValue.c_str();
		}
	}

	return NULL;
}

/**
 * @ingroup SipParser
 * @brief parameter list 를 삭제한다.
 */
void CSipParameterList::ClearParam()
{
	ClearList< SIP_PARAMETER_LIST, SIP_PARAMETER_LIST::iterator >( m_clsParamList );
}

CSipParameterList & CSipParameterList::operator=( CSipParameterList & clsOld )
{
	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = clsOld.m_clsParamList.begin(); itList != clsOld.m_clsParamList.end(); ++itList )
	{
		CSipParameter * pclsParam = new CSipParameter();
		if( pclsParam )
		{
			*pclsParam = **itList;
			m_clsParamList.push_back( pclsParam );
		}
	}

	return *this;
}
