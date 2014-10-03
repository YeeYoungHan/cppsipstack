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

	/** RTSP �޼ҵ� ( SETUP, PLAY �� ) */
	std::string		m_strRtspMethod;

	/** RTSP request URI */
	CRtspUri				m_clsReqUri;

	/** RTSP version ( SIP/2.0 ) */
	std::string		m_strRtspVersion;

	/** RTSP ���� �ڵ�. RTSP ���� �޽����� ��쿡�� 0 ���� ū ���� ������ �ִ�. */
	int						m_iStatusCode;

	/** RTSP ���� �޽��� */
	std::string		m_strReasonPhrase;

	/** RTSP CSeq ��� */
	CSipCSeq				m_clsCSeq;

	/** RTSP Content-Type ��� */
	CSipContentType	m_clsContentType;

	/** RTSP Content-Length ����� �� */
	int							m_iContentLength;

	/** RTSP ��� ����Ʈ. CRtspMessage ���� �����Ͽ��� ������ ����� ������� �ʴ� ������� �����Ѵ�. */
	SIP_HEADER_LIST				m_clsHeaderList;

	/** RTSP body �޽��� */
	std::string			m_strBody;

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );

private:
	int ParseStatusLine( const char * pszText, int iTextLen );
	int ParseRequestLine( const char * pszText, int iTextLen );
};

#endif
