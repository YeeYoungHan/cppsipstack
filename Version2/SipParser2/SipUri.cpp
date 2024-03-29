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
#include "SipUri.h"
#include <stdlib.h>
#include "SipUtility.h"
#include "MemoryDebug.h"

CSipUri::CSipUri() : m_iPort(0)
{
}

CSipUri::~CSipUri()
{
}

/**
 * @ingroup SipParser
 * @brief Uri 문자열을 파싱하여 CSipUri 클래스의 멤버 변수에 저장한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipUri::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int iPos, iCurPos;

	iPos = ParseProtocol( pszText, iTextLen );
	if( iPos == -1 ) return -1;
	iCurPos = iPos;

	iPos = ParseUser( pszText + iCurPos, iTextLen - iCurPos );
	if( iPos > 0 ) iCurPos += iPos;

	iPos = ParseHost( pszText + iCurPos, iTextLen - iCurPos );
	if( iPos == -1 ) return -1;
	iCurPos += iPos;

	while( iCurPos < iTextLen )
	{
		if( pszText[iCurPos] == '?' )
		{
			++iCurPos;
			break;
		}

		if( pszText[iCurPos] == ';' || pszText[iCurPos] == ' ' || pszText[iCurPos] == '\t' )
		{
			++iCurPos;
			continue;
		}

		iPos = m_clsUriParamList.ParamParse( pszText + iCurPos, iTextLen - iCurPos );
		if( iPos == -1 ) return -1;
		iCurPos += iPos;
	}

	while( iCurPos < iTextLen )
	{
		if( pszText[iCurPos] == ' ' || pszText[iCurPos] == '\t' || pszText[iCurPos] == '&'  )
		{
			++iCurPos;
			continue;
		}

		iPos = m_clsHeaderList.ParamParse( pszText + iCurPos, iTextLen - iCurPos );
		if( iPos == -1 ) return -1;
		iCurPos += iPos;
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
int CSipUri::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;
	if( m_strProtocol.empty() || m_strHost.empty() ) return 0;

	int iLen = 0, iPos;

	iLen += snprintf( pszText + iLen, iTextSize - iLen, "%s:", m_strProtocol.c_str() );

	if( m_strUser.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "%s@", m_strUser.c_str() );
	}

	iLen += SipIpv6Print( m_strHost, pszText, iTextSize, iLen );

	if( m_iPort > 0 )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, ":%d", m_iPort );
	}

	iPos = m_clsUriParamList.ParamToString( pszText + iLen, iTextSize - iLen );
	if( iPos == -1 ) return -1;
	iLen += iPos;

	if( m_clsHeaderList.m_clsParamList.empty() == false )
	{
		iPos = m_clsHeaderList.ParamToString( pszText + iLen, iTextSize - iLen, '?', '&' );
		if( iPos == -1 ) return -1;
		iLen += iPos;
	}

	return iLen;
}

/**
 * @ingroup SipParser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CSipUri::Clear()
{
	m_strProtocol.clear();
	m_strUser.clear();
	m_strHost.clear();
	m_iPort = 0;
	m_clsUriParamList.ClearParam();
	m_clsHeaderList.ClearParam();
}

/**
 * @ingroup SipParser
 * @brief 멤버변수가 저장되어 있지 않으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 * @returns 멤버변수가 저장되어 있지 않으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipUri::Empty()
{
	if( m_strProtocol.empty() || m_strHost.empty() ) return true;

	return false;
}

/**
 * @ingroup SipParser
 * @brief uri param 을 추가한다.
 * @param pszName		param 이름
 * @param pszValue	param 값
 */
void CSipUri::InsertParam( const char * pszName, const char * pszValue )
{
	m_clsUriParamList.InsertParam( pszName, pszValue );
}

/**
 * @ingroup SipParser
 * @brief SIP URI 에 transport tag 를 추가한다.
 * @param eTransport 프로토콜
 */
void CSipUri::InsertTransport( ESipTransport eTransport )
{
	if( eTransport == E_SIP_TCP )
	{
		m_clsUriParamList.InsertParam( SIP_TRANSPORT, SIP_TRANSPORT_TCP );
	}
	else if( eTransport == E_SIP_TLS )
	{
		m_clsUriParamList.InsertParam( SIP_TRANSPORT, SIP_TRANSPORT_TLS );
	}
}

