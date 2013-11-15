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
#include "SipReason.h"
#include "StringUtility.h"

CSipReason::CSipReason()
{
}

CSipReason::~CSipReason()
{
}

/**
 * @ingroup SipParser
 * @brief SIP Reason 헤더를 파싱하여서 멤버 변수에 저장한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipReason::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos, iCurPos;
	bool	bParam = false;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == ';' )
		{
			m_strProtocol.append( pszText, iPos );
			bParam = true;
			break;
		}
		else if( pszText[iPos] == ',' )
		{
			m_strProtocol.append( pszText, iPos );
			break;
		}
	}

	iCurPos = iPos;

	if( m_strProtocol.empty() )
	{
		m_strProtocol.append( pszText, iCurPos );
	}

	if( bParam )
	{
		int iRet = HeaderListParamParse( pszText + iCurPos, iTextLen - iCurPos );
		if( iRet == -1 ) return -1;
		iCurPos += iRet;

		SIP_PARAMETER_LIST::iterator	itList;

		for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
		{
			if( itList->m_strValue.empty() == false && itList->m_strValue.at(0) == '"' )
			{
				ReplaceString( itList->m_strValue, "\"", "" );
			}
		}
	}

	return iCurPos;
}

/**
 * @ingroup SipParser
 * @brief SIP 메시지에 포함된 문자열을 작성한다.
 * @param pszText		SIP 헤더의 값을 저장할 문자열 변수
 * @param iTextSize	pszText 변수의 크기
 * @returns 성공하면 작성한 문자열 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipReason::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	int iLen, iPos;

	iLen = snprintf( pszText, iTextSize, "%s", m_strProtocol.c_str() );

	iPos = ParamToString( pszText + iLen, iTextSize - iLen );
	if( iPos == -1 ) return -1;
	iLen += iPos;

	return iLen;
}

/**
 * @ingroup SipParser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CSipReason::Clear()
{
	m_strProtocol.clear();
	ClearParam();
}
