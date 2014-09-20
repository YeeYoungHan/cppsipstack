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

#include "RoomMap.h"

CRoomMap gclsRoomMap;

CRoomMap::CRoomMap()
{
}

CRoomMap::~CRoomMap()
{
}

bool CRoomMap::Insert( const char * pszRoomId )
{
	ROOM_MAP::iterator	itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszRoomId );
	if( itMap == m_clsMap.end() )
	{
		CRoomInfo	clsInfo;

		m_clsMap.insert( ROOM_MAP::value_type( pszRoomId, clsInfo ) );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

bool CRoomMap::Insert( const char * pszRoomId, const char * pszCallId )
{
	ROOM_MAP::iterator	itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszRoomId );
	if( itMap == m_clsMap.end() )
	{
		CRoomInfo	clsInfo;

		clsInfo.Insert( pszCallId );

		m_clsMap.insert( ROOM_MAP::value_type( pszRoomId, clsInfo ) );
		bRes = true;
	}
	else
	{
		itMap->second.Insert( pszCallId );
	}
	m_clsMutex.release();

	return bRes;
}

bool CRoomMap::Select( const char * pszRoomId )
{
	ROOM_MAP::iterator	itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszRoomId );
	if( itMap != m_clsMap.end() )
	{
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

bool CRoomMap::Select( const char * pszRoomId, CRoomInfo & clsRoomInfo )
{
	ROOM_MAP::iterator	itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszRoomId );
	if( itMap != m_clsMap.end() )
	{
		clsRoomInfo = itMap->second;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

bool CRoomMap::Delete( const char * pszRoomId, const char * pszCallId )
{
	ROOM_MAP::iterator	itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszRoomId );
	if( itMap != m_clsMap.end() )
	{
		itMap->second.Delete( pszCallId );

		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}