/**
 * @ingroup SipParser
 * @brief SIP URI 에서 전송 프로토콜을 가져온다.
 * @returns 전송 프로토콜을 리턴한다.
 */
ESipTransport CSipUri::SelectTransport( )
{
	std::string	strValue;

	if( !strcmp( m_strProtocol.c_str(), "sips" ) )
	{
		return E_SIP_TLS;
	}

	if( m_clsUriParamList.SelectParam( SIP_TRANSPORT, strValue ) )
	{
		const char * pszValue = strValue.c_str();

		if( !strcasecmp( pszValue, SIP_TRANSPORT_TCP ) )
		{
			return E_SIP_TCP;
		}
		else if( !strcasecmp( pszValue, SIP_TRANSPORT_TLS ) )
		{
			return E_SIP_TLS;
		}
	}

	return E_SIP_UDP;
}

/**
 * @ingroup SipParser
 * @brief 멤버 변수에 입력된 데이터를 저장한다.
 * @param pszProtocol 프로토콜
 * @param pszUser			사용자 아이디
 * @param pszHost			도메인 이름 또는 IP 주소
 * @param iPort				포트 번호
 */
void CSipUri::Set( const char * pszProtocol, const char * pszUser, const char * pszHost, int iPort )
{
	if( pszProtocol )
	{
		m_strProtocol = pszProtocol;
	}
	else
	{
		m_strProtocol = SIP_PROTOCOL;
	}

	if( pszUser )
	{
		m_strUser = pszUser;
	}

	if( pszHost )
	{
		m_strHost = pszHost;
	}

	if( iPort == 5060 )
	{
		m_iPort = 0;
	}
	else
	{
		m_iPort = iPort;
	}
}

/**
 * @ingroup SipParser
 * @brief SIP URI 의 프로토콜을 파싱한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipUri::ParseProtocol( const char * pszText, int iTextLen )
{
	for( int iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == ':' )
		{
			m_strProtocol.append( pszText, iPos );
			return iPos + 1;
		}
	}

	return -1;
}

/**
 * @ingroup SipParser
 * @brief SIP URI 의 사용자 아이디를 파싱한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipUri::ParseUser( const char * pszText, int iTextLen )
{
	for( int iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == '@' )
		{
			m_strUser.append( pszText, iPos );
			return iPos + 1;
		}
	}

	return -1;
}

/**
 * @ingroup SipParser
 * @brief SIP URI 의 호스트를 파싱한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipUri::ParseHost( const char * pszText, int iTextLen )
{
	int iPos, iPortPos = -1;

	if( pszText[0] == '[' )
	{
		bool bIpFound = false;

		for( iPos = 1; iPos < iTextLen; ++iPos )
		{
			if( bIpFound == false )
			{
				if( pszText[iPos] == ']' )
				{
					m_strHost.append( pszText + 1, iPos - 1 );
					bIpFound = true;
				}
			}
			else if( pszText[iPos] == ':' )
			{
				iPortPos = iPos + 1;
			}
			else if( pszText[iPos] == ';' || pszText[iPos] == '?' )
			{
				break;
			}
		}

		if( bIpFound == false ) return -1;
		if( iPortPos != -1 && iPortPos < iPos )
		{
			std::string strTemp;

			strTemp.append( pszText + iPortPos, iPos - iPortPos );
			m_iPort = atoi( strTemp.c_str() );
		}

		return iPos;
	}
	else
	{
		for( iPos = 0; iPos < iTextLen; ++iPos )
		{
			if( pszText[iPos] == ':' )
			{
				m_strHost.append( pszText, iPos );
				iPortPos = iPos + 1;
			}
			else if( pszText[iPos] == ';' || pszText[iPos] == '?' )
			{
				break;
			}
		}

		if( iPortPos == -1 )
		{
			if( iPos > 0 )
			{
				m_strHost.append( pszText, iPos );
				return iPos;
			}
		}
		else
		{
			if( iPortPos < iPos )
			{
				std::string strTemp;

				strTemp.append( pszText + iPortPos, iPos - iPortPos );
				m_iPort = atoi( strTemp.c_str() );
			}

			return iPos;
		}
	}

	return -1;
}
