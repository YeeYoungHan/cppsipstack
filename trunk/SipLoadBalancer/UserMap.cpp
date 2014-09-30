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

#include "SipStackDefine.h"
#include "UserMap.h"
#include "SipServerMap.h"
#include "MemoryDebug.h"

CUserMap gclsUserMap;

CUserInfo::CUserInfo() : m_iSipServerPort(5060), m_iPort(0), m_eTransport(E_SIP_UDP)
{
}

/**
 * @brief �Էµ� SIP ���� ������ ��ġ�ϴ��� �˻��Ѵ�.
 * @param pszIp SIP ���� IP �ּ�
 * @param iPort SIP ���� ��Ʈ ��ȣ
 * @returns ��ġ�ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CUserInfo::EqualSipServer( const char * pszIp, int iPort )
{
	if( !strcmp( m_strSipServerIp.c_str(), pszIp ) && m_iSipServerPort == iPort ) return true;

	return false;
}

CUserMap::CUserMap()
{
}

CUserMap::~CUserMap()
{
}

/**
 * @ingroup SipLoadBalancer
 * @brief SIP �޽����� SIP Ŭ���̾�Ʈ ������ �����Ѵ�.
 * @param pclsRequest SIP �޽���
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CUserMap::Insert( CSipMessage * pclsRequest )
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
			CUserInfo clsUserInfo;

			clsUserInfo.m_strSipServerIp = clsSipServerInfo.m_strIp;
			clsUserInfo.m_iSipServerPort = clsSipServerInfo.m_iPort;
			clsUserInfo.m_strIp = pclsRequest->m_strClientIp;
			clsUserInfo.m_iPort = pclsRequest->m_iClientPort;
			clsUserInfo.m_eTransport = pclsRequest->m_eTransport;

			m_clsMap.insert( USER_MAP::value_type( strUserId, clsUserInfo ) );
		}
		else
		{
			bRes = false;
		}
	}
	else
	{
		if( strcmp( pclsRequest->m_strClientIp.c_str(), itMap->second.m_strIp.c_str() ) )
		{
			itMap->second.m_strIp = pclsRequest->m_strClientIp;
		}

		if( pclsRequest->m_iClientPort != itMap->second.m_iPort )
		{
			itMap->second.m_iPort = pclsRequest->m_iClientPort;
		}
	}
	m_clsMutex.release();

	return bRes;
}


/**
 * @ingroup SipLoadBalancer
 * @brief SIP �޽����� ���õ� SIP Ŭ���̾�Ʈ ������ �����´�.
 * @param pclsRequest SIP �޽���
 * @param clsUserInfo SIP Ŭ���̾�Ʈ ���� ���� ����
 * @returns �˻��Ǹ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CUserMap::Select( CSipMessage * pclsRequest, CUserInfo & clsUserInfo )
{
	bool								bRes = true;
	USER_MAP::iterator	itMap;
	std::string & strUserId = pclsRequest->m_clsTo.m_clsUri.m_strUser;

	if( pclsRequest->IsMethod( SIP_METHOD_REGISTER ) )
	{
		strUserId = pclsRequest->m_clsFrom.m_clsUri.m_strUser;
	}

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strUserId );
	if( itMap == m_clsMap.end() )
	{
		bRes = false;
	}
	else
	{
		clsUserInfo = itMap->second;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipLoadBalancer
 * @brief SIP ������ IP �ּ�/��Ʈ ��ȣ�� ������ �ִ� SIP Ŭ���̾�Ʈ�� �����Ѵ�.
 * @param pszIp SIP ���� IP �ּ�
 * @param iPort SIP ���� ��Ʈ ��ȣ
 */
void CUserMap::DeleteSipServer( const char * pszIp, int iPort )
{
	USER_MAP::iterator	itMap, itNext;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
LOOP_START:
		if( itMap->second.EqualSipServer( pszIp, iPort ) == false ) continue;

		itNext = itMap;
		++itNext;

		m_clsMap.erase( itMap );

		if( itNext == m_clsMap.end() ) break;
		itMap = itNext;

		goto LOOP_START;
	}
	m_clsMutex.release();
}

/**
 * @ingroup SipLoadBalancer
 * @brief �ڷᱸ�� ����͸��� ���ڿ��� �����Ѵ�. 
 * @param strBuf �ڷᱸ�� ����͸��� ���ڿ� ����
 */
void CUserMap::GetString( CMonitorString & strBuf )
{
	USER_MAP::iterator	itMap;

	strBuf.Clear();

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		strBuf.AddCol( itMap->first );
		strBuf.AddCol( itMap->second.m_strIp, itMap->second.m_iPort );
		strBuf.AddRow( itMap->second.m_strSipServerIp, itMap->second.m_iSipServerPort );
	}
	m_clsMutex.release();
}
