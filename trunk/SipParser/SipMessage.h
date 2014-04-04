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

#include "SipParserDefine.h"
#include "SipFrom.h"
#include "SipVia.h"
#include "SipAcceptData.h"
#include "SipCredential.h"
#include "SipChallenge.h"
#include "SipHeader.h"
#include "SipCSeq.h"
#include "SipCallId.h"
#include "SipContentType.h"

/**
 * @ingroup SipParser
 * @brief SIP 메시지 정보를 저장하는 클래스
 */
class CSipMessage
{
public:
	CSipMessage();
	~CSipMessage();

	/** SIP 메소드 ( INVITE, CANCEL, ACK, BYE, REFER 등 ) */
	std::string		m_strSipMethod;

	/** SIP request URI */
	CSipUri				m_clsReqUri;

	/** SIP version ( SIP/2.0 ) */
	std::string		m_strSipVersion;

	/** SIP 응답 코드. SIP 응답 메시지인 경우에만 0 보다 큰 값을 가지고 있다. */
	int						m_iStatusCode;

	/** SIP 응답 메시지 */
	std::string		m_strReasonPhrase;

	/** SIP From 헤더 */
	CSipFrom			m_clsFrom;

	/** SIP To 헤더 */
	CSipFrom			m_clsTo;

	/** SIP Via 헤더 리스트 */
	SIP_VIA_LIST	m_clsViaList;

	/** SIP Contact 헤더 리스트 */
	SIP_FROM_LIST	m_clsContactList;

	/** SIP Record-Route 헤더 리스트 */
	SIP_FROM_LIST	m_clsRecordRouteList;

	/** SIP Route 헤더 리스트 */
	SIP_FROM_LIST	m_clsRouteList;

	/** SIP Accept 헤더 리스트 */
	SIP_CONTENT_TYPE_LIST	m_clsAcceptList;

	/** SIP Accept-Encoding 헤더 리스트 */
	SIP_ACCEPT_DATA_LIST	m_clsAcceptEncodingList;

	/** SIP Accept-Language 헤더 리스트 */
	SIP_ACCEPT_DATA_LIST	m_clsAcceptLanguageList;

	/** SIP Authorization 헤더 리스트 */
	SIP_CREDENTIAL_LIST		m_clsAuthorizationList;

	/** SIP Proxy-Authorization 헤더 리스트 */
	SIP_CREDENTIAL_LIST		m_clsProxyAuthorizationList;

	/** SIP Www-Authenticate 헤더 리스트 */
	SIP_CHALLENGE_LIST		m_clsWwwAuthenticateList;

	/** SIP Proxy-Authenticate 헤더 리스트 */
	SIP_CHALLENGE_LIST		m_clsProxyAuthenticateList;

	/** SIP 헤더 리스트. CSipMessage 에서 구분하여서 정의한 헤더에 저장되지 않는 헤더들을 저장한다. */
	SIP_HEADER_LIST				m_clsHeaderList;

	/** SIP CSeq 헤더 */
	CSipCSeq				m_clsCSeq;

	/** SIP Call-ID 헤더 */
	CSipCallId			m_clsCallId;

	/** SIP Content-Type 헤더 */
	CSipContentType	m_clsContentType;

	/** SIP Content-Length 헤더의 값 */
	int							m_iContentLength;

	/** SIP Expires 헤더의 값 */
	int							m_iExpires;

	/** SIP Max-Forwards 헤더의 값 */
	int							m_iMaxForwards;

	/** SIP User-Agent 헤더 */
	std::string			m_strUserAgent;

	/** SIP body 메시지 */
	std::string			m_strBody;

	/** 네트워크로 전송할 SIP 메시지 */
	std::string			m_strPacket;

	/** 네트워크로 전송/수신된 SIP 메시지의 transport */
	ESipTransport		m_eTransport;

	/** SIP 메시지를 전송한 클라이언트의 IP 주소 */
	std::string			m_strClientIp;

	/** SIP 메시지를 전송한 클라이언트의 포트 번호 */
	int							m_iClientPort;

	/** SIP 메시지를 compact form 으로 생성하는지 설정 */
	bool						m_bUseCompact;

	/** 객체 사용 개수 */
	int8_t					m_iUseCount;

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	bool MakePacket();
	void Clear();

	bool IsRequest();
	bool IsMethod( const char * pszMethod );
	bool IsEqualCallId( CSipMessage * pclsMessage );
	bool IsEqualCallIdSeq( CSipMessage * pclsMessage );
	bool GetCallId( std::string & strCallId );

	bool AddIpPortToTopVia( const char * pszIp, int iPort, ESipTransport eTransport = E_SIP_UDP );
	bool AddVia( const char * pszIp, int iPort, const char * pszBranch = NULL, ESipTransport eTransport = E_SIP_UDP );
	bool AddRoute( const char * pszIp, int iPort, ESipTransport eTransport = E_SIP_UDP );
	bool AddRecordRoute( const char * pszIp, int iPort, ESipTransport eTransport = E_SIP_UDP );
	bool AddHeader( const char * pszName, const char * pszValue );
	bool AddHeader( const char * pszName, int iValue );

	bool GetTopViaIpPort( std::string & strIp, int & iPort );

	int GetExpires();

	CSipHeader * GetHeader( const char * pszName );

	CSipMessage * CreateResponse( int iStatus, const char * pszToTag = NULL );
	CSipMessage * CreateResponseWithToTag( int iStatus );

private:
	int ParseStatusLine( const char * pszText, int iTextLen );
	int ParseRequestLine( const char * pszText, int iTextLen );
};

#endif
