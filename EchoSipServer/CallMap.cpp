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

#include "CallMap.h"

CCallMap gclsCallMap;

bool CCallMap::Insert( const char * pszCallId1, const char * pszCallId2 )
{
	CALL_MAP::iterator itMap;
	bool bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId1 );
	if( itMap == m_clsMap.end() )
	{
		itMap = m_clsMap.find( pszCallId2 );
		if( itMap == m_clsMap.end() )
		{
			CCallInfo clsInfo;

			clsInfo.m_strCallId = pszCallId2;
			m_clsMap.insert( CALL_MAP::value_type( pszCallId1, clsInfo ) );

			clsInfo.m_strCallId = pszCallId1;
			m_clsMap.insert( CALL_MAP::value_type( pszCallId2, clsInfo ) );

			bRes = true;
		}
	}
	m_clsMutex.release();

	return bRes;
}

bool CCallMap::Select( const char * pszCallId1, std::string & strCallId2 )
{
	CALL_MAP::iterator itMap;
	bool bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId1 );
	if( itMap != m_clsMap.end() )
	{
		strCallId2 = itMap->second.m_strCallId;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

bool CCallMap::Delete( const char * pszCallId )
{
	CALL_MAP::iterator itMap;
	bool bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		std::string strCallId2 = itMap->second.m_strCallId;
		m_clsMap.erase( itMap );

		itMap = m_clsMap.find( strCallId2 );
		if( itMap != m_clsMap.end() )
		{
			m_clsMap.erase( itMap );
		}
	}
	m_clsMutex.release();

	return bRes;
}
