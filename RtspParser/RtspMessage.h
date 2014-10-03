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

#ifndef _RTSP_MESSAGE_H_
#define _RTSP_MESSAGE_H_

#include "RtspParserDefine.h"
#include "RtspUri.h"
#include "SipCSeq.h"
#include "SipHeader.h"
#include "SipContentType.h"

class CRtspMessage
{
public:
	CRtspMessage();
	~CRtspMessage();

	/** RTSP 메소드 ( SETUP, PLAY 등 ) */
	std::string		m_strRtspMethod;

	/** RTSP request URI */
	CRtspUri				m_clsReqUri;

	/** RTSP version ( SIP/2.0 ) */
	std::string		m_strRtspVersion;

	/** RTSP 응답 코드. RTSP 응답 메시지인 경우에만 0 보다 큰 값을 가지고 있다. */
	int						m_iStatusCode;

	/** RTSP 응답 메시지 */
	std::string		m_strReasonPhrase;

	/** RTSP CSeq 헤더 */
	CSipCSeq				m_clsCSeq;

	/** RTSP Content-Type 헤더 */
	CSipContentType	m_clsContentType;

	/** RTSP Content-Length 헤더의 값 */
	int							m_iContentLength;

	/** RTSP 헤더 리스트. CRtspMessage 에서 구분하여서 정의한 헤더에 저장되지 않는 헤더들을 저장한다. */
	SIP_HEADER_LIST				m_clsHeaderList;

	/** RTSP body 메시지 */
	std::string			m_strBody;

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );

private:
	int ParseStatusLine( const char * pszText, int iTextLen );
	int ParseRequestLine( const char * pszText, int iTextLen );
};

#endif
