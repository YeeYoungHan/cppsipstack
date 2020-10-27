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
#include "SipMessage.h"
#include "SipStatusCode.h"
#include "SipUtility.h"
#include <stdlib.h>
#include "MemoryDebug.h"

CSipMessage::CSipMessage() : m_iStatusCode(-1), m_iContentLength(0), m_iExpires(-1), m_iMaxForwards(-1), m_eTransport(E_SIP_UDP)
	, m_iClientPort(0)
	, m_bUseCompact(false), m_iUseCount(0)
{
}

CSipMessage::~CSipMessage()
{
}

/**
 * @brief SIP 헤더 문자열을 파싱하여 CSipMessage 클래스의 멤버 변수에 저장한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipMessage::Parse( const char * pszText, int iTextLen )
{
	if( pszText == NULL || iTextLen <= 4 ) return -1;

	int iPos, iCurPos, iNameLen, iValueLen;
	const char * pszName, * pszValue;
	CSipHeader	clsHeader;

#ifdef PARSE_FAST
	bool bNotFound;
#endif

	if( !strncmp( pszText, "SIP/", 4 ) )
	{
		iCurPos = ParseStatusLine( pszText, iTextLen );
	}
	else
	{
		iCurPos = ParseRequestLine( pszText, iTextLen );
	}

	if( iCurPos == -1 ) return -1;

	while( iCurPos < iTextLen )
	{
		iPos = clsHeader.Parse( pszText + iCurPos, iTextLen - iCurPos );
		if( iPos == -1 ) return -1;
		iCurPos += iPos;

		iNameLen = (int)clsHeader.m_strName.length();
		if( iNameLen == 0 ) break;

		pszName = clsHeader.m_strName.c_str();
		pszValue = clsHeader.m_strValue.c_str();
		iValueLen = (int)clsHeader.m_strValue.length();

#ifdef PARSE_FAST
		bNotFound = false;

		if( iNameLen == 1 )
		{
			if( pszName[0] == 'v' )
			{
				if( ParseSipVia( m_clsViaList, pszValue, iValueLen ) == -1 ) return -1;
			}
			else if( pszName[0] == 'f' )
			{
				if( m_clsFrom.Parse( pszValue, iValueLen ) == -1 ) return -1;
			}
			else if( pszName[0] == 't' )
			{
				if( m_clsTo.Parse( pszValue, iValueLen ) == -1 ) return -1;
			}
			else if( pszName[0] == 'i' )
			{
				if( m_clsCallId.Parse( pszValue, iValueLen ) == -1 ) return -1;
			}
			else if( pszName[0] == 'm' )
			{
				if( ParseSipFrom( m_clsContactList, pszValue, iValueLen ) == -1 ) return -1;
			}
			else if( pszName[0] == 'c' )
			{
				if( m_clsContentType.Parse( pszValue, iValueLen ) == -1 ) return -1;
			}
			else if( pszName[0] == 'l' )
			{
				m_iContentLength = atoi( pszValue );
			}
			else
			{
				bNotFound = true;
			}
		}
		else if( iNameLen == 2 )
		{
			if( !strcasecmp( pszName, "To" ) )
			{
				if( m_clsTo.Parse( pszValue, iValueLen ) == -1 ) return -1;
			}
			else
			{
				bNotFound = true;
			}
		}
		else if( iNameLen == 3 )
		{
			if( !strcasecmp( pszName, "Via" ) )
			{
				if( ParseSipVia( m_clsViaList, pszValue, iValueLen ) == -1 ) return -1;
			}
			else
			{
				bNotFound = true;
			}
		}
		else if( iNameLen == 4 )
		{
			if( !strcasecmp( pszName, "From" ) )
			{
				if( m_clsFrom.Parse( pszValue, iValueLen ) == -1 ) return -1;
			}
			else if( !strcasecmp( pszName, "CSeq" ) )
			{
				if( m_clsCSeq.Parse( pszValue, iValueLen ) == -1 ) return -1;
			}
			else
			{
				bNotFound = true;
			}
		}
		else if( iNameLen == 5 )
		{
			if( !strcasecmp( pszName, "Route" ) )
			{
				if( ParseSipFrom( m_clsRouteList, pszValue, iValueLen ) == -1 ) return -1;
			}
			else
			{
				bNotFound = true;
			}
		}
#ifdef USE_ACCEPT_HEADER
		else if( iNameLen == 6 )
		{
			if( !strcasecmp( pszName, "Accept" ) )
			{
				if( ParseSipContentType( m_clsAcceptList, pszValue, iValueLen ) == -1 ) return -1;
			}
			else
			{
				bNotFound = true;
			}
		}
#endif
		else if( iNameLen == 7 )
		{
			if( !strcasecmp( pszName, "Call-ID" ) )
			{
				if( m_clsCallId.Parse( pszValue, iValueLen ) == -1 ) return -1;
			}
			else if( !strcasecmp( pszName, "Contact" ) )
			{
				if( ParseSipFrom( m_clsContactList, pszValue, iValueLen ) == -1 ) return -1;
			}
			else if( !strcasecmp( pszName, "Expires" ) )
			{
				m_iExpires = atoi( pszValue );
			}
			else
			{
				bNotFound = true;
			}
		}
		else if( iNameLen == 10 )
		{
			if( !strcasecmp( pszName, "User-Agent" ) )
			{
				m_strUserAgent = clsHeader.m_strValue;
			}
			else
			{
				bNotFound = true;
			}
		}
		else if( iNameLen == 12 )
		{
			if( !strcasecmp( pszName, "Max-Forwards" ) )
			{
				m_iMaxForwards = atoi( pszValue );
			}
			else if( !strcasecmp( pszName, "Record-Route" ) )
			{
				if( ParseSipFrom( m_clsRecordRouteList, pszValue, iValueLen ) == -1 ) return -1;
			}
			else if( !strcasecmp( pszName, "Content-Type" ) )
			{
				if( m_clsContentType.Parse( pszValue, iValueLen ) == -1 ) return -1;
			}
			else
			{
				bNotFound = true;
			}
		}
		else if( iNameLen == 13 )
		{
			if( !strcasecmp( pszName, "Authorization" ) )
			{
				if( ParseSipCredential( m_clsAuthorizationList, pszValue, iValueLen ) == -1 ) return -1;
			}
			else
			{
				bNotFound = true;
			}
		}
		else if( iNameLen == 14 )
		{
			if( !strcasecmp( pszName, "Content-Length" ) )
			{
				m_iContentLength = atoi( pszValue );
			}
			else
			{
				bNotFound = true;
			}
		}
#ifdef USE_ACCEPT_HEADER
		else if( iNameLen == 15 )
		{
			else if( !strcasecmp( pszName, "Accept-Encoding" ) )
			{
				if( ParseSipAcceptData( m_clsAcceptEncodingList, pszValue, iValueLen ) == -1 ) return -1;
			}
			else if( !strcasecmp( pszName, "Accept-Language" ) )
			{
				if( ParseSipAcceptData( m_clsAcceptLanguageList, pszValue, iValueLen ) == -1 ) return -1;
			}
			else
			{
				bNotFound = true;
			}
		}
#endif
		else if( iNameLen == 16 )
		{
			if( !strcasecmp( pszName, "WWW-Authenticate" ) )
			{
				if( ParseSipChallenge( m_clsWwwAuthenticateList, pszValue, iValueLen ) == -1 ) return -1;
			}
			else
			{
				bNotFound = true;
			}
		}
		else if( iNameLen == 18 )
		{
			if( !strcasecmp( pszName, "Proxy-Authenticate" ) )
			{
				if( ParseSipChallenge( m_clsProxyAuthenticateList, pszValue, iValueLen ) == -1 ) return -1;
			}
			else
			{
				bNotFound = true;
			}
		}
		else if( iNameLen == 19 )
		{
			if( !strcasecmp( pszName, "Proxy-Authorization" ) )
			{
				if( ParseSipCredential( m_clsProxyAuthorizationList, pszValue, iValueLen ) == -1 ) return -1;
			}
			else
			{
				bNotFound = true;
			}
		}
		else
		{
			bNotFound = true;
		}

		if( bNotFound )
		{
			m_clsHeaderList.push_back( clsHeader );
		}
#else
		if( !strcasecmp( pszName, "Via" ) || !strcasecmp( pszName, "v" ) )
		{
			if( ParseSipVia( m_clsViaList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Max-Forwards" ) )
		{
			m_iMaxForwards = atoi( pszValue );
		}
		else if( !strcasecmp( pszName, "From" ) || !strcasecmp( pszName, "f" ) )
		{
			if( m_clsFrom.Parse( pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "To" ) || !strcasecmp( pszName, "t" ) )
		{
			if( m_clsTo.Parse( pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "CSeq" ) )
		{
			if( m_clsCSeq.Parse( pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Call-ID" ) || !strcasecmp( pszName, "i" ) )
		{
			if( m_clsCallId.Parse( pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Contact" ) || !strcasecmp( pszName, "m" ) )
		{
			if( ParseSipFrom( m_clsContactList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Record-Route" ) )
		{
			if( ParseSipFrom( m_clsRecordRouteList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Route" ) )
		{
			if( ParseSipFrom( m_clsRouteList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Authorization" ) )
		{
			if( ParseSipCredential( m_clsAuthorizationList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "WWW-Authenticate" ) )
		{
			if( ParseSipChallenge( m_clsWwwAuthenticateList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Proxy-Authorization" ) )
		{
			if( ParseSipCredential( m_clsProxyAuthorizationList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Proxy-Authenticate" ) )
		{
			if( ParseSipChallenge( m_clsProxyAuthenticateList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Content-Type" ) || !strcasecmp( pszName, "c" ) )
		{
			if( m_clsContentType.Parse( pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Content-Length" ) || !strcasecmp( pszName, "l" ) )
		{
			m_iContentLength = atoi( pszValue );
		}
#ifdef USE_ACCEPT_HEADER
		else if( !strcasecmp( pszName, "Accept-Encoding" ) )
		{
			if( ParseSipAcceptData( m_clsAcceptEncodingList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Accept-Language" ) )
		{
			if( ParseSipAcceptData( m_clsAcceptLanguageList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Accept" ) )
		{
			if( ParseSipContentType( m_clsAcceptList, pszValue, iValueLen ) == -1 ) return -1;
		}
#endif
		else if( !strcasecmp( pszName, "Expires" ) )
		{
			m_iExpires = atoi( pszValue );
		}
		else if( !strcasecmp( pszName, "User-Agent" ) )
		{
			m_strUserAgent = clsHeader.m_strValue;
		}
		else
		{
			m_clsHeaderList.push_back( clsHeader );
		}
#endif
	}

	if( m_iContentLength > 0 )
	{
		if( m_iContentLength > ( iTextLen - iCurPos ) ) return -1;

		m_strBody.append( pszText + iCurPos, m_iContentLength );
		return iTextLen;
	}

	return iCurPos;
}

/**
 * @brief SIP 메시지 문자열을 작성한다.
 * @param pszText		SIP 메시지 문자열을 저장할 문자열 변수
 * @param iTextSize	pszText 변수의 크기
 * @returns 성공하면 작성한 문자열 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipMessage::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	int iLen, n;

	if( m_strSipVersion.empty() ) m_strSipVersion = SIP_VERSION;

	if( m_iStatusCode > 0 )
	{
		if( m_strReasonPhrase.empty() )
		{
			m_strReasonPhrase = GetReasonPhrase( m_iStatusCode );
		}

		iLen = snprintf( pszText, iTextSize, "%s %d %s\r\n", m_strSipVersion.c_str(), m_iStatusCode, m_strReasonPhrase.c_str() );
	}
	else
	{
		if( m_strSipMethod.empty() || m_clsReqUri.Empty() || m_strSipVersion.empty() ) return -1;
		iLen = snprintf( pszText, iTextSize, "%s ", m_strSipMethod.c_str() );
		n = m_clsReqUri.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, " %s\r\n", m_strSipVersion.c_str() );
	}

	for( SIP_VIA_LIST::iterator itList = m_clsViaList.begin(); itList != m_clsViaList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, ( m_bUseCompact ? "v: " : "Via: " ) );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	for( SIP_FROM_LIST::iterator itList = m_clsRecordRouteList.begin(); itList != m_clsRecordRouteList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Record-Route: " );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	for( SIP_FROM_LIST::iterator itList = m_clsRouteList.begin(); itList != m_clsRouteList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Route: " );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_iMaxForwards >= 0 )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Max-Forwards: %d\r\n", m_iMaxForwards );
	}

	if( m_clsFrom.Empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, ( m_bUseCompact ? "f: " : "From: " ) );
		n = m_clsFrom.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_clsTo.Empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, ( m_bUseCompact ? "t: " : "To: " ) );
		n = m_clsTo.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_clsCallId.Empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, ( m_bUseCompact ? "i: " : "Call-ID: " ) );
		n = m_clsCallId.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_clsCSeq.Empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "CSeq: " );
		n = m_clsCSeq.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	for( SIP_FROM_LIST::iterator itList = m_clsContactList.begin(); itList != m_clsContactList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, ( m_bUseCompact ? "m: " : "Contact: " ) );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	for( SIP_CREDENTIAL_LIST::iterator itList = m_clsAuthorizationList.begin(); itList != m_clsAuthorizationList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Authorization: " );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	for( SIP_CHALLENGE_LIST::iterator itList = m_clsWwwAuthenticateList.begin(); itList != m_clsWwwAuthenticateList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "WWW-Authenticate: " );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	for( SIP_CREDENTIAL_LIST::iterator itList = m_clsProxyAuthorizationList.begin(); itList != m_clsProxyAuthorizationList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Proxy-Authorization: " );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	for( SIP_CHALLENGE_LIST::iterator itList = m_clsProxyAuthenticateList.begin(); itList != m_clsProxyAuthenticateList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Proxy-Authenticate: " );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_clsContentType.Empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, ( m_bUseCompact ? "c: " : "Content-Type: " ) );
		n = m_clsContentType.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	iLen += snprintf( pszText + iLen, iTextSize - iLen, "%s: %d\r\n", ( m_bUseCompact ? "l" : "Content-Length" ), m_iContentLength );

#ifdef USE_ACCEPT_HEADER
	if( m_clsAcceptList.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Accept: " );
		for( SIP_CONTENT_TYPE_LIST::iterator itList = m_clsAcceptList.begin(); itList != m_clsAcceptList.end(); ++itList )
		{
			if( itList != m_clsAcceptList.begin() )
			{
				iLen += snprintf( pszText + iLen, iTextSize - iLen, ", " );
			}
			n = itList->ToString( pszText + iLen, iTextSize - iLen );
			if( n == -1 ) return -1;
			iLen += n;
		}
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_clsAcceptEncodingList.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Accept-Encoding: " );
		for( SIP_ACCEPT_DATA_LIST::iterator itList = m_clsAcceptEncodingList.begin(); itList != m_clsAcceptEncodingList.end(); ++itList )
		{
			if( itList != m_clsAcceptEncodingList.begin() )
			{
				iLen += snprintf( pszText + iLen, iTextSize - iLen, ", " );
			}
			n = itList->ToString( pszText + iLen, iTextSize - iLen );
			if( n == -1 ) return -1;
			iLen += n;
		}
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_clsAcceptLanguageList.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Accept-Language: " );
		for( SIP_ACCEPT_DATA_LIST::iterator itList = m_clsAcceptLanguageList.begin(); itList != m_clsAcceptLanguageList.end(); ++itList )
		{
			if( itList != m_clsAcceptLanguageList.begin() )
			{
				iLen += snprintf( pszText + iLen, iTextSize - iLen, ", " );
			}
			n = itList->ToString( pszText + iLen, iTextSize - iLen );
			if( n == -1 ) return -1;
			iLen += n;
		}
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}
#endif

	if( m_iExpires >= 0 )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Expires: %d\r\n", m_iExpires );
	}

	if( m_strUserAgent.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "User-Agent: %s\r\n", m_strUserAgent.c_str() );
	}

	for( SIP_HEADER_LIST::iterator itList = m_clsHeaderList.begin(); itList != m_clsHeaderList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "%s: %s\r\n", itList->m_strName.c_str(), itList->m_strValue.c_str() );
	}

	iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );

	if( m_iContentLength > 0 )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "%s", m_strBody.c_str() );
	}

	return iLen;
}

/**
 * @ingroup SipParser
 * @brief 네트워크로 전송할 수 있는 SIP 메시지 문자열을 작성한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipMessage::MakePacket()
{
	char	szPacket[SIP_MESSAGE_MAX_LEN];

	if( ToString( szPacket, sizeof(szPacket) ) == -1 ) return false;

	m_strPacket = szPacket;

	return true;
}

/**
 * @ingroup SipParser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CSipMessage::Clear()
{
	m_strSipMethod.clear();
	m_clsReqUri.Clear();
	m_strSipVersion.clear();

	m_iStatusCode = -1;
	m_strReasonPhrase.clear();

	m_clsFrom.Clear();
	m_clsTo.Clear();

	m_clsViaList.clear();        
	m_clsContactList.clear();    
	m_clsRecordRouteList.clear();
	m_clsRouteList.clear();      

#ifdef USE_ACCEPT_HEADER
	m_clsAcceptList.clear();            
                            
	m_clsAcceptEncodingList.clear();    
	m_clsAcceptLanguageList.clear();    
#endif

	m_clsAuthorizationList.clear();     
	m_clsWwwAuthenticateList.clear();   

	m_clsProxyAuthorizationList.clear();
	m_clsProxyAuthenticateList.clear(); 

	m_clsHeaderList.clear();            

	m_clsCSeq.Clear();       
	m_clsCallId.Clear();     
	m_clsContentType.Clear();
	m_iContentLength = 0;
	                 
	m_iExpires = -1;      
	m_iMaxForwards = -1;  
	                 
	m_strUserAgent.clear();  
	m_strBody.clear();
}

/**
 * @ingroup SipParser
 * @brief SIP 요청 메시지인지 검사한다.
 * @returns SIP 요청 메시지이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipMessage::IsRequest()
{
	if( m_strSipMethod.empty() ) return false;

	return true;
}

/**
 * @ingroup SipParser
 * @brief SIP 메소드가 입력한 메소드와 같은지 검사한다.
 * @param pszMethod SIP 메소드
 * @returns SIP 메소드가 입력한 메소드와 같으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipMessage::IsMethod( const char * pszMethod )
{
	if( pszMethod == NULL ) return false;

	if( m_strSipMethod.empty() == false )
	{
		if( !strcmp( m_strSipMethod.c_str(), pszMethod ) ) return true;
	}
	else
	{
		if( !strcmp( m_clsCSeq.m_strMethod.c_str(), pszMethod ) ) return true;
	}

	return false;
}

/**
 * @ingroup SipParser
 * @brief Call-ID 가 동일한지 검사한다.
 * @param pclsMessage SIP 메시지 저장 객체
 * @returns Call-ID 가 동일하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipMessage::IsEqualCallId( CSipMessage * pclsMessage )
{
	if( pclsMessage == NULL ) return false;

	return m_clsCallId.IsEqual( &pclsMessage->m_clsCallId );
}

/**
 * @ingroup SipParser
 * @brief Call-ID 와 CSeq 의 번호가 동일한지 검사한다.
 * @param pclsMessage SIP 메시지 저장 객체
 * @returns Call-ID 와 CSeq 의 번호가 동일하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipMessage::IsEqualCallIdSeq( CSipMessage * pclsMessage )
{
	if( pclsMessage == NULL ) return false;

	if( m_clsCallId.IsEqual( &pclsMessage->m_clsCallId ) && m_clsCSeq.m_iDigit == pclsMessage->m_clsCSeq.m_iDigit ) return true;

	return false;
}

/**
 * @ingroup SipParser
 * @brief SIP 메시지에 100rel 헤더를 포함하고 있는가?
 * @returns 100rel 헤더를 포함하고 있으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipMessage::Is100rel( )
{
	CSipHeader * pclsHeader = GetHeader( "Supported" );
	if( pclsHeader )
	{
		if( strstr( pclsHeader->m_strValue.c_str(), "100rel" ) ) return true;
	}

	pclsHeader = GetHeader( "Requires" );
	if( pclsHeader )
	{
		if( strstr( pclsHeader->m_strValue.c_str(), "100rel" ) ) return true;
	}

	return false;
}

/**
 * @ingroup SipParser
 * @brief Call-ID 문자열을 가져온다.
 * @param strCallId Call-ID 문자열을 저장할 변수
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipMessage::GetCallId( std::string & strCallId )
{
	strCallId.clear();

	if( m_clsCallId.Empty() ) return false;
	m_clsCallId.ToString( strCallId );

	return true;
}

/**
 * @ingroup SipParser
 * @brief Call-ID + CSeq digit 문자열을 가져온다.
 * @param strCallId Call-ID + CSeq digit 문자열을 저장할 변수
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipMessage::GetCallIdSeq( std::string & strCallId )
{
	if( GetCallId( strCallId ) == false ) return false;

	char szDigit[12];

	snprintf( szDigit, sizeof(szDigit), " %d", m_clsCSeq.m_iDigit );
	strCallId.append( szDigit );

	return true;
}

/**
 * @ingroup SipParser
 * @brief Top Via 헤더에 발신 IP 주소와 포트 번호를 추가한다.
 * @param pszIp SIP 메시지 발신 IP 주소
 * @param iPort SIP 메시지 발신 포트 번호
 * @param eTransport SIP 메시지를 발신한 transport
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipMessage::AddIpPortToTopVia( const char * pszIp, int iPort, ESipTransport eTransport )
{
	SIP_VIA_LIST::iterator itViaList = m_clsViaList.begin();
	if( itViaList == m_clsViaList.end() ) return false;

	SIP_PARAMETER_LIST::iterator	itList;
	bool bRport = false, bReceived = false;

	char	szNum[11];

	snprintf( szNum, sizeof(szNum), "%d", iPort );

	for( itList = itViaList->m_clsParamList.begin(); itList != itViaList->m_clsParamList.end(); ++itList )
	{
		if( !strcmp( itList->m_strName.c_str(), SIP_RPORT ) )
		{
			itList->m_strValue = szNum;
			bRport = true;
		}
		else if( !strcmp( itList->m_strName.c_str(), SIP_RECEIVED ) )
		{
			itList->m_strValue = pszIp;
			bReceived = true;
		}

		if( bRport && bReceived ) break;
	}

	if( bRport == false && itViaList->m_iPort != iPort )
	{
		itViaList->InsertParam( SIP_RPORT, szNum );
	}

	if( bReceived == false && strcmp( itViaList->m_strHost.c_str(), pszIp ) )
	{
		itViaList->InsertParam( SIP_RECEIVED, pszIp );
	}

	if( eTransport == E_SIP_TCP && strcasecmp( itViaList->m_strTransport.c_str(), SIP_TRANSPORT_TCP ) )
	{
		itViaList->InsertParam( SIP_TRANSPORT, SIP_TRANSPORT_TCP );
	}

	return true;
}

/**
 * @ingroup SipParser
 * @brief Via 헤더를 추가한다.
 * @param pszIp			Via 헤더에 저장할 IP 주소
 * @param iPort			Via 헤더에 저장할 포트 번호
 * @param pszBranch Via branch tag 문자열
 * @param eTransport SIP 메시지를 발신할 transport
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipMessage::AddVia( const char * pszIp, int iPort, const char * pszBranch, ESipTransport eTransport )
{
	if( pszIp == NULL || strlen(pszIp) == 0 ) return false;
	if( iPort <= 0 ) return false;

	CSipVia clsVia;

	clsVia.m_strProtocolName = "SIP";
	clsVia.m_strProtocolVersion = "2.0";
	clsVia.m_strTransport = SipGetTransport( eTransport );

	clsVia.m_strHost = pszIp;
	clsVia.m_iPort = iPort;

	clsVia.InsertParam( SIP_RPORT, NULL );

	if( pszBranch == NULL || strlen(pszBranch) == 0 )
	{
		char	szBranch[SIP_BRANCH_MAX_SIZE];

		SipMakeBranch( szBranch, sizeof(szBranch) );
		clsVia.InsertParam( SIP_BRANCH, szBranch );
	}
	else
	{
		clsVia.InsertParam( SIP_BRANCH, pszBranch );
	}

	m_clsViaList.push_front( clsVia );

	return true;
}

/**
 * @ingroup SipParser
 * @brief Route 헤더를 추가한다.
 * @param pszIp		Route 헤더에 저장할 IP 주소
 * @param iPort		Route 헤더에 저장할 포트 번호
 * @param eTransport SIP 메시지를 발신할 transport
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipMessage::AddRoute( const char * pszIp, int iPort, ESipTransport eTransport )
{
	if( pszIp == NULL || strlen(pszIp) == 0 ) return false;
	if( iPort <= 0 ) return false;

	CSipFrom clsFrom;

	clsFrom.m_clsUri.m_strProtocol = SipGetProtocol( eTransport );
	clsFrom.m_clsUri.m_strHost = pszIp;
	clsFrom.m_clsUri.m_iPort = iPort;

	clsFrom.m_clsUri.InsertParam( "lr", NULL );
	clsFrom.m_clsUri.InsertTransport( eTransport );

	m_clsRouteList.push_front( clsFrom );

	return true;
}

/**
 * @ingroup SipParser
 * @brief RecordRoute 헤더를 추가한다.
 * @param pszIp		Route 헤더에 저장할 IP 주소
 * @param iPort		Route 헤더에 저장할 포트 번호
 * @param eTransport SIP 메시지를 발신할 transport
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipMessage::AddRecordRoute( const char * pszIp, int iPort, ESipTransport eTransport )
{
	if( pszIp == NULL || strlen(pszIp) == 0 ) return false;
	if( iPort <= 0 ) return false;

	CSipFrom clsFrom;

	clsFrom.m_clsUri.m_strProtocol = SipGetProtocol( eTransport );
	clsFrom.m_clsUri.m_strHost = pszIp;
	clsFrom.m_clsUri.m_iPort = iPort;

	clsFrom.m_clsUri.InsertParam( "lr", NULL );
	clsFrom.m_clsUri.InsertTransport( eTransport );

	m_clsRecordRouteList.push_front( clsFrom );

	return true;
}


/**
 * @ingroup SipParser
 * @brief SIP 헤더 자료구조에 이름과 값을 추가한다.
 * @param pszName		SIP 헤더 이름
 * @param pszValue	SIP 헤더 값
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipMessage::AddHeader( const char * pszName, const char * pszValue )
{
	if( pszName == NULL || pszValue == NULL ) return false;

	CSipHeader clsHeader;

	clsHeader.m_strName = pszName;
	clsHeader.m_strValue = pszValue;

	m_clsHeaderList.push_back( clsHeader );

	return true;
}

/**
 * @ingroup SipParser
 * @brief SIP 헤더 자료구조에 이름과 값을 추가한다.
 * @param pszName		SIP 헤더 이름
 * @param iValue		SIP 헤더 값
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipMessage::AddHeader( const char * pszName, int iValue )
{
	char szValue[22];

	snprintf( szValue, sizeof(szValue), "%d", iValue );

	return AddHeader( pszName, szValue );
}

/**
 * @ingroup SipParser
 * @brief SIP 응답 메시지 객체를 생성한다.
 * @param iStatus		SIP 응답 코드
 * @param pszToTag	To 헤더의 tag
 * @returns 성공하면 SIP 응답 메시지 객체의 포인터를 리턴하고 실패하면 NULL 을 리턴한다.
 */
