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
	CSipCallRoute() : m_eTransport( E_SIP_UDP )
	{}

	/** ������ IP �ּ� */
	std::string	m_strDestIp;

	/** ������ ��Ʈ ��ȣ */
	int					m_iDestPort;

	/** SIP transport */
	ESipTransport	m_eTransport;
};

typedef std::map< std::string, CSipDialog > SIP_DIALOG_MAP;

/**
 * @ingroup SipUserAgent
 * @brief SIP UserAgent Ŭ����
 */
class CSipUserAgent : public ISipStackCallBack
{
public:
	CSipUserAgent();
	~CSipUserAgent();

	bool InsertRegisterInfo( CSipServerInfo & clsInfo );
	bool UpdateRegisterInfo( CSipServerInfo & clsInfo );
	bool DeleteRegisterInfo( CSipServerInfo & clsInfo );

	// �α��� ����
	bool Start( CSipStackSetup & clsSetup, ISipStackSecurityCallBack * pclsSecurityCallBack = NULL );
	bool Stop( );

	// ��ȭ ����
	bool StartCall( const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp, CSipCallRoute * pclsRoute, std::string & strCallId );
	bool StopCall( const char * pszCallId, int iSipCode = 0 );
	bool AcceptCall( const char * pszCallId, CSipCallRtp * pclsRtp );

	bool SendSms( const char * pszFrom, const char * pszTo, const char * pszText, CSipCallRoute * pclsRoute );
	
	// IP-PBX ���� ���Ǵ� ��ȭ ���� �޼ҵ�
	CSipMessage * DeleteIncomingCall( const char * pszCallId );
	bool RingCall( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp );

	bool GetRemoteCallRtp( const char * pszCallId, CSipCallRtp * pclsRtp );
	bool GetToId( const char * pszCallId, std::string & strToId );
	bool GetCdr( const char * pszCallId, CSipCdr * pclsCdr );

	bool SendReInvite( const char * pszCallId, CSipCallRtp * pclsRtp );
	bool SendNotify( const char * pszCallId, int iSipCode );

	void GetString( std::string & strBuf );

	// ISipStackCallBack
	virtual bool RecvRequest( int iThreadId, CSipMessage * pclsMessage );
	virtual bool RecvResponse( int iThreadId, CSipMessage * pclsMessage );
	virtual bool SendTimeout( int iThreadId, CSipMessage * pclsMessage );
	virtual void CallBackThreadEnd( int iThreadId );

	/** IP-PBX �α��� ���� ���� �ڷᱸ�� */
	SIP_SERVER_INFO_LIST	m_clsRegisterList;
	CSipMutex							m_clsRegisterMutex;

	/** ���� ���α׷� callback �������̽� */
	ISipUserAgentCallBack * m_pclsCallBack;

private:
	void DeleteRegisterInfoAll( );
	void DeRegister( );

	bool RecvRegisterResponse( int iThreadId, CSipMessage * pclsMessage );

	bool RecvInviteRequest( int iThreadId, CSipMessage * pclsMessage );
	bool RecvInviteResponse( int iThreadId, CSipMessage * pclsMessage );
	
	bool RecvByeRequest( int iThreadId, CSipMessage * pclsMessage );

	bool RecvCancelRequest( int iThreadId, CSipMessage * pclsMessage );

	bool RecvReferRequest( int iThreadId, CSipMessage * pclsMessage );

	bool RecvMessageRequest( int iThreadId, CSipMessage * pclsMessage );

	bool SendInvite( CSipDialog & clsDialog );
	bool SetCallEnd( const char * pszCallId );
	bool Delete( const char * pszCallId );

	bool SetInviteResponse( CSipMessage * pclsMessage, CSipCallRtp * pclsRtp );
	bool GetSipCallRtp( CSipMessage * pclsMessage, CSipCallRtp & clsRtp );

	int GetSeqNum( );

	/** SIP Dialog �� */
	SIP_DIALOG_MAP			m_clsMap;
	CSipMutex						m_clsMutex;

	/** */
	int									m_iSeq;
	CSipMutex						m_clsOtherMutex;
};

extern CSipStack	gclsSipStack;

#endif
