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

#ifndef _TCP_SOCKET_MAP_H_
#define _TCP_SOCKET_MAP_H_

#include "SipTcp.h"
#include <string>
#include <map>
#include "SipMutex.h"
#include "TlsFunction.h"
#include "MonitorString.h"

/**
 * @ingroup SipStack
 * @brief TCP/TLS 세션 정보
 */
class CTcpSocketInfo
{
public:
	CTcpSocketInfo();

	Socket	m_hSocket;

#ifdef USE_TLS
	SSL			* m_psttSsl;
#endif
};

typedef std::map< std::string, CTcpSocketInfo > TCP_SOCKET_MAP;

/**
 * @ingroup SipStack
 * @brief TCP/TLS 세션 정보를 저장하는 자료구조
 */
class CTcpSocketMap
{
public:
	CTcpSocketMap(void);
	~CTcpSocketMap(void);

	bool Insert( const char * pszIp, int iPort, Socket hSocket, SSL * psttSsl = NULL );
	bool Select( const char * pszIp, int iPort, Socket & hSocket );

#ifdef USE_TLS
	bool SendTls( const char * pszIp, int iPort, CSipMessage * pclsMessage );
	bool SendTls( const char * pszIp, int iPort, const char * pszMessage, int iMessageSize );
#endif

	bool Delete( const char * pszIp, int iPort );
	void DeleteAll( );

	void GetString( CMonitorString & strBuf );

private:
	TCP_SOCKET_MAP	m_clsMap;
	CSipMutex				m_clsMutex;

	void GetKey( const char * pszIp, int iPort, std::string & strKey );
};

#endif
