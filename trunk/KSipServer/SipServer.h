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

#ifndef _SIP_SERVER_H_
#define _SIP_SERVER_H_

#include "SipUserAgent.h"

/**
 * @defgroup SimpleSipServer SimpleSipServer
 * SIP Stack 을 이용한 초간단 SIP 서버 예제 프로그램
 */

/**
 * @ingroup SimpleSipServer
 * @brief SIP Stack 을 이용한 초간단 SIP 서버 클래스
 */
class CSipServer : public ISipStackCallBack, ISipUserAgentCallBack, ISipNetworkLog
{
public:
	CSipServer();
	~CSipServer();

	bool Start( CSipStackSetup & clsSetup );

	// ISipStackCallBack
	virtual bool RecvRequest( int iThreadId, CSipMessage * pclsMessage );
	virtual bool RecvResponse( int iThreadId, CSipMessage * pclsMessage );

	// ISipUserAgentCallBack
	virtual void EventRegister( CSipServerInfo clsInfo, int iStatus );
	virtual void EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp );
	virtual void EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp );
	virtual void EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp );
	virtual void EventCallEnd( const char * pszCallId, int iSipStatus );
	virtual void EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp );
	virtual void EventTimer( );

	// ISipNetworkLog
	virtual void SipLog( bool bSend, const char * pszPacket, const char * pszIp, int iPort );

private:
	bool SendResponse( CSipMessage * pclsMessage, int iStatusCode );

	// SipServerRegister.hpp
	bool RecvRequestRegister( int iThreadId, CSipMessage * pclsMessage );
};

extern CSipServer gclsSipServer;
extern CSipUserAgent gclsUserAgent;

#endif
