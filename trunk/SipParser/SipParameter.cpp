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

CSipParameter::CSipParameter(void)
{
}

CSipParameter::~CSipParameter(void)
{
}

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

void CSipParameter::Clear()
{
	m_strName.clear();
	m_strValue.clear();
}

int ParseSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszText, int iTextLen )
{
	CSipParameter clsParam;

	int iPos = clsParam.Parse( pszText, iTextLen );
	if( iPos == -1 ) return -1;

	clsList.push_back( clsParam );

	return iPos;
}

bool SearchSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszName, std::string & strValue )
{
	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		if( !strcmp( itList->m_strName.c_str(), pszName ) )
		{
			strValue = itList->m_strValue;
			return true;
		}
	}

	return false;
}

void AddSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszName, const char * pszValue )
{
	CSipParameter clsParam;

	clsParam.m_strName = pszName;
	clsParam.m_strValue = pszValue;

	clsList.push_back( clsParam );
}

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
