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

#include "SipDefine.h"
#include "SipContentType.h"

CSipContentType::CSipContentType(void)
{
}

CSipContentType::~CSipContentType(void)
{
}

int CSipContentType::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos, iSubTypePos = -1, iParamPos = -1;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == '/' )
		{
			m_strType.append( pszText, iPos );
			iSubTypePos = iPos + 1;
		}
		else if( iSubTypePos != -1 )
		{
			if( pszText[iPos] == ';' || pszText[iPos] == ',' )
			{
				m_strSubType.append( pszText + iSubTypePos, iPos - iSubTypePos );
				iParamPos = iPos + 1;
				break;
			}
		}
		else if( pszText[iPos] == ',' )
		{
			break;
		}
	}

	if( m_strType.empty() )
	{
		m_strType.append( pszText );
	}
	else if( iSubTypePos != -1 && m_strSubType.empty() )
	{
		m_strSubType.append( pszText + iSubTypePos );
	}

	if( iParamPos != -1 )
	{
		int iCurPos = iPos;

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

			iPos = ParseSipParameter( m_clsParamList, pszText + iCurPos, iTextLen - iCurPos );
			if( iPos == -1 ) return -1;
			iCurPos += iPos;
		}

		return iCurPos;
	}
	else
	{
		return iPos;
	}
}

int CSipContentType::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	int iLen, iPos;

	iLen = snprintf( pszText, iTextSize, "%s/%s", m_strType.c_str(), m_strSubType.c_str() );

	iPos = MakeSipParameterString( m_clsParamList, pszText + iLen, iTextSize - iLen );
	if( iPos == -1 ) return -1;
	iLen += iPos;

	return iLen;
}

void CSipContentType::Clear()
{
	m_strType.clear();
	m_strSubType.clear();
	m_clsParamList.clear();
}

bool CSipContentType::Empty()
{
	if( m_strType.empty() || m_strSubType.empty() ) return true;

	return false;
}

int ParseSipContentType( SIP_CONTENT_TYPE_LIST & clsList, const char * pszText, int iTextLen )
{
	int iPos, iCurPos = 0;
	CSipContentType	clsContentType;

	while( iCurPos < iTextLen )
	{
		if( pszText[iCurPos] == ' ' || pszText[iCurPos] == '\t' || pszText[iCurPos] == ',' )
		{
			++iCurPos;
			continue;
		}

		iPos = clsContentType.Parse( pszText + iCurPos, iTextLen - iCurPos );
		if( iPos == -1 ) return -1;
		iCurPos += iPos;

		clsList.push_back( clsContentType );
	}

	return iCurPos;
}
