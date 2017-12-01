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

#ifndef _SIMPLE_HTTP_SERVER_H_
#define _SIMPLE_HTTP_SERVER_H_

#include "HttpStack.h"
#include "StringUtility.h"

/**
 * @ingroup TestHttpStack
 * @brief HTTP 요청 및 WebSocket callback 클래스
 */
class CWebRtcServer : public IHttpStackCallBack
{
public:
	CWebRtcServer();
	virtual ~CWebRtcServer();

	virtual bool RecvHttpRequest( CHttpMessage * pclsRequest, CHttpMessage * pclsResponse );

	virtual void WebSocketConnected( const char * pszClientIp, int iClientPort );
	virtual void WebSocketClosed( const char * pszClientIp, int iClientPort );
	virtual bool WebSocketData( const char * pszClientIp, int iClientPort, std::string & strData );

	std::string m_strDocumentRoot;
	bool m_bStop;

private:
	bool Send( const char * pszClientIp, int iClientPort, const char * fmt, ... );
	bool SendCall( const char * pszClientIp, int iClientPort, std::string & strData, std::string & strUserId );
};

#endif
