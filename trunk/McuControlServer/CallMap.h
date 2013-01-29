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
#include "SipMessage.h"

class CCallInfo
{
public:
	CCallInfo() : m_bRecvFromDevice(false), m_bInviteToMCU(false)
	{}

	std::string	m_strRoomId;
	std::string m_strToId;
	std::string m_strToTag;
	CSipMessage m_clsSipMessage;
	bool				m_bRecvFromDevice;
	bool				m_bInviteToMCU;
};

// key = SIP Call-Id
typedef std::map< std::string, CCallInfo > CALL_MAP;

class CCallMap
{
public:
	CCallMap();
	~CCallMap();

	bool Insert( CSipMessage * pclsMessage );
	bool Insert( CSipMessage * pclsMessage, std::string & strRoomId );
	bool Update( CSipMessage * pclsMessage );
	bool Delete( CSipMessage * pclsMessage );
	bool Select( CSipMessage * pclsMessage, CCallInfo & clsCallInfo );

	bool UpdateToTag( CSipMessage * pclsMessage );

private:
	CALL_MAP	m_clsMap;
	CSipMutex	m_clsMutex;
};

extern CCallMap gclsCallMap;

#endif
