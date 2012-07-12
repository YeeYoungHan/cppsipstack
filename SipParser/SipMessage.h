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

#ifndef _SIP_MESSAGE_H_
#define _SIP_MESSAGE_H_

#include "SipFrom.h"
#include "SipVia.h"
#include "SipAcceptData.h"
#include "SipCredential.h"
#include "SipChallenge.h"
#include "SipHeader.h"
#include "SipCSeq.h"
#include "SipCallId.h"
#include "SipContentType.h"

class CSipMessage
{
public:
	CSipMessage(void);
	~CSipMessage(void);

	std::string		m_strSipMethod;
	CSipUri				m_clsReqUri;
	std::string		m_strSipVersion;

	int						m_iStatusCode;
	std::string		m_strReasonPhrase;

	CSipFrom			m_clsFrom;
	CSipFrom			m_clsTo;

	SIP_VIA_LIST	m_clsViaList;
	SIP_FROM_LIST	m_clsContactList;
	SIP_FROM_LIST	m_clsRecordRouteList;
	SIP_FROM_LIST	m_clsRouteList;

	SIP_CONTENT_TYPE_LIST	m_clsAcceptList;

	SIP_ACCEPT_DATA_LIST	m_clsAcceptEncodingList;
	SIP_ACCEPT_DATA_LIST	m_clsAcceptLanguageList;

	SIP_CREDENTIAL_LIST		m_clsAuthorizationList;
	SIP_CREDENTIAL_LIST		m_clsProxyAuthorizationList;

	SIP_CHALLENGE_LIST		m_clsWwwAuthenticateList;
	SIP_CHALLENGE_LIST		m_clsProxyAuthenticateList;

	SIP_HEADER_LIST				m_clsHeaderList;

	CSipCSeq				m_clsCSeq;
	CSipCallId			m_clsCallId;
	CSipContentType	m_clsContentType;
	int							m_iContentLength;

	int							m_iExpires;
	int							m_iMaxForwards;

	std::string			m_strUserAgent;
	std::string			m_strBody;

	std::string			m_strPacket;

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	bool MakePacket();
	void Clear();

	bool IsRequest();
	bool IsMethod( const char * pszMethod );
	bool IsEqualCallId( CSipMessage * pclsMessage );
	bool AddIpPortToTopVia( const char * pszIp, int iPort );

private:
	int ParseStatusLine( const char * pszText, int iTextLen );
	int ParseRequestLine( const char * pszText, int iTextLen );
};

#endif
