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

#ifndef _ROOM_MAP_H_
#define _ROOM_MAP_H_

#include <map>
#include <list>
#include <string>
#include "SipMutex.h"

typedef std::list< std::string > CALL_ID_LIST;

class CRoomInfo
{
public:
	void Insert( const char * pszCallId )
	{
		CALL_ID_LIST::iterator	itList;
		bool	bFound = false;

		for( itList = m_clsCallIdList.begin(); itList != m_clsCallIdList.end(); ++itList )
		{
			if( !strcmp( pszCallId, itList->c_str() ) )
			{
				bFound = true;
				break;
			}
		}

		if( bFound == false )
		{
			m_clsCallIdList.push_back( pszCallId );
		}
	}

	void Delete( const char * pszCallId )
	{
		CALL_ID_LIST::iterator	itList;

		for( itList = m_clsCallIdList.begin(); itList != m_clsCallIdList.end(); ++itList )
		{
			if( !strcmp( pszCallId, itList->c_str() ) )
			{
				m_clsCallIdList.erase( itList );
				break;
			}
		}
	}

	CALL_ID_LIST	m_clsCallIdList;
};

// key 는 회의방 room id 이다.
typedef std::map< std::string, CRoomInfo > ROOM_MAP;

class CRoomMap
{
public:
	CRoomMap();
	~CRoomMap();

	bool Insert( const char * pszRoomId );
	bool Insert( const char * pszRoomId, const char * pszCallId );
	bool Select( const char * pszRoomId );
	bool Select( const char * pszRoomId, CRoomInfo & clsRoomInfo );
	bool Delete( const char * pszRoomId, const char * pszCallId );

private:
	CSipMutex	m_clsMutex;
	ROOM_MAP	m_clsMap;
};

extern CRoomMap gclsRoomMap;

#endif
