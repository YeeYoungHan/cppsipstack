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
#include "Log.h"

CUserMap gclsUserMap;

CUserMap::CUserMap()
{
}

CUserMap::~CUserMap()
{
}

/**
 * @ingroup SimpleSipServer
 * @brief �α��ε� Ŭ���̾�Ʈ ������ �����Ѵ�.
 * @param pclsMessage SIP REGISTER �޽���
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CUserMap::Insert( CSipMessage * pclsMessage )
{
	CUserInfo			clsInfo;
	std::string		strUserId;
	USER_MAP::iterator	itMap;

	strUserId = pclsMessage->m_clsFrom.m_clsUri.m_strUser;
	if( strUserId.empty() ) return false;

	if( pclsMessage->GetTopViaIpPort( clsInfo.m_strIp, clsInfo.m_iPort ) == false ) return false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strUserId );
	if( itMap == m_clsMap.end() )
	{
		m_clsMap.insert( USER_MAP::value_type( strUserId, clsInfo ) );
		CLog::Print( LOG_DEBUG, "user(%s) is inserted", strUserId.c_str() );
	}
	else
	{
		itMap->second = clsInfo;
	}
	m_clsMutex.release();

	return true;
}

/**
 * @ingroup SimpleSipServer
 * @brief ����� ID �� �ش��ϴ� ������ �˻��Ѵ�.
 * @param pszUserId ����� ID
 * @param clsInfo		����� ������ ������ ����
 * @returns ����� ID �� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CUserMap::Select( const char * pszUserId, CUserInfo & clsInfo )
{
	bool bRes = false;
	USER_MAP::iterator	itMap;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszUserId );
	if( itMap != m_clsMap.end() )
	{
		clsInfo = itMap->second;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

bool CUserMap::Delete( const char * pszUserId )
{
	bool bRes = false;
	USER_MAP::iterator	itMap;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszUserId );
	if( itMap != m_clsMap.end() )
	{
		m_clsMap.erase( itMap );
		CLog::Print( LOG_DEBUG, "user(%s) is deleted", pszUserId );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}
