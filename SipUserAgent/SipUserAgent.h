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

#ifndef _SIP_USER_AGENT_H_
#define _SIP_USER_AGENT_H_

#include "SipStack.h"
#include "SipServerInfo.h"
#include "SipUserAgentCallBack.h"
#include "SipDialog.h"

/**
 * @defgroup SipUserAgent SipUserAgent
 * SIP User Agent 라이브러리
 * SIP 로그인/통화 관리 라이브러리
 */

/**
 * @ingroup SipUserAgent
 * @brief SIP 메시지 목적지 주소 저장 클래스
 */
class CSipCallRoute
{
public:
	/** 목적지 IP 주소 */
	std::string	m_strDestIp;

	/** 목적지 포트 번호 */
	int					m_iDestPort;
};

typedef std::map< std::string, CSipDialog > SIP_DIALOG_MAP;

/**
 * @ingroup SipUserAgent
 * @brief SIP UserAgent 클래스
 */
class CSipUserAgent : public ISipStackCallBack
{
public:
	CSipUserAgent();
	~CSipUserAgent();

	bool AddRegisterInfo( CSipServerInfo & clsInfo );

	// 로그인 관련
	bool Start( CSipStackSetup & clsSetup );
	bool Stop( );

	// 통화 관련
	bool StartCall( const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp, CSipCallRoute * pclsRoute, std::string & strCallId );
	bool StopCall( const char * pszCallId );
	bool AcceptCall( const char * pszCallId, CSipCallRtp * pclsRtp );
	
	// IP-PBX 에서 사용되는 통화 관련 메소드
	CSipMessage * DeleteIncomingCall( const char * pszCallId );
	bool RingCall( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp );

	// ISipStackCallBack
	virtual bool RecvRequest( int iThreadId, CSipMessage * pclsMessage );
	virtual bool RecvResponse( int iThreadId, CSipMessage * pclsMessage );

	/** IP-PBX 로그인 정보 저장 자료구조 */
	SIP_SERVER_INFO_LIST	m_clsRegisterList;
	CSipMutex							m_clsRegisterMutex;

	ISipUserAgentCallBack * m_pclsCallBack;

private:
	bool RecvRegisterResponse( int iThreadId, CSipMessage * pclsMessage );

	bool RecvInviteRequest( int iThreadId, CSipMessage * pclsMessage );
	bool RecvInviteResponse( int iThreadId, CSipMessage * pclsMessage );
	
	bool RecvByeRequest( int iThreadId, CSipMessage * pclsMessage );

	bool RecvCancelRequest( int iThreadId, CSipMessage * pclsMessage );

	bool SendInvite( CSipDialog & clsDialog );
	bool Delete( const char * pszCallId );

	bool SetInviteResponse( CSipMessage * pclsMessage, CSipCallRtp * pclsRtp );
	bool GetSipCallRtp( CSipMessage * pclsMessage, CSipCallRtp & clsRtp );

	/** SIP Dialog 맵 */
	SIP_DIALOG_MAP			m_clsMap;
	CSipMutex						m_clsMutex;
};

extern CSipStack	gclsSipStack;

#endif
