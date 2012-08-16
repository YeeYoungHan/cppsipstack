/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch), Jeong Jong Hwan(sosolili@naver.com)
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

#ifndef _SIP_CLIENT_H_
#define _SIP_CLIENT_H_

#ifndef ANDROID
#include "SipUserAgent.h"
#else
#include "../sipuseragent/SipUserAgent.h"
#endif

class CSipClient : public ISipUserAgentCallBack, public ISipNetworkLog
{
public:
	virtual ~CSipClient(){}
	virtual void EventRegister( CSipServerInfo clsInfo, int iStatus );
	virtual void EventIncomingCall( const char * pszCallId, const char * pszFrom, CSipCallRtp * pclsRtp );
	virtual void EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp );
	virtual void EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp );
	virtual void EventCallEnd( const char * pszCallId, int iSipStatus );
	virtual void EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp );

	virtual void SipLog( bool bSend, const char * pszPacket );
};

class CSipWrapper
{
public :
	CSipWrapper();
	~CSipWrapper();

public:
	CSipUserAgent clsUserAgent;
	CSipServerInfo clsServerInfo;
	CSipStackSetup clsSetup;
	CSipClient clsSipClient;

public:
	bool setSipClient();
	bool setSipServerInfo(char* pszServerIp, char* pszUserId, char* pszPassWord);
	bool setSipStackSetup(char* strLocalIp, int UdpPort);
	bool setCallback();
	bool start();
};

#endif
