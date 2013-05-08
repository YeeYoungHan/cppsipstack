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

CSipParameterList::CSipParameterList()
{
}

CSipParameterList::~CSipParameterList()
{
}

int CSipParameterList::ParamParse( const char * pszText, int iTextLen )
{
	CSipParameter clsParam;

	int iPos = clsParam.Parse( pszText, iTextLen );
	if( iPos == -1 ) return -1;

	m_clsList.push_back( clsParam );

	return iPos;
}

int CSipParameterList::ParamToString( char * pszText, int iTextSize )
{
	SIP_PARAMETER_LIST::iterator	itList;
	int iLen = 0, iPos;

	for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
	{
		if( iLen >= iTextSize ) return -1;
		pszText[iLen++] = ';';
		iPos = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( iPos == -1 ) return -1;
		iLen += iPos;
	}

	return iLen;
}

bool CSipParameterList::Insert( const char * pszName, const char * pszValue )
{
	if( pszName == NULL ) return false;

	CSipParameter clsParam;

	clsParam.m_strName = pszName;
	if( pszValue ) clsParam.m_strValue = pszValue;

	m_clsList.push_back( clsParam );

	return true;
}

bool CSipParameterList::Update( const char * pszName, const char * pszValue )
{
	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
	{
		if( !strcasecmp( itList->m_strName.c_str(), pszName ) )
		{
			itList->m_strValue = pszValue;
			return true;
		}
	}

	return false;
}

const char * CSipParameterList::Select( const char * pszName )
{
	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
	{
		if( !strcasecmp( itList->m_strName.c_str(), pszName ) )
		{
			return itList->m_strValue.c_str();
		}
	}

	return NULL;
}
