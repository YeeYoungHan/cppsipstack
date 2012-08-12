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

#ifndef ANDROID
#include "SipParserDefine.h"
#else
#include "../SipParser/SipParserDefine.h"
#endif
#include "SdpMessage.h"

CSdpMessage::CSdpMessage()
{
}

CSdpMessage::~CSdpMessage()
{
}

int CSdpMessage::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos, iStartPos = 0;
	CSdpMedia * pclsMedia = NULL;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == '\r' )
		{
			switch( pszText[iStartPos] )
			{
			case 'v':
				m_strVersion.append( pszText + iStartPos + 2, iPos - ( iStartPos + 2 ) );
				break;
			case 'o':
				if( m_clsOrigin.Parse( pszText + iStartPos + 2, iPos - ( iStartPos + 2 ) ) == -1 ) return -1;
				break;
			case 's':
				m_strSessionName.append( pszText + iStartPos + 2, iPos - ( iStartPos + 2 ) );
				break;
			case 'i':
				if( pclsMedia )
				{

				}
				else
				{
					m_strSessionInformation.append( pszText + iStartPos + 2, iPos - ( iStartPos + 2 ) );
				}
				break;
			case 'u':
				m_strUri.append( pszText + iStartPos + 2, iPos - ( iStartPos + 2 ) );
				break;
			case 'e':
				{
					std::string	strTemp;

					strTemp.append( pszText + iStartPos + 2, iPos - ( iStartPos + 2 ) );
					m_clsEmailList.push_back( strTemp );
				}
				break;
			case 'p':
				{
					std::string	strTemp;

					strTemp.append( pszText + iStartPos + 2, iPos - ( iStartPos + 2 ) );
					m_clsPhoneList.push_back( strTemp );
				}
				break;
			case 'c':
				if( pclsMedia )
				{
					if( pclsMedia->m_clsConnection.Parse( pszText + iStartPos + 2, iPos - ( iStartPos + 2 ) ) == -1 ) return -1;
				}
				else
				{
					if( m_clsConnection.Parse( pszText + iStartPos + 2, iPos - ( iStartPos + 2 ) ) == -1 ) return -1;
				}
				break;
			case 'b':
				{
					CSdpBandWidth clsBandWidth;

					if( clsBandWidth.Parse( pszText + iStartPos + 2, iPos - ( iStartPos + 2 ) ) == -1 ) return -1;

					if( pclsMedia )
					{
						pclsMedia->m_clsBandWidthList.push_back( clsBandWidth );
					}
					else
					{
						m_clsBandWidthList.push_back( clsBandWidth );
					}
				}
				break;
			case 't':
				{
					CSdpTime	clsTime;

					if( clsTime.Parse( pszText + iStartPos + 2, iPos - ( iStartPos + 2 ) ) == -1 ) return -1;
					m_clsTimeList.push_back( clsTime );
				}
				break;
			case 'r':
				{
					SDP_TIME_LIST::reverse_iterator itTime = m_clsTimeList.rbegin();
					if( itTime != m_clsTimeList.rend() )
					{
						std::string	strTemp;

						strTemp.append( pszText + iStartPos + 2, iPos - ( iStartPos + 2 ) );
						itTime->m_clsRepeatTimeList.push_back( strTemp );
					}
				}
				break;
			case 'z':
				m_strTimeZone.append( pszText + iStartPos + 2, iPos - ( iStartPos + 2 ) );
				break;
			case 'a':
				{
					CSdpAttribute	clsAttribute;

					if( clsAttribute.Parse( pszText + iStartPos + 2, iPos - ( iStartPos + 2 ) ) == -1 ) return -1;

					if( pclsMedia )
					{
						pclsMedia->m_clsAttributeList.push_back( clsAttribute );
					}
					else
					{
						m_clsAttributeList.push_back( clsAttribute );
					}
				}
				break;
			case 'm':
				{
					CSdpMedia clsMedia;

					if( clsMedia.Parse( pszText + iStartPos + 2, iPos - ( iStartPos + 2 ) ) == -1 ) return -1;
					m_clsMediaList.push_back( clsMedia );

					SDP_MEDIA_LIST::reverse_iterator itML = m_clsMediaList.rbegin();
					pclsMedia = &(*itML);
				}
				break;
			}

			++iPos;
			iStartPos = iPos + 1;
		}
	}

	return iPos;
}

int CSdpMessage::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	int iLen = 0, n;

	if( m_strVersion.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "v=%s\r\n", m_strVersion.c_str() );
	}

	if( m_clsOrigin.Empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "o=" );
		n = m_clsOrigin.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_strSessionName.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "s=%s\r\n", m_strSessionName.c_str() );
	}

	if( m_strSessionInformation.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "i=%s\r\n", m_strSessionInformation.c_str() );
	}

	if( m_strUri.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "u=%s\r\n", m_strUri.c_str() );
	}

	if( m_clsEmailList.size() > 0 )
	{
		SDP_EMAIL_LIST::iterator	itList;

		for( itList = m_clsEmailList.begin(); itList != m_clsEmailList.end(); ++itList )
		{
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "e=%s\r\n", itList->c_str() );
		}
	}

	if( m_clsPhoneList.size() > 0 )
	{
		SDP_PHONE_LIST::iterator	itList;

		for( itList = m_clsPhoneList.begin(); itList != m_clsPhoneList.end(); ++itList )
		{
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "p=%s\r\n", itList->c_str() );
		}
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
		SDP_BANDWIDTH_LIST::iterator	itList;

		for( itList = m_clsBandWidthList.begin(); itList != m_clsBandWidthList.end(); ++itList )
		{
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "b=" );
			n = itList->ToString( pszText + iLen, iTextSize - iLen );
			if( n == -1 ) return -1;
			iLen += n;
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
		}
	}

	if( m_clsTimeList.size() > 0 )
	{
		SDP_TIME_LIST::iterator	itList;

		for( itList = m_clsTimeList.begin(); itList != m_clsTimeList.end(); ++itList )
		{
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "t=" );
			n = itList->ToString( pszText + iLen, iTextSize - iLen );
			if( n == -1 ) return -1;
			iLen += n;
		}
	}

	if( m_strTimeZone.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "z=%s\r\n", m_strTimeZone.c_str() );
	}

	if( m_clsAttributeList.size() > 0 )
	{
		SDP_ATTRIBUTE_LIST::iterator	itList;

		for( itList = m_clsAttributeList.begin(); itList != m_clsAttributeList.end(); ++itList )
		{
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "a=" );
			n = itList->ToString( pszText + iLen, iTextSize - iLen );
			if( n == -1 ) return -1;
			iLen += n;
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
		}
	}

	if( m_clsMediaList.size() > 0 )
	{
		SDP_MEDIA_LIST::iterator	itList;

		for( itList = m_clsMediaList.begin(); itList != m_clsMediaList.end(); ++itList )
		{
			iLen += snprintf( pszText + iLen, iTextSize - iLen, "m=" );
			n = itList->ToString( pszText + iLen, iTextSize - iLen );
			if( n == -1 ) return -1;
			iLen += n;
		}
	}

	return iLen;
}

void CSdpMessage::Clear()
{
	m_strVersion.clear();
	m_strSessionName.clear();
	m_strSessionInformation.clear();
	m_strUri.clear();
	m_strTimeZone.clear();

	m_clsOrigin.Clear();
	m_clsConnection.Clear();

	m_clsEmailList.clear();
	m_clsPhoneList.clear();

	m_clsBandWidthList.clear();
	m_clsTimeList.clear();
	m_clsAttributeList.clear();
	m_clsMediaList.clear();
}
