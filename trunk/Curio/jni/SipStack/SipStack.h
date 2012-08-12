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

#ifndef _SIP_STACK_H_
#define _SIP_STACK_H_

#include "SipStackDefine.h"
#include "SipUdp.h"
#ifndef ANDROID
#include "SipStatusCode.h"
#else
#include "../sipparser/SipStatusCode.h"
#endif
#include "SipICTList.h"
#include "SipNICTList.h"
#include "SipISTList.h"
#include "SipNISTList.h"
#include "SipStackSetup.h"
#include "SipStackCallBack.h"

typedef std::list< ISipStackCallBack * > SIP_STACK_CALLBACK_LIST;
typedef std::list< ISipNetworkLog * > SIP_NETWORK_LOG_LIST;

/**
 * @brief SIP stack 클래스
 */
class CSipStack
{
public:
	CSipStack();
	~CSipStack();

	bool Start( CSipStackSetup & clsSetup );
	bool Stop( );
	bool AddCallBack( ISipStackCallBack * pclsCallBack );
	bool AddNetworkLog( ISipNetworkLog * pclsLog );

	bool SendSipMessage( CSipMessage * pclsMessage );
	bool RecvSipMessage( int iThreadId, CSipMessage * pclsMessage );

	bool Execute( struct timeval * psttTime );
	bool Send( CSipMessage * pclsMessage );

	void RecvRequest( int iThreadId, CSipMessage * pclsMessage );
	void RecvResponse( int iThreadId, CSipMessage * pclsMessage );

	void IncreateUdpThreadCount( int & iThreadId );
	void DecreateUdpThreadCount();

	void GetString( std::string & strBuf );
	void GetICTString( std::string & strBuf );

	void NetworkLog( bool bSend, const char * pszPacket );

	bool	m_bStopEvent;						// SIP stack thread 종료 이벤트
	bool	m_bStackThreadRun;			// SIP stack thread 가 실행 중에 있는가?

	Socket m_iUdpSocket;					// SIP 메시지 전송/수신 소켓 핸들
	CSipMutex m_clsUdpRecvMutex;	// SIP 메시지 수신 뮤텍스
	CSipStackSetup	m_clsSetup;		// SIP stack 설정

private:
	bool	 m_bStarted;
	int		 m_iUdpThreadRunCount;	// UDP 수신 쓰레드 개수
	CSipMutex m_clsMutex;
	CSipMutex m_clsUdpSendMutex;	// SIP 메시지 전송 뮤텍스

	CSipICTList		m_clsICT;
	CSipNICTList	m_clsNICT;
	CSipISTList		m_clsIST;
	CSipNISTList	m_clsNIST;

	SIP_STACK_CALLBACK_LIST	m_clsCallBackList;
	SIP_NETWORK_LOG_LIST		m_clsNetLogList;

	bool _Stop( );
	void CheckSipMessage( CSipMessage * pclsMessage );
};

#endif
