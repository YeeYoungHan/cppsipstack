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

#include "SipServerMap.h"

CSipServerMap gclsSipServerMap;

CSipServerMap::CSipServerMap() : m_iIndex(0)
{
}

CSipServerMap::~CSipServerMap()
{
}

bool CSipServerMap::Insert( const char * pszIp )
{
	CSipServerInfo clsInfo;

	clsInfo.m_strIp = pszIp;
	
	m_clsMutex.acquire();
	m_clsList.push_back( clsInfo );
	m_clsMutex.release();

	return true;
}

bool CSipServerMap::SelectNext( CSipServerInfo & clsInfo )
{
	bool bRes = false;

	m_clsMutex.acquire();
	int iCount = (int)m_clsList.size();
	if( iCount > 0 )
	{
		if( m_iIndex >= iCount )
		{
			m_iIndex = 0;
		}

		clsInfo = m_clsList[m_iIndex];
		++m_iIndex;

		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}
