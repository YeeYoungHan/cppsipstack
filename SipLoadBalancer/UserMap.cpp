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

#include "UserMap.h"
#include "SipServerMap.h"

CUserMap gclsUserMap;

CUserMap::CUserMap()
{
}

CUserMap::~CUserMap()
{
}

bool CUserMap::Select( CSipMessage * pclsRequest, CUserInfo & clsUserInfo )
{
	bool								bRes = true;
	USER_MAP::iterator	itMap;
	std::string & strUserId = pclsRequest->m_clsFrom.m_clsUri.m_strUser;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strUserId );
	if( itMap == m_clsMap.end() )
	{
		CSipServerInfo clsSipServerInfo;

		if( gclsSipServerMap.SelectNext( clsSipServerInfo ) )
		{
			clsUserInfo.m_strSipServerIp = clsSipServerInfo.m_strIp;

			m_clsMap.insert( USER_MAP::value_type( strUserId, clsUserInfo ) );
		}
		else
		{
			bRes = false;
		}
	}
	else
	{
		clsUserInfo = itMap->second;
	}
	m_clsMutex.release();

	return bRes;
}
