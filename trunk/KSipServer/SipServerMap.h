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

#ifndef _SIP_SERVER_MAP_H_
#define _SIP_SERVER_MAP_H_

#include "XmlSipServer.h"
#include "SipMutex.h"
#include <map>

// key = IP �ּ� + ����� ���̵�
typedef std::map< std::string, CXmlSipServer > SIP_SERVER_MAP;

/**
 * @ingroup KSipServer
 * @brief IP-PBX ���� �����ϴ� �ڷᱸ��
 */
class CSipServerMap
{
public:
	CSipServerMap();
	~CSipServerMap();

	bool Load( );

	bool SetSipUserAgentRegisterInfo( );

	bool Select( const char * pszIp, const char * pszUserId );
	bool SelectRoutePrefix( const char * pszTo, CXmlSipServer & clsXmlSipServer, std::string & strTo );
	bool SelectIncomingRoute( const char * pszIp, const char * pszTo, std::string & strTo );

	bool Insert( CXmlSipServer & clsXmlSipServer );
	bool Set( CSipServerInfo * pclsInfo, int iStatus );

	void GetString( CMonitorString & strBuf );

private:
	SIP_SERVER_MAP	m_clsMap;
	CSipMutex				m_clsMutex;

	bool ReadDir( const char * pszDirName );
	void GetKey( CXmlSipServer & clsXmlSipServer, std::string & strKey );
	void GetKey( const char * pszIp, const char * pszUserId, std::string & strKey );
	void ReSetFlag( );
};

extern CSipServerMap gclsSipServerMap;

#endif
