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

#include "HttpUri.h"
#include <stdlib.h>
#include <string.h>

CHttpUri::CHttpUri() : m_iPort(80)
{
}

CHttpUri::~CHttpUri()
{
}

/**
 * @ingroup HttpParser
 * @brief HTTP URI 를 파싱한다.
 * @param pszText		HTTP URI 문자열
 * @param iTextLen	HTTP URI 문자열 길이
 * @returns 성공하면 파싱한 문자열 개수를 리턴하고 실패하면 -1 을 리턴한다.
 */
int CHttpUri::Parse( const char * pszText, int iTextLen )
{
	char cType = 0;
	int iStartPos = 0;

	Clear();

	for( int i = 0; i < iTextLen; ++i )
	{
		if( pszText[i] == ':' )
		{
			if( cType == 0 )
			{
				if( (i+2) < iTextLen && pszText[i+1] == '/' && pszText[i+2] == '/' )
				{
					m_strProtocol.append( pszText, i );
					++cType;
					i += 2;
					iStartPos = i + 1;

					if( !strcmp( m_strProtocol.c_str(), "https" ) )
					{
						m_iPort = 443;
					}
				}
				else
				{
					return -1;
				}
			}
			else if( cType == 1 )
			{
				m_strHost.append( pszText + iStartPos, i - iStartPos );
				++cType;
				iStartPos = i + 1;
			}
		}
		else if( pszText[i] == '/' )
		{
			if( cType == 1 )
			{
				m_strHost.append( pszText + iStartPos, i - iStartPos );
				m_strPath.append( pszText + i );
				return i;
			}
			else if( cType == 2 )
			{
				std::string strTemp;

				strTemp.append( pszText + iStartPos, i - iStartPos );
				m_iPort = atoi( strTemp.c_str() );
				m_strPath.append( pszText + i );
				return i;
			}
		}
	}

	if( cType == 1 )
	{
		m_strHost.append( pszText + iStartPos );
		return iTextLen;
	}

	return -1;
}

/**
 * @ingroup HttpParser
 * @brief 내부 변수를 초기화시킨다.
 */
void CHttpUri::Clear()
{
	m_strProtocol.clear();
	m_strHost.clear();
	m_iPort = 80;
	m_strPath.clear();
}
