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

#ifndef _NONCE_MAP_H_
#define _NONCE_MAP_H_

#include <stdio.h>
#include <map>
#include <string>
#include "SipMutex.h"

#ifndef WIN32
#include <sys/time.h>
#endif

#define PRIVATE_KEY	"hotyoungsipserver"

/** 
 * @ingroup KSipServer
 * @brief nonce 정보를 저장하는 클래스
 */
class CNonceInfo
{
public:
	/** nonce 저장 시간 */
	time_t	m_iTime;
};

typedef std::map< std::string, CNonceInfo >	NONCE_MAP;

/**
 * @ingroup KSipServer
 * @brief nonce 를 관리하는 클래스
 */
class CNonceMap
{
public:
	CNonceMap();

	bool GetNewValue( char * pszNonce, int iNonceSize );
	bool Select( const char * pszNonce, bool bIsRemove = true );
	void DeleteTimeout( int iSecond );
	int GetCount();

private:
	NONCE_MAP	m_clsMap;
	CSipMutex	m_clsMutex;
};

extern CNonceMap	gclsNonceMap;

#endif