CSipMessage * CSipMessage::CreateResponse( int iStatus, const char * pszToTag )
{
	if( IsRequest() == false ) return NULL;

	CSipMessage * pclsResponse = new CSipMessage();
	if( pclsResponse == NULL ) return NULL;

	pclsResponse->m_iStatusCode = iStatus;
	pclsResponse->m_clsTo = m_clsTo;
	pclsResponse->m_clsFrom = m_clsFrom;
	pclsResponse->m_clsViaList = m_clsViaList;
	pclsResponse->m_clsCallId = m_clsCallId;
	pclsResponse->m_clsCSeq = m_clsCSeq;
	pclsResponse->m_eTransport = m_eTransport;

	if( iStatus != SIP_TRYING )
	{
		// 100 Trying 은 SIP Record-Route 헤더를 포함하지 않아도 된다.
		pclsResponse->m_clsRecordRouteList = m_clsRecordRouteList;
	}

	if( pszToTag )
	{
		pclsResponse->m_clsTo.InsertParam( SIP_TAG, pszToTag );
	}

	return pclsResponse;
}

/**
 * @ingroup SipParser
 * @brief To 헤더의 tag 가 존재하지 않는 경우 tag 를 생성하여서 저장한 SIP 응답 메시지 객체를 생성한다.
 * @param iStatus SIP 응답 코드
 * @returns 성공하면 SIP 응답 메시지 객체의 포인터를 리턴하고 실패하면 NULL 을 리턴한다.
 */
