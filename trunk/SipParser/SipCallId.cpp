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

void CSipCallId::Clear()
{
	m_strName.clear();
	m_strHost.clear();
}

bool CSipCallId::Empty()
{
	return m_strName.empty();
}

bool CSipCallId::IsEqual( CSipCallId * pclsCallId )
{
	if( pclsCallId == NULL ) return false;

	if( !strcmp( m_strName.c_str(), pclsCallId->m_strName.c_str() ) && !strcmp( m_strHost.c_str(), pclsCallId->m_strHost.c_str() ) ) return true;

	return false;
}

void CSipCallId::Make( const char * pszHost )
{
	char	szName[SIP_CALL_ID_NAME_MAX_SIZE];

	SipMakeCallIdName( szName, sizeof(szName) );

	m_strName = szName;

	if( pszHost ) m_strHost = pszHost;
}
