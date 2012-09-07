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

#ifndef _CALL_MAP_H_
#define _CALL_MAP_H_

#include <string>
#include <map>
#include "SipMutex.h"

class CCallInfo
{
public:
	CCallInfo();

	std::string	m_strPeerCallId;
	bool				m_bRecv;
};

/**
 * @ingroup KSipServer
 * @brief 연결된 통화 정보를 저장하는 자료구조. key 와 value 는 SIP Call-ID 이다.
 */
typedef std::map< std::string, CCallInfo > CALL_MAP;

/**
 * @ingroup KSipServer
 * @brief 연결된 통화 정보를 저장하는 자료구조 클래스
 */
class CCallMap
{
public:
	CCallMap();
	~CCallMap();

	bool Insert( const char * pszRecvCallId, const char * pszSendCallId );
	bool Select( const char * pszCallId, std::string & strCallId );
	bool Select( const char * pszCallId, CCallInfo & clsCallInfo );
	bool Delete( const char * pszCallId );

	void StopCallAll();
	int GetCount();

private:
	CALL_MAP	m_clsMap;
	CSipMutex	m_clsMutex;
};

extern CCallMap gclsCallMap;
extern CCallMap gclsTransCallMap;

#endif
