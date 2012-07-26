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
#include "SdpConnection.h"

CSdpConnection::CSdpConnection() : m_MulticastTtl(-1), m_MulticastNum(-1)
{
}

CSdpConnection::~CSdpConnection()
{
}

int CSdpConnection::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos, iStartPos = 0, iType = 0;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == ' ' || pszText[iPos] == '/' )
		{
			SetData( pszText + iStartPos, iPos - iStartPos, iType );
			++iType;
			iStartPos = iPos + 1;
		}
	}

	if( iStartPos < iTextLen )
	{
		SetData( pszText + iStartPos, iPos - iStartPos, iType );
	}

	return iTextLen;
}

int CSdpConnection::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;
	if( Empty() ) return -1;

	int iLen;

	iLen = snprintf( pszText, iTextSize, "%s %s %s", m_strNetType.c_str(), m_strAddrType.c_str(), m_strAddr.c_str() );

	if( m_MulticastTtl >= 0 )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "/%d", m_MulticastTtl );

		if( m_MulticastNum >= 0 )
		{
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "/%d", m_MulticastNum );
		}
	}

	return iLen;
}

void CSdpConnection::Clear()
{
	m_strNetType.clear();
	m_strAddrType.clear();
	m_strAddr.clear();
	m_MulticastTtl = -1;
	m_MulticastNum = -1;
}

bool CSdpConnection::Empty()
{
	if( m_strNetType.empty() || m_strAddrType.empty() || m_strAddr.empty() ) return true;

	return false;
}

void CSdpConnection::SetData( const char * pszData, int iLen, int iType )
{
	switch( iType )
	{
	case 0:
		m_strNetType.append( pszData, iLen );
		break;
	case 1:
		m_strAddrType.append( pszData, iLen );
		break;
	case 2:
		m_strAddr.append( pszData, iLen );
		break;
	case 3:
		{
			std::string	strTemp;

			strTemp.append( pszData, iLen );
			m_MulticastTtl = atoi( strTemp.c_str() );
		}
		break;
	case 4:
		{
			std::string	strTemp;

			strTemp.append( pszData, iLen );
			m_MulticastNum = atoi( strTemp.c_str() );
		}
		break;
	}
}
