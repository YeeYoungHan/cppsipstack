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
#include "SdpMedia.h"
#include <stdlib.h>

/**
 * @ingroup SdpParser
 * @brief 생성자
 */
CSdpMedia::CSdpMedia() : m_iPort(-1), m_iNumOfPort(-1)
{
}

/**
 * @ingroup SdpParser
 * @brief 생성자
 * @param pszMedia		미디어 이름 
 * @param iPort				포트 번호
 * @param pszProtocol 전송 프로토콜
 */
CSdpMedia::CSdpMedia( const char * pszMedia, int iPort, const char * pszProtocol )
{
	m_strMedia = pszMedia;
	m_iPort = iPort;
	m_strProtocol = pszProtocol;
}

/**
 * @ingroup SdpParser
 * @brief 소멸자
 */
CSdpMedia::~CSdpMedia()
{
}

/**
 * @ingroup SdpParser
 * @brief SDP media 의 value 문자열을 파싱한다.
 * @param pszText		SDP media 의 value 문자열
 * @param iTextLen	SDP media 의 value 문자열 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1를 리턴한다.
 */
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
			iStartPos = iPos + 1;
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

/**
 * @ingroup SdpParser
 * @brief SDP media 의 value 문자열을 저장한다.
 * @param pszText		SDP media 의 value 문자열을 저장할 변수
 * @param iTextSize pszText 변수의 크기
 * @returns 성공하면 저장된 문자열 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
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

/**
 * @ingroup SdpParser
 * @brief 멤버 변수를 초기화시킨다.
 */
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

/**
 * @ingroup SdpParser
 * @brief FMT 를 추가한다.
 * @param iPayLoadType payload type
 */
void CSdpMedia::AddFmt( int iPayLoadType )
{
	if( SelectFmt( iPayLoadType ) ) return;

	char	szNum[11];

	snprintf( szNum, sizeof(szNum), "%d", iPayLoadType );

	m_clsFmtList.push_back( szNum );
}

/**
 * @ingroup SdpParser
 * @brief FMT 리스트에 payload type 이 존재하는지 확인한다. 
 * @param iPayLoadType payload type
 * @returns FMT 리스트에 payload type 이 존재하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSdpMedia::SelectFmt( int iPayLoadType )
{
	char	szNum[11];
	SDP_FMT_LIST::iterator	itList;

	snprintf( szNum, sizeof(szNum), "%d", iPayLoadType );

	for( itList = m_clsFmtList.begin(); itList != m_clsFmtList.end(); ++itList )
	{
		if( !strcmp( itList->c_str(), szNum ) )
		{
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SdpParser
 * @brief 애트리뷰트를 삭제한다.
 * @param pszName 삭제할 애트리뷰트 이름
 * @returns 애트리뷰트를 삭제하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSdpMedia::DeleteAttribute( const char * pszName )
{
	SDP_ATTRIBUTE_LIST::iterator	itAttr, itNext;
	bool bRes = false;

	for( itAttr = m_clsAttributeList.begin(); itAttr != m_clsAttributeList.end(); ++itAttr )
	{
LOOP_START:
		if( !strcmp( itAttr->m_strName.c_str(), pszName ) )
		{
			bRes = true;

			itNext = itAttr;
			++itNext;

			m_clsAttributeList.erase( itAttr );

			if( itNext == m_clsAttributeList.end() ) break;
			itAttr = itNext;
			goto LOOP_START;
		}
	}

	return bRes;
}

/**
 * @ingroup SdpParser
 * @brief 애트리뷰트를 추가한다. rtpmap 이면 fmt 로 추가한다.
 * @param pclsAttr 애트리뷰트 객체
 */
void CSdpMedia::AddAttribute( CSdpAttribute * pclsAttr )
{
	if( !strcmp( pclsAttr->m_strName.c_str(), "rtpmap" ) )
	{
		AddFmt( pclsAttr->GetPayLoadType() );
	}

	m_clsAttributeList.push_back( *pclsAttr );
}

/**
 * @ingroup SdpParser
 * @brief 애트리뷰트를 추가한다. rtpmap 이면 fmt 로 추가한다.
 * @param pszName		애트리뷰트 이름
 * @param pszValue	애트리뷰트 값
 */
void CSdpMedia::AddAttribute( const char * pszName, const char * pszValue )
{
	CSdpAttribute clsAttr( pszName, pszValue );

	if( !strcmp( clsAttr.m_strName.c_str(), "rtpmap" ) )
	{
		AddFmt( clsAttr.GetPayLoadType() );
	}

	m_clsAttributeList.push_back( clsAttr );
}

/**
 * @ingroup SdpParser
 * @brief SDP media 문자열을 파싱한 각 항목을 저장한다.
 * @param pszData 문자열
 * @param iLen		문자열 길이
 * @param iType		SDP media 문자열을 파싱한 문자열 배열의 인덱스
 */
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
