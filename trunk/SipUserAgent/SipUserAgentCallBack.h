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

#ifndef _SIP_USER_AGENT_CALLBACK_H_
#define _SIP_USER_AGENT_CALLBACK_H_

#include "SipStackDefine.h"

/**
 * @brief RTP 정보 저장 클래스
 */
class CSipCallRtp
{
public:
	std::string	m_strIp;
	int					m_iPort;
	int					m_iCodec;
};

/**
 * @brief CSipUserAgent 의 이벤트를 응용 프로그램으로 전달하는 callback 인터페이스
 */
class ISipUserAgentCallBack
{
public:
	virtual ~ISipUserAgentCallBack(){};

	virtual void EventRegister( CSipServerInfo clsInfo, int iStatus ) = 0;
	virtual void EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp ) = 0;
	virtual void EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp ) = 0;
	virtual void EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp ) = 0;
	virtual void EventCallEnd( const char * pszCallId, int iSipStatus ) = 0;
	virtual void EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp ) = 0;

	virtual void EventTimer( ) = 0;
};

#endif
