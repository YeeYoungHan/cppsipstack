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

#include "SipPlatformDefine.h"
#include "SdpAttributeCrypto.h"

CSdpAttributeCrypto::CSdpAttributeCrypto()
{
}

CSdpAttributeCrypto::~CSdpAttributeCrypto()
{
}

/**
 * @ingroup SdpParser
 * @brief SDP 의 crypto attribute 의 내용을 파싱한다.
 *				"1 AES_CM_128_HMAC_SHA1_80 inline:7s65riA38OlE4U5OepU5zgaoMvzOL19nSQqeursI"
 * @param pszText		crypto attribute 내용
 * @param iTextLen	crypto attribute 내용의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 실패하면 -1 를 리턴한다.
 */
int CSdpAttributeCrypto::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos, iStartPos = 0;
	char	cType = 0;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == ' ' )
		{
			switch( cType )
			{
			case 0:
				m_strTag.append( pszText + iStartPos, iPos - iStartPos );
				break;
			case 1:
				m_strCryptoSuite.append( pszText + iStartPos, iPos - iStartPos );
				break;
			}

			iStartPos = iPos + 1;
			++cType;
		}
		else if( cType == 2 )
		{
			if( pszText[iPos] == ':' )
			{
				iStartPos = iPos + 1;
			}
			else if( pszText[iPos] == '|' )
			{
				m_strKey.append( pszText + iStartPos, iPos - iStartPos );
				iStartPos = iPos + 1;
				m_strText.append( pszText + iStartPos );
			}
		}
	}

	if( cType == 2 && m_strKey.empty() )
	{
		m_strKey.append( pszText + iStartPos );
	}

	return iTextLen;
}

/**
 * @ingroup SdpParser
 * @brief SDP crypto attribute 내용 문자열을 생성한다.
 * @param pszText		SDP crypto attribute 내용 문자열 저장 변수
 * @param iTextSize SDP crypto attribute 내용 문자열 저장 변수 크기
 * @returns 성공하면 저장된 문자열 길이를 리턴하고 실패하면 -1 를 리턴한다.
 */
int CSdpAttributeCrypto::ToString( char * pszText, int iTextSize )
{
	if( Empty() ) return -1;

	int iLen = snprintf( pszText, iTextSize, "%s %s inline:%s", m_strTag.c_str(), m_strCryptoSuite.c_str(), m_strKey.c_str() );

	if( m_strText.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "|%s", m_strText.c_str() );
	}

	return iLen;
}

/**
 * @ingroup SdpParser
 * @brief 멤버 변수를 초기화 시킨다.
 */
void CSdpAttributeCrypto::Clear()
{
	m_strTag.clear();
	m_strCryptoSuite.clear();
	m_strKey.clear();
	m_strText.clear();
}

/**
 * @ingroup SdpParser
 * @brief 사용할 수 있는 crypto 정보인지 검사한다.
 * @returns 사용할 수 있는 crypto 정보가 저장되어 있으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSdpAttributeCrypto::Empty()
{
	if( m_strTag.empty() || m_strCryptoSuite.empty() || m_strKey.empty() ) return true;

	return false;
}
