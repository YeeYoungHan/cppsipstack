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
#include "SipCSeq.h"

CSipCSeq::CSipCSeq() : m_iDigit(-1)
{
}

CSipCSeq::~CSipCSeq()
{
}

/**
 * @ingroup SipParser
 * @brief SIP 헤더 문자열을 파싱하여 CSipCSeq 클래스의 멤버 변수에 저장한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipCSeq::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int iPos;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( m_iDigit == -1 )
		{
			if( pszText[iPos] == ' ' )
			{
				std::string	strDigit;

				strDigit.append( pszText, iPos );
				m_iDigit = atoi( strDigit.c_str() );
			}
		}
		else if( pszText[iPos] == ' ' || pszText[iPos] == '\t' )
		{
			continue;
		}
		else
		{
			m_strMethod.append( pszText + iPos );
			break;
		}
	}

	if( m_iDigit == -1 ) return -1;

	return iTextLen;
}

/**
 * @ingroup SipParser
 * @brief SIP 메시지에 포함된 문자열을 작성한다.
 * @param pszText		SIP 헤더의 값을 저장할 문자열 변수
 * @param iTextSize	pszText 변수의 크기
 * @returns 성공하면 작성한 문자열 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipCSeq::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;
	if( m_iDigit == -1 || m_strMethod.empty() ) return 0;

	return snprintf( pszText, iTextSize, "%d %s", m_iDigit, m_strMethod.c_str() );
}

/**
 * @ingroup SipParser
 * @brief SIP CSeq 헤더 정보를 저장한다.
 * @param iDigit		CSeq 숫자
 * @param pszMethod SIP 메소드
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipCSeq::Set( int iDigit, const char * pszMethod )
{
	if( iDigit < 0 || pszMethod == NULL ) return false;

	m_iDigit = iDigit;
	m_strMethod = pszMethod;

	return true;
}

/**
 * @ingroup SipParser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CSipCSeq::Clear()
{
	m_iDigit = -1;
	m_strMethod.clear();
}

/**
 * @ingroup SipParser
 * @brief 멤버변수가 저장되어 있지 않으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 * @returns 멤버변수가 저장되어 있지 않으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipCSeq::Empty()
{
	if( m_iDigit == -1 ) return true;
	return m_strMethod.empty();
}
