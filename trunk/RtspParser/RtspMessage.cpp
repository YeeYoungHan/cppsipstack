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
#include "RtspMessage.h"
#include "SipStatusCode.h"

CRtspMessage::CRtspMessage() : m_iStatusCode(-1), m_iCSeq(0), m_iContentLength(0)
{
}

CRtspMessage::~CRtspMessage()
{
}

int CRtspMessage::Parse( const char * pszText, int iTextLen )
{
	if( pszText == NULL || iTextLen <= 4 ) return -1;

	Clear();

	int iPos, iCurPos, iNameLen, iValueLen;
	const char * pszName, * pszValue;
	CSipHeader	clsHeader;

#ifdef PARSE_FAST
	bool bNotFound;
#endif

	if( !strncmp( pszText, "RTSP/", 4 ) )
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

		if( !strcasecmp( pszName, "CSeq" ) )
		{
			m_iCSeq = atoi( pszValue );
		}
		if( !strcasecmp( pszName, "Content-Type" ) )
		{
			if( m_clsContentType.Parse( pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Content-Length" ) )
		{
			m_iContentLength = atoi( pszValue );
		}
		else
		{
			m_clsHeaderList.push_back( clsHeader );
		}
	}

	if( m_iContentLength > 0 )
	{
		if( m_iContentLength > ( iTextLen - iCurPos ) ) return -1;

		m_strBody.append( pszText + iCurPos, m_iContentLength );
		return iTextLen;
	}

	return iCurPos;
}

void CRtspMessage::Clear()
{
	m_strRtspMethod.clear();
	m_clsReqUri.Clear();
	m_strRtspVersion.clear();
	m_iStatusCode = -1;
	m_strReasonPhrase.clear();
	m_iCSeq = 0;
	m_clsContentType.Clear();
	m_iContentLength = 0;
	m_clsHeaderList.clear();
	m_strBody.clear();
}

int CRtspMessage::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	int iLen, n;

	if( m_strRtspVersion.empty() ) m_strRtspVersion = RTSP_VERSION;

	if( m_iStatusCode > 0 )
	{
		if( m_strReasonPhrase.empty() )
		{
			m_strReasonPhrase = GetReasonPhrase( m_iStatusCode );
		}

		iLen = snprintf( pszText, iTextSize, "%s %d %s\r\n", m_strRtspVersion.c_str(), m_iStatusCode, m_strReasonPhrase.c_str() );
	}
	else
	{
		if( m_strRtspMethod.empty() || m_clsReqUri.Empty() || m_strRtspVersion.empty() ) return -1;
		iLen = snprintf( pszText, iTextSize, "%s ", m_strRtspMethod.c_str() );
		n = m_clsReqUri.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, " %s\r\n", m_strRtspVersion.c_str() );
	}

	iLen += snprintf( pszText + iLen, iTextSize - iLen, "%s: %d\r\n", "CSeq", m_iCSeq );

	if( m_clsContentType.Empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Content-Type: " );
		n = m_clsContentType.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	iLen += snprintf( pszText + iLen, iTextSize - iLen, "%s: %d\r\n", "Content-Length", m_iContentLength );

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
 * @ingroup RtspParser
 * @brief SIP 헤더 자료구조에 이름과 값을 추가한다.
 * @param pszName		SIP 헤더 이름
 * @param pszValue	SIP 헤더 값
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CRtspMessage::AddHeader( const char * pszName, const char * pszValue )
{
	if( pszName == NULL || pszValue == NULL ) return false;

	CSipHeader clsHeader;

	clsHeader.m_strName = pszName;
	clsHeader.m_strValue = pszValue;

	m_clsHeaderList.push_back( clsHeader );

	return true;
}

CRtspMessage * CRtspMessage::CreateResponse( int iStatus )
{
	CRtspMessage * pclsResponse = new CRtspMessage();
	if( pclsResponse == NULL ) return NULL;

	pclsResponse->m_iStatusCode = iStatus;
	pclsResponse->m_iCSeq = m_iCSeq;

	return pclsResponse;
}

/**
 * @ingroup RtspParser
 * @brief RTSP status line 을 파싱한다.
 * @param pszText		RTSP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CRtspMessage::ParseStatusLine( const char * pszText, int iTextLen )
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
					m_strRtspVersion.append( pszText, iPos );
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
 * @ingroup RtspParser
 * @brief RTSP request line 을 파싱한다.
 * @param pszText		SIP 헤더의 값을 저장한 문자열
 * @param iTextLen	pszText 문자열의 길이
 * @returns 성공하면 파싱한 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CRtspMessage::ParseRequestLine( const char * pszText, int iTextLen )
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
					m_strRtspMethod.append( pszText, iPos );
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

				m_strRtspVersion.append( pszText + iStartPos, iPos - iStartPos );
				return iPos + 2;
			}
		}
	}

	return -1;
}
