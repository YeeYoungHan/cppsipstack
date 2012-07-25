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
#include "SdpMedia.h"

CSdpMedia::CSdpMedia() : m_iPort(-1), m_iNumOfPort(-1)
{
}

CSdpMedia::~CSdpMedia()
{
}

int CSdpMedia::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos, iStartPos = 0, iType = 0;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == ' ' )
		{
			SetData( pszText + iStartPos, iPos - iStartPos, iType );
			++iType;
		}
	}

	if( iStartPos < iTextLen )
	{
		SetData( pszText + iStartPos, iPos - iStartPos, iType );
		++iType;
	}

	if( iType < 3 ) return -1;

	return iTextLen;
}

int CSdpMedia::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	int iLen, n;
	SDP_FMT_LIST::iterator	itFmt;

	iLen = snprintf( pszText, iTextSize, "%s ", m_strMedia.c_str() );

	if( m_iNumOfPort == -1 )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "%d ", m_iPort );
	}
	else
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "%d/%d ", m_iPort, m_iNumOfPort );
	}

	iLen += snprintf( pszText + iLen, iTextSize - iLen, "%s", m_strProtocol.c_str() );

	for( itFmt = m_clsFmtList.begin(); itFmt != m_clsFmtList.end(); ++itFmt )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, " %s", itFmt->c_str() );
	}

	iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );

	if( m_strTitle.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "i=%s\r\n", m_strTitle.c_str() );
	}

	if( m_clsConnection.Empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "c=" );
		n = m_clsConnection.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_clsBandWidthList.size() > 0 )
	{
		SDP_BANDWIDTH_LIST::iterator	itBL;

		for( itBL = m_clsBandWidthList.begin(); itBL != m_clsBandWidthList.end(); ++itBL )
		{
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "b=" );
			n = itBL->ToString( pszText + iLen, iTextSize - iLen );
			if( n == -1 ) return -1;
			iLen += n;
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
		}
	}

	if( m_clsAttributeList.size() > 0 )
	{
		SDP_ATTRIBUTE_LIST::iterator	itAL;

		for( itAL = m_clsAttributeList.begin(); itAL != m_clsAttributeList.end(); ++itAL )
		{
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "a=" );
			n = itAL->ToString( pszText + iLen, iTextSize - iLen );
			if( n == -1 ) return -1;
			iLen += n;
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
		}
	}

	return iLen;
}

void CSdpMedia::Clear()
{
	m_strMedia.clear();
	m_iPort = -1;
	m_iNumOfPort = -1;
	m_strProtocol.clear();
	m_clsFmtList.clear();
	m_strTitle.clear();
	m_clsConnection.Clear();
	m_clsBandWidthList.clear();
	m_clsAttributeList.clear();
}

void CSdpMedia::SetData( const char * pszData, int iLen, int iType )
{
	switch( iType )
	{
	case 0:
		m_strMedia.append( pszData, iLen );
		break;
	case 1:
		for( int i = 0; i < iLen; ++i )
		{
			if( pszData[i] == '/' )
			{
				std::string	strTemp;

				strTemp.append( pszData, i );
				m_iPort = atoi( strTemp.c_str() );

				strTemp.clear();
				strTemp.append( pszData + i + 1, iLen - ( i + 1 ) );
				break;
			}
		}

		if( m_iPort == -1 )
		{
			std::string	strTemp;

			strTemp.append( pszData, iLen );
			m_iPort = atoi( strTemp.c_str() );
		}
		break;
	case 2:
		m_strProtocol.append( pszData, iLen );
		break;
	default:
		{
			std::string strTemp;

			strTemp.append( pszData, iLen );
			m_clsFmtList.push_back( strTemp );
		}
		break;
	}
}
