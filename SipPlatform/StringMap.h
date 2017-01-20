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

#ifndef _STRING_MAP_H_
#define _STRING_MAP_H_

#include <string>
#include <map>
#include "SipMutex.h"

typedef std::map< std::string, std::string > STRING_MAP;

/**
 * @ingroup SipPlatform
 * @brief 문자열 맵 자료구조 클래스
 */
class CStringMap
{
public:
	CStringMap();
	~CStringMap();

	bool Insert( const char * pszKey, const char * pszValue );
	bool Select( const char * pszKey );
	bool Select( const char * pszKey, std::string & strValue );
	bool Delete( const char * pszKey );
	int GetCount( );
	void DeleteAll( );

private:
	CSipMutex	m_clsMutex;
	STRING_MAP	m_clsMap;
};

#endif
