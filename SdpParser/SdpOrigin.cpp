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
#include "SdpOrigin.h"

/**
 * @ingroup SdpParser
 * @brief 생성자
 */
CSdpOrigin::CSdpOrigin()
{
}

/**
 * @ingroup SdpParser
 * @brief 소멸자
 */
CSdpOrigin::~CSdpOrigin()
{
}

/**
 * @ingroup SdpParser
 * @brief SDP origin 의 value 문자열을 파싱한다.
 * @param pszText		SDP origin 의 value 문자열
 * @param iTextLen	SDP origin 의 value 문자열 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1를 리턴한다.
 */
int CSdpOrigin::Parse( const char * pszText, int iTextLen )
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
			iStartPos = iPos + 1;
		}
	}

	if( iStartPos < iTextLen )
	{
		SetData( pszText + iStartPos, iPos - iStartPos, iType );
		++iType;
	}

	if( iType != 6 ) return -1;

	return iTextLen;
}

/**
 * @ingroup SdpParser
 * @brief SDP origin 의 value 문자열을 저장한다.
 * @param pszText		SDP origin 의 value 문자열을 저장할 변수
 * @param iTextSize pszText 변수의 크기
 * @returns 성공하면 저장된 문자열 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSdpOrigin::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;
	if( Empty() ) return -1;

	int iLen;

	iLen = snprintf( pszText, iTextSize, "%s %s %s %s %s %s", m_strUserName.c_str(), m_strSessId.c_str(), m_strSessVersion.c_str()
		, m_strNetType.c_str(), m_strAddrType.c_str(), m_strUnicastAddress.c_str() );

	return iLen;
}

/**
 * @ingroup SdpParser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CSdpOrigin::Clear()
{
	m_strUserName.clear();
	m_strSessId.clear();
	m_strSessVersion.clear();
	m_strNetType.clear();
	m_strAddrType.clear();
	m_strUnicastAddress.clear();
}

/**
 * @ingroup SdpParser
 * @brief 데이터가 존재하면 false 를 리턴하고 그렇지 않으면 true 를 리턴한다.
 * @return 데이터가 존재하면 false 를 리턴하고 그렇지 않으면 true 를 리턴한다.
 */
bool CSdpOrigin::Empty()
{
	if( m_strUserName.empty() || m_strSessId.empty() || m_strSessVersion.empty() ) return true;
	if( m_strNetType.empty() || m_strAddrType.empty() || m_strUnicastAddress.empty() ) return true;

	return false;
}

void CSdpOrigin::SetData( const char * pszData, int iLen, int iType )
{
	switch( iType )
	{
	case 0:
		m_strUserName.append( pszData, iLen );
		break;
	case 1:
		m_strSessId.append( pszData, iLen );
		break;
	case 2:
		m_strSessVersion.append( pszData, iLen );
		break;
	case 3:
		m_strNetType.append( pszData, iLen );
		break;
	case 4:
		m_strAddrType.append( pszData, iLen );
		break;
	case 5:
		m_strUnicastAddress.append( pszData, iLen );
		break;
	}
}
