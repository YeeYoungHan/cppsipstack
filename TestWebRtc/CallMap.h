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

// key = caller user id, value = callee user id
typedef std::map< std::string, std::string > CALL_MAP;

/**
 * @ingroup TestWebRtc
 * @brief 통화 정보 저장 자료구조 클래스
 */
class CCallMap
{
public:
	CCallMap();
	~CCallMap();

	bool Insert( const char * pszCaller, const char * pszCallee );
	bool Select( const char * pszUserId, std::string & strOtherId );
	bool Delete( const char * pszUserId );

private:
	CALL_MAP	m_clsMap;
	CSipMutex	m_clsMutex;
};

extern CCallMap gclsCallMap;

#endif
