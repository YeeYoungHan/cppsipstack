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
#include <time.h>

CUserMap gclsUserMap;

CUserMap::CUserMap()
{
}

CUserMap::~CUserMap()
{
}

/**
 * @ingroup KSipServer
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
	clsInfo.m_iLoginTimeout = pclsMessage->GetExpires();

	if( clsInfo.m_iLoginTimeout == 0 )
	{
		return false;
	}

	time( &clsInfo.m_iLoginTime );

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
 * @ingroup KSipServer
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

/**
 * @ingroup KSipServer
 * @brief ����� ���̵� �����ϴ��� �˻��Ѵ�.
 * @param pszUserId ����� ���̵�
 * @returns ����� ���̵� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CUserMap::Select( const char * pszUserId )
{
	bool bRes = false;
	USER_MAP::iterator	itMap;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszUserId );
	if( itMap != m_clsMap.end() )
	{
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief ����� ���̵� �ڷᱸ������ �����Ѵ�.
 * @param pszUserId ����� ���̵�
 * @returns ����� ���̵� �ڷᱸ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
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

/**
 * @ingroup KSipServer
 * @brief ����� ����ڸ� �ڷᱸ������ �����Ѵ�.
 * @param iTimeout ����� �ð� ���Ŀ� ��� �ð� (�ʴ���)
 */
void CUserMap::DeleteTimeout( int iTimeout )
{
	USER_MAP::iterator	itMap, itNext;
	time_t iTime;

	time( &iTime );

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
LOOP_START:
		if( iTime > ( itMap->second.m_iLoginTime + itMap->second.m_iLoginTimeout + iTimeout ) )
		{
			itNext = itMap;
			++itNext;

			CLog::Print( LOG_DEBUG, "user(%s) is deleted - timeout", itMap->first.c_str() );

			m_clsMap.erase( itMap );
			if( itNext == m_clsMap.end() ) break;
			itMap = itNext;
			goto LOOP_START;
		}
	}
	m_clsMutex.release();
}
