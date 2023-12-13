/* 
 * Copyright (C) 2021 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
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

#include "SipMessage.h"
#include <map>

class CCallInfo
{
public:
	std::string m_strRecvCallId;
	std::string m_strBranch;
};

// key = 로그 파일에서 읽은 SIP Call-ID
typedef std::map< std::string, CCallInfo > CALL_MAP;

class CCallMap
{
public:
	void Insert( std::string & strCallId, CSipMessage & clsMessage );
	bool Select( std::string & strCallId, CCallInfo & clsCallInfo );

	CALL_MAP m_clsMap;
};

extern CCallMap gclsCallMap;

#endif
