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

#ifndef _HTTP_MESSAGE_H_
#define _HTTP_MESSAGE_H_

#define HTTP_VERSION	"HTTP/1.1"

#include "HttpHeader.h"
#include "HttpUri.h"

/**
 * @ingroup HttpParser
 * @brief HTTP 메시지 클래스
 */
class CHttpMessage
{
public:
	CHttpMessage();
	~CHttpMessage();

	int Parse( const char * pszText, int iTextLen );
	int ParseHeader( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	void Clear();

	bool AddHeader( const char * pszName, const char * pszValue );
	bool AddHeader( const char * pszName, int iValue );

	bool UpdateHeader( const char * pszName, const char * pszValue );
	bool ReplaceHeader( const char * pszName, const char * pszValue );
	
	CHttpHeader * GetHeader( const char * pszName );

	bool SetRequest( const char * pszMethod, CHttpUri * pclsUri, const char * pszUserAgent = NULL );
	bool IsRequest( );

	/** HTTP 메소드 ( GET, POST ) */
	std::string		m_strHttpMethod;

	/** HTTP request URI */
	std::string		m_strReqUri;

	/** HTTP version ( HTTP/1.1 ) */
	std::string		m_strHttpVersion;

	/** HTTP 응답 코드. HTTP 응답 메시지인 경우에만 0 보다 큰 값을 가지고 있다. */
	int						m_iStatusCode;

	/** HTTP 응답 메시지 */
	std::string		m_strReasonPhrase;

	/** HTTP Content-Type 헤더의 값 */
	std::string		m_strContentType;

	/** HTTP Content-Length 헤더의 값 */
	int							m_iContentLength;

	/** body 의 chunked 여부 */
	bool						m_bChunked;

	/** HTTP 헤더 리스트 */
	HTTP_HEADER_LIST		m_clsHeaderList;

	/** HTTP body */
	std::string			m_strBody;

private:
	int ParseStatusLine( const char * pszText, int iTextLen );
	int ParseRequestLine( const char * pszText, int iTextLen );
};

#endif