CSipMessage * CSipMessage::CreateResponseWithToTag( int iStatus )
{
	if( IsRequest() == false ) return NULL;

	CSipMessage * pclsResponse = new CSipMessage();
	if( pclsResponse == NULL ) return NULL;

	pclsResponse->m_iStatusCode = iStatus;
	pclsResponse->m_clsTo = m_clsTo;
	pclsResponse->m_clsFrom = m_clsFrom;
	pclsResponse->m_clsViaList = m_clsViaList;
	pclsResponse->m_clsCallId = m_clsCallId;
	pclsResponse->m_clsCSeq = m_clsCSeq;
	pclsResponse->m_eTransport = m_eTransport;

	if( pclsResponse->m_clsTo.SelectParam( SIP_TAG ) == false )
	{
		pclsResponse->m_clsTo.InsertTag();
	}

	return pclsResponse;
}

/**
 * @ingroup SipParser
 * @brief Top Via 헤더의 IP 주소와 포트 번호를 가져온다.
 * @param strIp		IP 주소를 저장할 변수
 * @param iPort		포트 번호를 저장할 변수
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipMessage::GetTopViaIpPort( std::string & strIp, int & iPort )
{
	strIp.clear();
	iPort = 0;

	SIP_VIA_LIST::iterator itViaList = m_clsViaList.begin();
	if( itViaList == m_clsViaList.end() ) return false;

	std::string strTemp;

	if( itViaList->SelectParam( SIP_RPORT, strTemp ) )
	{
		iPort = atoi( strTemp.c_str() );
	}

	if( itViaList->SelectParam( SIP_RECEIVED, strTemp ) )
	{
		strIp = strTemp;
	}

	if( iPort == 0 ) iPort = itViaList->m_iPort;
	if( strIp.empty() ) strIp = itViaList->m_strHost;

	return true;
}

/**
 * @ingroup SipParser
 * @brief Top Via 헤더의 IP 주소와 포트 번호를 수정한다.
 * @param pszIp		IP 주소
 * @param iPort		포트 번호
 * @param eTransport	transport
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 *					이미 저장된 값과 동일한 값이면 false 를 리턴한다.
 */
