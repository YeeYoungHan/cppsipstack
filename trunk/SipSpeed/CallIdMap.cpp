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

#include "StdAfx.h"
#include "CallIdMap.h"

CCallIdMap gclsCallIdMap;

CCallIdMap::CCallIdMap()
{
}

CCallIdMap::~CCallIdMap()
{
}

bool CCallIdMap::Insert( const char * pszCallId )
{
	CALL_ID_MAP::iterator	it;
	bool bRes = false;

	m_clsMutex.acquire();
	it = m_clsMap.find( pszCallId );
	if( it == m_clsMap.end() )
	{
		CCallIdInfo clsInfo;

		m_clsMap.insert( CALL_ID_MAP::value_type( pszCallId, clsInfo ) );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

bool CCallIdMap::Delete( const char * pszCallId )
{
	CALL_ID_MAP::iterator	it;
	bool bRes = false;

	m_clsMutex.acquire();
	it = m_clsMap.find( pszCallId );
	if( it != m_clsMap.end() )
	{
		m_clsMap.erase( it );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

void CCallIdMap::DeleteAll( )
{
	m_clsMutex.acquire();
	m_clsMap.clear();
	m_clsMutex.release();
}

int CCallIdMap::GetCount()
{
	int iCount;

	m_clsMutex.acquire();
	iCount = m_clsMap.size();
	m_clsMutex.release();

	return iCount;
}
