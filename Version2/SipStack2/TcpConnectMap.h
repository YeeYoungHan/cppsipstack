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

#ifndef _TCP_CONNECT_MAP_H_
#define _TCP_CONNECT_MAP_H_

#include "SipMessage.h"
#include "SipMutex.h"
#include <map>
#include "MonitorString.h"

typedef std::list< CSipMessage * > SIP_MESSAGE_LIST;

/**
 * @ingroup SipStack
 * @brief TCP/TLS 연결 진행 중인 경우에 전송 요청한 SIP 메시지를 저장하는 클래스
 */
class CTcpConnectInfo
{
public:
	CTcpConnectInfo();

	SIP_MESSAGE_LIST	m_clsSipMessageList;
	time_t						m_iCreateTime;
};

// key = ip:port
typedef std::map< std::string, CTcpConnectInfo > TCP_CONNECT_MAP;

/**
 * @ingroup SipStack
 * @brief TCP/TLS 연결 진행 중인 정보를 저장하는 클래스
 */
class CTcpConnectMap
{
public:
	CTcpConnectMap();
	~CTcpConnectMap();

	bool Insert( const char * pszIp, int iPort );
	bool Insert( const char * pszIp, int iPort, CSipMessage * pclsMessage );
	bool Delete( const char * pszIp, int iPort, SIP_MESSAGE_LIST & clsList );
	bool Delete( const char * pszIp, int iPort );
	int GetSize();
	void SetStateful( bool bStateful );

	void GetString( CMonitorString & strBuf );

private:
	TCP_CONNECT_MAP m_clsMap;
	CSipMutex				m_clsMutex;
	bool						m_bStateful;

	void GetKey( const char * pszIp, int iPort, std::string & strKey );
};

#endif