bool CSipMessage::SetTopViaIpPort( const char * pszIp, int iPort, ESipTransport eTransport )
{
	SIP_VIA_LIST::iterator itViaList = m_clsViaList.begin();
	if( itViaList == m_clsViaList.end() ) return false;

	const char * pszTransport = SipGetTransport( eTransport );
		
	if( !strcmp( itViaList->m_strHost.c_str(), pszIp ) && itViaList->m_iPort == iPort && !strcmp( itViaList->m_strTransport.c_str(), pszTransport ) ) return false;

	itViaList->m_strHost = pszIp;
	itViaList->m_iPort = iPort;
	itViaList->m_strTransport = pszTransport;

	return true;
}

/**
 * @ingroup SipParser
 * @brief Top Via 헤더의 transport 문자열을 수정한다.
 * @param eTransport	transport
 * @param iPort				포트 번호
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 *					이미 저장된 값과 동일한 값이면 false 를 리턴한다.
 */
bool CSipMessage::SetTopViaTransPort( ESipTransport eTransport, int iPort )
{
	SIP_VIA_LIST::iterator itViaList = m_clsViaList.begin();
	if( itViaList == m_clsViaList.end() ) return false;

	const char * pszTransport = SipGetTransport( eTransport );

	if( !strcmp( itViaList->m_strTransport.c_str(), pszTransport ) && itViaList->m_iPort == iPort ) return false;

	itViaList->m_strTransport = pszTransport;
	itViaList->m_iPort = iPort;

	return true;
}

