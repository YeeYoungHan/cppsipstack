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

#ifndef _HTTP_STACK_CALLBACK_H_
#define _HTTP_STACK_CALLBACK_H_

#include "HttpMessage.h"

class CHttpStackSession;

/**
 * @ingroup HttpStack
 * @brief HTTP 서버 callback 인터페이스
 */
class IHttpStackCallBack
{
public:
	IHttpStackCallBack(){};
	virtual ~IHttpStackCallBack(){};

	/**
	 * @ingroup HttpStack
	 * @brief HTTP 요청 수신 이벤트 callback
	 * @param pclsRequest		HTTP 요청 메시지
	 * @param pclsResponse	HTTP 응답 메시지 - 응용에서 저장한다.
	 * @returns 응용에서 HTTP 응답 메시지를 정상적으로 생성하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
	 */
	virtual bool RecvHttpRequest( CHttpMessage * pclsRequest, CHttpMessage * pclsResponse ) = 0;

	/**
	 * @ingroup HttpStack
	 * @brief WebSocket 클라이언트 TCP 연결 시작 이벤트 callback
	 * @param pszClientIp WebSocket 클라이언트 IP 주소
	 * @param iClientPort WebSocket 클라이언트 포트 번호
	 */
	virtual void WebSocketConnected( const char * pszClientIp, int iClientPort ) = 0;

	/**
	 * @ingroup HttpStack
	 * @brief WebSocket 클라이언트 TCP 연결 종료 이벤트 callback
	 * @param pszClientIp WebSocket 클라이언트 IP 주소
	 * @param iClientPort WebSocket 클라이언트 포트 번호
	 */
	virtual void WebSocketClosed( const char * pszClientIp, int iClientPort ) = 0;

	/**
	 * @ingroup HttpStack
	 * @brief WebSocket 클라이언트 데이터 수신 이벤트 callback
	 * @param pszClientIp WebSocket 클라이언트 IP 주소
	 * @param iClientPort WebSocket 클라이언트 포트 번호
	 * @param strData			WebSocket 클라이언트가 전송한 데이터
	 * @param pclsSession	HTTP 세션 정보
	 * @returns WebSocket 클라이언트 연결을 유지하려면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
	 */
	virtual bool WebSocketData( const char * pszClientIp, int iClientPort, std::string & strData, CHttpStackSession * pclsSession ) = 0;
};

#endif
