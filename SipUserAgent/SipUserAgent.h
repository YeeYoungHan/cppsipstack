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
	CSipCallRoute() : m_eTransport( E_SIP_UDP ), m_b100rel(false)
	{}

	/** 목적지 IP 주소 */
	std::string	m_strDestIp;

	/** 목적지 포트 번호 */
	int					m_iDestPort;

	/** SIP transport */
	ESipTransport	m_eTransport;

	/** 100rel 기능 */
	bool				m_b100rel;
};

// key 는 SIP Call-ID 이다.
typedef MAP< std::string, CSipDialog > SIP_DIALOG_MAP;

typedef std::list< std::string > SIP_CALL_ID_LIST;

/**
 * @ingroup SipUserAgent
 * @brief SIP UserAgent 클래스
 */
class CSipUserAgent : public ISipStackCallBack
{
public:
	CSipUserAgent();
	~CSipUserAgent();

	bool InsertRegisterInfo( CSipServerInfo & clsInfo );
	bool UpdateRegisterInfo( CSipServerInfo & clsInfo );
	bool DeleteRegisterInfo( CSipServerInfo & clsInfo );

	// 로그인 관련
	bool Start( CSipStackSetup & clsSetup, ISipUserAgentCallBack * pclsCallBack, ISipStackSecurityCallBack * pclsSecurityCallBack = NULL );
	bool Stop( );

	// 통화 관련
	bool StartCall( const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp, CSipCallRoute * pclsRoute, std::string & strCallId );
	bool StopCall( const char * pszCallId, int iSipCode = 0 );
	bool RingCall( const char * pszCallId, CSipCallRtp * pclsRtp );
	bool AcceptCall( const char * pszCallId, CSipCallRtp * pclsRtp );
	bool SendPrack( const char * pszCallId, CSipCallRtp * pclsRtp );

	int GetCallCount( );
	void GetCallIdList( SIP_CALL_ID_LIST & clsList );
	void StopCallAll( );

	bool CreateCall( const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp, CSipCallRoute * pclsRoute, std::string & strCallId, CSipMessage ** ppclsInvite );
	bool StartCall( const char * pszCallId, CSipMessage * pclsInvite );

	bool TransferCallBlind( const char * pszCallId, const char * pszTo );
	bool TransferCall( const char * pszCallId, const char * pszToCallId );

	bool SendSms( const char * pszFrom, const char * pszTo, const char * pszText, CSipCallRoute * pclsRoute );
	
	// IP-PBX 에서 사용되는 통화 관련 메소드
	CSipMessage * DeleteIncomingCall( const char * pszCallId );
	bool RingCall( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp );

	bool GetRemoteCallRtp( const char * pszCallId, CSipCallRtp * pclsRtp );
	bool GetToId( const char * pszCallId, std::string & strToId );
	bool GetFromId( const char * pszCallId, std::string & strFromId );
	bool GetCdr( const char * pszCallId, CSipCdr * pclsCdr );
	bool IsRingCall( const char * pszCallId, const char * pszTo );

	int GetRSeq( const char * pszCallId );
	void SetRSeq( const char * pszCallId, int iRSeq );
	bool Is100rel( const char * pszCallId );

	bool SendReInvite( const char * pszCallId, CSipCallRtp * pclsRtp );
	bool SendNotify( const char * pszCallId, int iSipCode );

	void GetString( std::string & strBuf );
	void Final();

	// ISipStackCallBack
	virtual bool RecvRequest( int iThreadId, CSipMessage * pclsMessage );
	virtual bool RecvResponse( int iThreadId, CSipMessage * pclsMessage );
	virtual bool SendTimeout( int iThreadId, CSipMessage * pclsMessage );
	virtual void TcpSessionEnd( const char * pszIp, int iPort, ESipTransport eTransport );
	virtual void CallBackThreadEnd( int iThreadId );

	/** IP-PBX 로그인 정보 저장 자료구조 */
	SIP_SERVER_INFO_LIST	m_clsRegisterList;
	CSipMutex							m_clsRegisterMutex;

	CSipStack							m_clsSipStack;

	/** 중지 이벤트 */
	bool m_bStopEvent;

private:
	void DeleteRegisterInfoAll( );
	void DeRegister( );

	bool RecvRegisterResponse( int iThreadId, CSipMessage * pclsMessage );

	bool RecvInviteRequest( int iThreadId, CSipMessage * pclsMessage );
	bool RecvInviteResponse( int iThreadId, CSipMessage * pclsMessage );
	
	bool RecvByeRequest( int iThreadId, CSipMessage * pclsMessage );

	bool RecvCancelRequest( int iThreadId, CSipMessage * pclsMessage );

	bool RecvReferRequest( int iThreadId, CSipMessage * pclsMessage );
	bool RecvReferResponse( int iThreadId, CSipMessage * pclsMessage );

	bool RecvNotifyRequest( int iThreadId, CSipMessage * pclsMessage );

	bool RecvMessageRequest( int iThreadId, CSipMessage * pclsMessage );

	bool RecvPrackRequest( int iThreadId, CSipMessage * pclsMessage );

	bool SendInvite( CSipDialog & clsDialog );
	bool SetCallEnd( const char * pszCallId );
	bool Delete( const char * pszCallId );
	void Delete( SIP_DIALOG_MAP::iterator & itMap );

	bool SetInviteResponse( std::string & strCallId, CSipMessage * pclsMessage, CSipCallRtp * pclsRtp );
	bool GetSipCallRtp( CSipMessage * pclsMessage, CSipCallRtp & clsRtp );

	int GetSeqNum( );

	/** 응용 프로그램 callback 인터페이스 */
	ISipUserAgentCallBack * m_pclsCallBack;

	/** SIP Dialog 맵 */
	SIP_DIALOG_MAP			m_clsMap;
	CSipMutex						m_clsMutex;

	/** */
	int									m_iSeq;
	CSipMutex						m_clsOtherMutex;

	bool								m_bStart;
};

#endif