/**
 * @ingroup SipParser
 * @brief Top Contact 헤더의 IP 주소와 포트 번호를 수정한다.
 * @param pszIp		IP 주소
 * @param iPort		포트 번호
 * @param eTransport	transport
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 *					이미 저장된 값과 동일한 값이면 false 를 리턴한다.
 */
bool CSipMessage::SetTopContactIpPort( const char * pszIp, int iPort, ESipTransport eTransport )
{
	SIP_FROM_LIST::iterator	itList = m_clsContactList.begin();
	if( itList == m_clsContactList.end() ) return false;

	if( !strcmp( itList->m_clsUri.m_strHost.c_str(), pszIp) && itList->m_clsUri.m_iPort == iPort ) return false;

	itList->m_clsUri.m_strHost = pszIp;
	itList->m_clsUri.m_iPort = iPort;

	return true;
}

/**
 * @ingroup SipParser
 * @brief Expires 헤더가 존재하면 Expires 헤더 값을 리턴하고 Contact 헤더에 expires 가 존재하면 Contact 헤더의 expires 를 리턴한다.
 * @returns Expires 헤더가 존재하면 Expires 헤더 값을 리턴하고 Contact 헤더에 expires 가 존재하면 Contact 헤더의 expires 를 리턴한다.
 *					둘 다 없으면 0 을 리턴한다.
 */
