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

// key = IP 주소 + 사용자 아이디
typedef std::map< std::string, CXmlSipServer > SIP_SERVER_MAP;

/**
 * @ingroup KSipServer
 * @brief IP-PBX 정보 저장하는 자료구조
 */
class CSipServerMap
{
public:
	CSipServerMap();
	~CSipServerMap();

	bool ReadDir( const char * pszDirName );
	bool SetSipUserAgentRegisterInfo( );

	bool SelectRoutePrefix( const char * pszTo, CXmlSipServer & clsXmlSipServer, std::string & strTo );

private:
	SIP_SERVER_MAP	m_clsMap;
	CSipMutex				m_clsMutex;

	void GetKey( CXmlSipServer & clsXmlSipServer, std::string & strKey );
	bool Insert( CXmlSipServer & clsXmlSipServer );
};

extern CSipServerMap gclsSipServerMap;

#endif
