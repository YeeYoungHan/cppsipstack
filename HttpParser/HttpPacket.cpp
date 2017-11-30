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
#include "HttpPacket.h"
#include <stdlib.h>
#include "MemoryDebug.h"

CHttpPacket::CHttpPacket() : m_iChunkedLen(-1), m_eStatus(H_HPS_HEADER)
{
}

CHttpPacket::~CHttpPacket()
{
}

/**
 * @ingroup HttpParser
 * @brief HTTP 프로토콜 기반으로 수신한 패킷을 저장하고 파싱한다.
 * @param pszPacket		패킷
 * @param iPacketLen	패킷 길이
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CHttpPacket::AddPacket( const char * pszPacket, int iPacketLen )
{
	m_strBuf.append( pszPacket, iPacketLen );

	if( m_eStatus == H_HPS_BODY_END )
	{
		ClearMessage();
	}

	if( m_eStatus == H_HPS_HEADER )
	{
		const char * pszBuf = m_strBuf.c_str();
		const char * pszHeaderEnd = strstr( pszBuf, "\r\n\r\n" );
		if( pszHeaderEnd )
		{
			int iHeaderLen = pszHeaderEnd + 4 - pszBuf;

			if( m_clsMessage.ParseHeader( pszBuf, iHeaderLen ) != iHeaderLen )
			{
				return false;
			}

			m_strBuf.erase( 0, iHeaderLen );
			m_eStatus = H_HPS_BODY;
		}
	}

	if( m_eStatus == H_HPS_BODY )
	{
		if( m_clsMessage.m_bChunked )
		{
			while( m_iChunkedLen != 0 )
			{
				if( m_iChunkedLen == -1 )
				{
					if( ParseChunkedHeader() == false ) break;
				}

				if( m_iChunkedLen > 0 )
				{
					if( ParseChunkedBody() == false ) break;
				}
			}
		}
		else if( m_clsMessage.m_iContentLength > 0 )
		{
			int iBufLen = m_strBuf.length();
			int iBodyLen = m_clsMessage.m_strBody.length();

			if( iBufLen >= ( m_clsMessage.m_iContentLength - iBodyLen ) )
			{
				int iLen = m_clsMessage.m_iContentLength - iBodyLen;

				m_clsMessage.m_strBody.append( m_strBuf.c_str(), iLen );
				m_strBuf.erase( 0, iLen );
				m_eStatus = H_HPS_BODY_END;
			}
			else
			{
				m_clsMessage.m_strBody.append( m_strBuf.c_str(), iBufLen );
				m_strBuf.clear();
			}
		}
		else if( m_clsMessage.m_iContentLength == 0 )
		{
			m_eStatus = H_HPS_BODY_END;
		}
		else
		{
			m_clsMessage.m_strBody = m_strBuf;
			m_strBuf.clear();
			m_eStatus = H_HPS_BODY_END;
		}
	}

	return true;
}

/**
 * @ingroup HttpParser
 * @brief HTTP body 를 모두 가져왔는지 검사한다.
 * @returns HTTP body 를 모두 가져온 경우 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CHttpPacket::IsCompleted( )
{
	if( m_eStatus == H_HPS_BODY_END ) return true;

	return false;
}

/**
 * @ingroup HttpParser
 * @brief HTTP 메시지 객체를 초기화시킨다.
 */
void CHttpPacket::ClearMessage( )
{
	m_clsMessage.Clear();
	m_eStatus = H_HPS_HEADER;
	m_iChunkedLen = -1;
}

/**
 * @ingroup HttpParser
 * @brief HTTP 메시지 객체를 리턴한다.
 * @returns HTTP 메시지 객체를 리턴한다.
 */
CHttpMessage * CHttpPacket::GetHttpMessage( )
{
	return &m_clsMessage;
}

/**
 * @ingroup HttpParser
 * @brief Chunked header 를 파싱한다.
 * @returns Chunked header 가 모두 파싱되면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CHttpPacket::ParseChunkedHeader( )
{
	const char * pszBuf = m_strBuf.c_str();
	int iBufLen = m_strBuf.length();

	if( iBufLen < 3 ) return false;

	for( int i = 1; i < iBufLen; ++i )
	{
		if( pszBuf[i-1] == '\r' && pszBuf[i] == '\n' )
		{
			char szChunkLen[11];

			snprintf( szChunkLen, sizeof(szChunkLen), "%.*s", i - 1, pszBuf );
			m_iChunkedLen = strtoul( szChunkLen, NULL, 16 );

			if( m_iChunkedLen == 0 )
			{
				if( iBufLen >= ( i + 3 ) )
				{
					// chunked length 이 0 인 경우 body 의 \r\n 까지 수신되어야 정상으로 처리한다.
					m_strBuf.erase( 0, i + 3 );
					m_eStatus = H_HPS_BODY_END;

					return true;
				}
				else
				{
					m_iChunkedLen = -1;
				}
			}
			else
			{
				m_strBuf.erase( 0, i + 1 );
				return true;
			}
		}
	}

	return false;
}

/**
 * @ingroup HttpParser
 * @brief Chunked body 를 파싱한다.
 * @returns Chunked body 가 모두 파싱되면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CHttpPacket::ParseChunkedBody( )
{
	const char * pszBuf = m_strBuf.c_str();
	int iBufLen = m_strBuf.length();

	if( iBufLen >= m_iChunkedLen )
	{
		if( iBufLen >= ( m_iChunkedLen + 2 ) )
		{
			m_clsMessage.m_strBody.append( pszBuf, m_iChunkedLen );
			m_strBuf.erase( 0, m_iChunkedLen + 2 );
			m_iChunkedLen = -1;

			return true;
		}
	}
	else
	{
		m_clsMessage.m_strBody.append( pszBuf, iBufLen );
		m_strBuf.clear();
		m_iChunkedLen -= iBufLen;
	}

	return false;
}
