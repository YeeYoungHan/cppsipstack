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
 * SIP User Agent ���̺귯��
 * SIP �α���/��ȭ ���� ���̺귯��
 */

/**
 * @ingroup SipUserAgent
 * @brief SIP �޽��� ������ �ּ� ���� Ŭ����
 */
class CSipCallRoute
{
public:
	CSipCallRoute() : m_iDestPort(0), m_eTransport( E_SIP_UDP ), m_b100rel(false)
	{}

	/** ������ IP �ּ� */
	std::string	m_strDestIp;

	/** ������ ��Ʈ ��ȣ */
	int					m_iDestPort;

	/** SIP transport */
	ESipTransport	m_eTransport;

	/** 100rel ��� */
	bool				m_b100rel;
};

// key �� SIP Call-ID �̴�.
typedef MAP< std::string, CSipDialog > SIP_DIALOG_MAP;

typedef std::list< std::string > SIP_CALL_ID_LIST;

/**
 * @ingroup SipUserAgent
 * @brief SIP UserAgent Ŭ����
 */
class CSipUserAgent : public ISipStackCallBack
{
public:
	CSipUserAgent();
	~CSipUserAgent();

	// SipUserAgentLogin.hpp : �α��� ����
	bool InsertRegisterInfo( CSipServerInfo & clsInfo );
	bool UpdateRegisterInfo( CSipServerInfo & clsInfo );
	bool DeleteRegisterInfo( CSipServerInfo & clsInfo );

	// ����/���� ����
	bool Start( CSipStackSetup & clsSetup, ISipUserAgentCallBack * pclsCallBack, ISipStackSecurityCallBack * pclsSecurityCallBack = NULL );
	bool Stop( );
	void Final();

	// ��ȭ ����
	bool StartCall( const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp, CSipCallRoute * pclsRoute, std::string & strCallId );
	bool StopCall( const char * pszCallId, int iSipCode = 0 );
	bool StopCall( const char * pszCallId, const char * pszForward );
	bool RingCall( const char * pszCallId, CSipCallRtp * pclsRtp );
	bool AcceptCall( const char * pszCallId, CSipCallRtp * pclsRtp );
	bool HoldCall( const char * pszCallId, ERtpDirection eDirection = E_RTP_SEND );
	bool ResumeCall( const char * pszCallId );
	bool SendPrack( const char * pszCallId, CSipCallRtp * pclsRtp );

	int GetCallCount( );
	void GetCallIdList( SIP_CALL_ID_LIST & clsList );
	void StopCallAll( );

	bool CreateCall( const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp, CSipCallRoute * pclsRoute, std::string & strCallId, CSipMessage ** ppclsInvite );
	bool StartCall( const char * pszCallId, CSipMessage * pclsInvite );

	bool TransferCallBlind( const char * pszCallId, const char * pszTo );
	bool TransferCall( const char * pszCallId, const char * pszToCallId );

	// SipStackAgentSms.hpp
	bool SendSms( const char * pszFrom, const char * pszTo, const char * pszText, CSipCallRoute * pclsRoute );
	
	// SipUserAgentPbx.hpp : IP-PBX ���� ���Ǵ� ��ȭ ���� �޼ҵ�
	CSipMessage * DeleteIncomingCall( const char * pszCallId );
	bool RingCall( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp );

	bool GetRemoteCallRtp( const char * pszCallId, CSipCallRtp * pclsRtp );
	bool GetToId( const char * pszCallId, std::string & strToId );
	bool GetFromId( const char * pszCallId, std::string & strFromId );
	bool GetCdr( const char * pszCallId, CSipCdr * pclsCdr );

	bool IsRingCall( const char * pszCallId, const char * pszTo );
	bool GetInviteHeaderValue( const char * pszCallId, const char * pszHeaderName, std::string & strValue );

	int GetRSeq( const char * pszCallId );
	void SetRSeq( const char * pszCallId, int iRSeq );
	bool Is100rel( const char * pszCallId );

	bool SendReInvite( const char * pszCallId, CSipCallRtp * pclsRtp );
	bool SendNotify( const char * pszCallId, int iSipCode );

	// SipUserAgentMonitor.hpp
	void GetDialogString( CMonitorString & strBuf );
	void GetServerString( CMonitorString & strBuf );

	// SipUserAgentSipStack.hpp : ISipStackCallBack 
	virtual bool RecvRequest( int iThreadId, CSipMessage * pclsMessage );
	virtual bool RecvResponse( int iThreadId, CSipMessage * pclsMessage );
	virtual bool SendTimeout( int iThreadId, CSipMessage * pclsMessage );
	virtual void TcpSessionEnd( const char * pszIp, int iPort, ESipTransport eTransport );
	virtual void ThreadEnd( int iThreadId );

	/** IP-PBX �α��� ���� ���� �ڷᱸ�� */
	SIP_SERVER_INFO_LIST	m_clsRegisterList;
	CSipMutex							m_clsRegisterMutex;

	CSipStack							m_clsSipStack;

	/** ���� �̺�Ʈ */
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

	bool RecvOptionsRequest( int iThreadId, CSipMessage * pclsMessage );

	bool SendInvite( CSipDialog & clsDialog );
	bool SetCallEnd( const char * pszCallId );
	bool Delete( const char * pszCallId );
	void Delete( SIP_DIALOG_MAP::iterator & itMap );

	bool SetInviteResponse( std::string & strCallId, CSipMessage * pclsMessage, CSipCallRtp * pclsRtp, bool & bReInvite );
	bool GetSipCallRtp( CSipMessage * pclsMessage, CSipCallRtp & clsRtp );

	int GetSeqNum( );

	/** ���� ���α׷� callback �������̽� */
	ISipUserAgentCallBack * m_pclsCallBack;

	/** SIP Dialog �� */
	SIP_DIALOG_MAP			m_clsDialogMap;
	CSipMutex						m_clsDialogMutex;

	/** */
	int									m_iSeq;
	CSipMutex						m_clsMutex;

	bool								m_bStart;
};

#endif