int CSipMessage::GetExpires()
{
	if( m_iExpires != -1 ) return m_iExpires;

	SIP_FROM_LIST::iterator itContact = m_clsContactList.begin();
	if( itContact == m_clsContactList.end() ) return 0;

	std::string	strExpires;

	if( itContact->SelectParam( "EXPIRES", strExpires ) )
	{
		return atoi( strExpires.c_str() );
	}

	return 0;
}

/**
 * @ingroup SipParser
 * @brief 헤더 리스트를 검색하여서 입력된 이름과 일치하는 헤더를 리턴한다.
 * @param pszName 헤더 이름
 * @returns 헤더 리스트에 존재하면 헤더 객체를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CSipHeader * CSipMessage::GetHeader( const char * pszName )
{
	SIP_HEADER_LIST::iterator	itList;

	for( itList = m_clsHeaderList.begin(); itList != m_clsHeaderList.end(); ++itList )
	{
		if( !strcasecmp( pszName, itList->m_strName.c_str() ) )
		{
			return &(*itList);
		}
	}

	return NULL;
}

/**
 * @ingroup SipParser
 * @brief SIP status line 을 파싱한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipMessage::ParseStatusLine( const char * pszText, int iTextLen )
{
	int		iPos, iStartPos = -1;
	char	cType = 0;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( cType != 2 )
		{
			if( pszText[iPos] == ' ' )
			{
				switch( cType )
				{
				case 0:
					m_strSipVersion.append( pszText, iPos );
					break;
				case 1:
					{
						std::string strStatusCode;

						strStatusCode.append( pszText + iStartPos, iPos - iStartPos );
						m_iStatusCode = atoi( strStatusCode.c_str() );
						break;
					}
				}

				iStartPos = iPos + 1;
				++cType;
			}
		}
		else
		{
			if( pszText[iPos] == '\r' )
			{
				if( iPos + 1 >= iTextLen || pszText[iPos+1] != '\n' ) return -1;

				m_strReasonPhrase.append( pszText + iStartPos, iPos - iStartPos );
				return iPos + 2;
			}
		}
	}

	return -1;
}

/**
 * @ingroup SipParser
 * @brief SIP request line 을 파싱한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CSipMessage::ParseRequestLine( const char * pszText, int iTextLen )
{
	int		iPos, iStartPos = -1;
	char	cType = 0;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( cType != 2 )
		{
			if( pszText[iPos] == ' ' )
			{
				switch( cType )
				{
				case 0:
					m_strSipMethod.append( pszText, iPos );
					break;
				case 1:
					{
						if( m_clsReqUri.Parse( pszText + iStartPos, iPos - iStartPos ) == -1 ) return -1;
						break;
					}
				}

				iStartPos = iPos + 1;
				++cType;
			}
		}
		else
		{
			if( pszText[iPos] == '\r' )
			{
				if( iPos + 1 >= iTextLen || pszText[iPos+1] != '\n' ) return -1;

				m_strSipVersion.append( pszText + iStartPos, iPos - iStartPos );
				return iPos + 2;
			}
		}
	}

	return -1;
}
