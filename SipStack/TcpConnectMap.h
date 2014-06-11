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

typedef std::list< CSipMessage * > SIP_MESSAGE_LIST;

class CTcpConnectInfo
{
public:
	SIP_MESSAGE_LIST	m_clsSipMessageList;
};

// key = ip:port
typedef std::map< std::string, CTcpConnectInfo > TCP_CONNECT_MAP;

class CTcpConnectMap
{
public:
	CTcpConnectMap();
	~CTcpConnectMap();

	bool Insert( const char * pszIp, int iPort );
	bool Insert( const char * pszIp, int iPort, CSipMessage * pclsMessage );
	bool Delete( const char * pszIp, int iPort, SIP_MESSAGE_LIST & clsList );
	bool Delete( const char * pszIp, int iPort );

private:
	TCP_CONNECT_MAP m_clsMap;
	CSipMutex				m_clsMutex;

	void GetKey( const char * pszIp, int iPort, std::string & strKey );
};

#endif
