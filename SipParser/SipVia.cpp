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
#include "SipVia.h"

CSipVia::CSipVia(void) : m_iPort(-1)
{
}

CSipVia::~CSipVia(void)
{
}

int CSipVia::Parse( const char * pszText, int iTextLen )
{
	if( pszText == NULL || iTextLen <= 0 ) return -1;
	Clear();

	int iPos, iCurPos = 0;

	iPos = ParseSentProtocol( pszText, iTextLen );
	if( iPos == -1 ) return -1;
	iCurPos += iPos;

	iPos = ParseSentBy( pszText + iCurPos, iTextLen - iCurPos );
	if( iPos == -1 ) return -1;
	iCurPos += iPos;

	while( iCurPos < iTextLen )
	{
		if( pszText[iCurPos] == ';' || pszText[iCurPos] == ' ' || pszText[iCurPos] == '\t' )
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

int CSipVia::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;
	if( m_strProtocolName.empty() || m_strProtocolVersion.empty() || m_strTransport.empty() || m_strHost.empty() ) return 0;

	int iLen = 0, iPos;

	iLen += snprintf( pszText, iTextSize, "%s/%s/%s %s", m_strProtocolName.c_str(), m_strProtocolVersion.c_str(), m_strTransport.c_str(), m_strHost.c_str() );

	if( m_iPort > 0 )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, ":%d", m_iPort );
	}

	iPos = MakeSipParameterString( m_clsParamList, pszText + iLen, iTextSize - iLen );
	if( iPos == -1 ) return -1;
	iLen += iPos;

	return iLen;
}

void CSipVia::Clear()
{
	m_strProtocolName.clear();
	m_strProtocolVersion.clear();
	m_strTransport.clear();
	m_strHost.clear();
	m_iPort = -1;

	m_clsParamList.clear();
}

const char * CSipVia::GetParamValue( const char * pszName )
{
	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
	{
		if( !strcmp( itList->m_strName.c_str(), pszName ) )
		{
			return itList->m_strValue.c_str();
		}
	}

	return NULL;
}

int CSipVia::ParseSentProtocol( const char * pszText, int iTextLen )
{
	int		iPos, iPrevPos = -1;
	char	cType = 0;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == '/' )
		{
			switch( cType )
			{
			case 0:
				m_strProtocolName.append( pszText, iPos );
				iPrevPos = iPos + 1;
				break;
			case 1:
				m_strProtocolVersion.append( pszText + iPrevPos, iPos - iPrevPos );
				iPrevPos = iPos + 1;
				break;
			default:
				return -1;
			}
			++cType;
		}
		else if( pszText[iPos] == ' ' )
		{
			if( cType == 2 )
			{
				m_strTransport.append( pszText + iPrevPos, iPos - iPrevPos );
				return iPos + 1;
			}
			else
			{
				return -1;
			}
		}
	}
	
	return -1;
}

int CSipVia::ParseSentBy( const char * pszText, int iTextLen )
{
	int		iPos, iPortPos = -1;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == ':' )
		{
			m_strHost.append( pszText, iPos );
			iPortPos = iPos + 1;
		}
		else if( pszText[iPos] == ' ' || pszText[iPos] == ';' || pszText[iPos] == ',' )
		{
			break;
		}
	}

	if( m_strHost.empty() )
	{
		m_strHost.append( pszText, iPos );
	}
	else
	{
		std::string	strPort;

		strPort.append( pszText + iPortPos, iPos - iPortPos );
		m_iPort = atoi( strPort.c_str() );
	}

	return iPos;
}
