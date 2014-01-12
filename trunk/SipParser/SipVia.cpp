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
#include <stdlib.h>
#include "MemoryDebug.h"

CSipVia::CSipVia() : m_iPort(-1)
{
}

CSipVia::~CSipVia()
{
}

/**
 * @ingroup SipParser
 * @brief SIP 헤더 문자열을 파싱하여 CSipVia 클래스의 멤버 변수에 저장한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
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

	iPos = HeaderListParamParse( pszText + iCurPos, iTextLen - iCurPos );
	if( iPos == -1 ) return -1;
	iCurPos += iPos;

	return iCurPos;
}

/**
 * @ingroup SipParser
 * @brief SIP 메시지에 포함된 문자열을 작성한다.
 * @param pszText		SIP 헤더의 값을 저장할 문자열 변수
 * @param iTextSize	pszText 변수의 크기
 * @returns 성공하면 작성한 문자열 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
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

	iPos = ParamToString( pszText + iLen, iTextSize - iLen );
	if( iPos == -1 ) return -1;
	iLen += iPos;

	return iLen;
}

/**
 * @ingroup SipParser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CSipVia::Clear()
{
	m_strProtocolName.clear();
	m_strProtocolVersion.clear();
	m_strTransport.clear();
	m_strHost.clear();
	m_iPort = -1;

	ClearParam();
}

/**
 * @brief 문자열에서 프로토콜 및 프로토콜 버전을 파싱한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
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

/**
 * @brief 전송 호스트 정보를 파싱한다.
 * @param pszText		전송 호스트 정보 문자열
 * @param iTextLen	전송 호스트 정보 문자열 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
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
