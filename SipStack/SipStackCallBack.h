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

#ifndef _SIP_STACK_CALLBACK_H_
#define _SIP_STACK_CALLBACK_H_

#include "SipStackDefine.h"

/**
 * @ingroup SipStack
 * @brief SIP stack callback 인터페이스
 */
class ISipStackCallBack
{
public:
	virtual ~ISipStackCallBack(){};

	/**
   * @ingroup SipStack
	 * @brief SIP 요청 메시지 수신 이벤트 핸들러
	 * @param iThreadId		UDP 쓰레드 번호
	 * @param pclsMessage SIP 요청 메시지
	 * @returns SIP 요청 메시지를 처리하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
	 */
	virtual bool RecvRequest( int iThreadId, CSipMessage * pclsMessage ) = 0;

	/**
   * @ingroup SipStack
	 * @brief SIP 응답 메시지 수신 이벤트 핸들러
	 * @param iThreadId		UDP 쓰레드 번호
	 * @param pclsMessage SIP 응답 메시지
	 * @returns SIP 응답 메시지를 처리하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
	 */
	virtual bool RecvResponse( int iThreadId, CSipMessage * pclsMessage ) = 0;

	/**
   * @ingroup SipStack
	 * @brief SIP 메시지 전송 timeout 이벤트 핸들러
	 * @param iThreadId		UDP 쓰레드 번호
	 * @param pclsMessage SIP 응답 메시지
	 * @returns SIP 응답 메시지를 처리하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
	 */
	virtual bool SendTimeout( int iThreadId, CSipMessage * pclsMessage ) = 0;
};

class ISipStackSecurityCallBack
{
public:
	virtual ~ISipStackSecurityCallBack(){};

	virtual bool IsAllowUserAgent( const char * pszSipUserAgent ) = 0;
	virtual bool IsDenyUserAgent( const char * pszSipUserAgent ) = 0;

	virtual bool IsAllowIp( const char * pszIp ) = 0;
};

#endif
