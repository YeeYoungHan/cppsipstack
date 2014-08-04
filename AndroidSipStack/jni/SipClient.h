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

#ifndef _SIP_CLIENT_H_
#define _SIP_CLIENT_H_

#include "SipUserAgent.h"
#include "jni.h"

/**
 * @defgroup AndroidSipStack AndroidSipStack
 * SIP Stack 을 이용한 SIP 클라이언트 예제 프로그램
 * SipUserAgent 라이브러리를 이용하여서 SIP stack 을 간접적으로 사용한다.
 * SIP 서버에 로그인하여 통화 요청/취소, 통화 종료, 통화 수신/수락/거절 기능을 수행한다.
 */

/**
 * @ingroup AndroidSipStack
 * @brief 안드로이드용 SIP stack 이벤트 클래스
 */
class CSipClient : public ISipUserAgentCallBack
{
public:
	virtual ~CSipClient(){}
	virtual void EventRegister( CSipServerInfo * pclsInfo, int iStatus );
	virtual void EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp );
	virtual void EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp );
	virtual void EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp );
	virtual void EventCallEnd( const char * pszCallId, int iSipStatus );
	virtual void EventReInvite( const char * pszCallId, CSipCallRtp * pclsRemoteRtp, CSipCallRtp * pclsLocalRtp );
	virtual void EventPrack( const char * pszCallId, CSipCallRtp * pclsRtp );
	virtual bool EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer );
	virtual bool EventBlindTransfer( const char * pszCallId, const char * pszReferToId );
	virtual bool EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage );
	virtual void EventThreadEnd( int iThreadId );

private:
	bool AttachCurrentThread( JNIEnv ** env );
};

extern CSipClient gclsSipClient;

#endif
