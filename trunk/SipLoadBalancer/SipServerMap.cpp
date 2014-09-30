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
#include "SipServerMap.h"
#include "UserMap.h"
#include "MemoryDebug.h"

CSipServerMap gclsSipServerMap;

CSipServerInfo::CSipServerInfo() : m_iPort(5060), m_bUse(true), m_bDelete(false)
{
}

/**
 * @ingroup SipLoadBalancer
 * @brief �Էµ� IP �ּ� �� ��Ʈ ��ȣ�� SIP ���� ������ ��ġ�ϴ��� �˻��Ѵ�.
 * @param pszIp IP �ּ�
 * @param iPort ��Ʈ ��ȣ
 * @returns �Է� ������ SIP ���� ������ ��ġ�ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipServerInfo::Equal( const char * pszIp, int iPort )
{
	if( !strcmp( m_strIp.c_str(), pszIp ) && m_iPort == iPort ) return true;

	return false;
}

CSipServerMap::CSipServerMap() : m_iIndex(0)
{
}

CSipServerMap::~CSipServerMap()
{
}

/**
 * @ingroup SipLoadBalancer
 * @brief SIP ���� ������ ����Ʈ�� �߰��Ѵ�.
 * @param pszIp IP �ּ�
 * @param iPort ��Ʈ ��ȣ
 * @param bUse	��� ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipServerMap::Insert( const char * pszIp, int iPort, bool bUse )
{
	if( pszIp == NULL || strlen(pszIp) == 0 ) return false;
	if( iPort <= 0 || iPort > 65535 ) return false;

	// �̹� ����Ʈ�� �����ϸ� �߰����� �ʴ´�.
	if( Select( pszIp, iPort, bUse ) ) return true;

	CSipServerInfo clsInfo;

	clsInfo.m_strIp = pszIp;
	clsInfo.m_iPort = iPort;
	clsInfo.m_bUse = bUse;
	
	m_clsMutex.acquire();
	m_clsList.push_back( clsInfo );
	m_clsMutex.release();

	return true;
}

/**
 * @ingroup SipLoadBalancer
 * @brief ������ SIP ���� ������ �����´�.
 * @param clsInfo SIP ���� ������ �����ϴ� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
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

/**
 * @ingroup SipLoadBalancer
 * @brief �Էµ� IP �ּ�, ��Ʈ��ȣ�� ��ġ�ϴ� SIP ���� ������ �����ϴ��� �˻��Ѵ�.
 * @param pszIp IP �ּ�
 * @param iPort ��Ʈ ��ȣ
 * @returns SIP ������ �˻��Ǹ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipServerMap::Select( const char * pszIp, int iPort )
{
	SIP_SERVER_LIST::iterator itList;
	bool bFound = false;

	m_clsMutex.acquire();
	for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
	{
		if( itList->Equal( pszIp, iPort ) )
		{
			bFound = true;
			break;
		}
	}
	m_clsMutex.release();

	return bFound;
}

/**
 * @ingroup SipLoadBalancer
 * @brief �Էµ� IP �ּ�, ��Ʈ��ȣ�� ��ġ�ϴ� SIP ���� ������ �����ϸ� ��� ���� �� ���� ������ ������Ʈ�Ѵ�.
 * @param pszIp IP �ּ�
 * @param iPort ��Ʈ ��ȣ
 * @param bUse	��� ����
 * @returns SIP ������ �˻��Ǹ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipServerMap::Select( const char * pszIp, int iPort, bool bUse )
{
	SIP_SERVER_LIST::iterator itList;
	bool bFound = false;

	m_clsMutex.acquire();
	for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
	{
		if( itList->Equal( pszIp, iPort ) )
		{
			itList->m_bUse = bUse;
			itList->m_bDelete = false;
			bFound = true;
			break;
		}
	}
	m_clsMutex.release();

	return bFound;
}

/**
 * @ingroup SipLoadBalancer
 * @brief ��� SIP ���� ������ ���� ǥ�ø� �Ѵ�.
 */
void CSipServerMap::SetDeleteAll( )
{
	SIP_SERVER_LIST::iterator itList;

	m_clsMutex.acquire();
	for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
	{
		itList->m_bDelete = true;
	}
	m_clsMutex.release();
}

/**
 * @ingroup SipLoadBalancer
 * @brief ���� ǥ�ð� �� SIP ������ �����Ѵ�.
 */
void CSipServerMap::DeleteIfSet( )
{
	SIP_SERVER_LIST::iterator itList, itNext;

	m_clsMutex.acquire();
	for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
	{
LOOP_START:
		if( itList->m_bDelete == false ) 
		{
			if( itList->m_bUse == false )
			{
				gclsUserMap.DeleteSipServer( itList->m_strIp.c_str(), itList->m_iPort );
			}
			continue;
		}

		itNext = itList;
		++itNext;

		gclsUserMap.DeleteSipServer( itList->m_strIp.c_str(), itList->m_iPort );
		m_clsList.erase( itList );

		if( itNext == m_clsList.end() ) break;

		itList = itNext;
		goto LOOP_START;
	}
	m_clsMutex.release();
}

/**
 * @ingroup SipLoadBalancer
 * @brief �ڷᱸ�� ����͸��� ���ڿ��� �����Ѵ�. 
 * @param strBuf �ڷᱸ�� ����͸��� ���ڿ� ����
 */
void CSipServerMap::GetString( CMonitorString & strBuf )
{
	SIP_SERVER_LIST::iterator itList;

	strBuf.Clear();

	m_clsMutex.acquire();
	for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
	{
		strBuf.AddCol( itList->m_strIp, itList->m_iPort );
		strBuf.AddCol( itList->m_bUse ? "use" : "no use" );
		strBuf.AddRow( itList->m_bDelete ? "del" : "" );
	}
	m_clsMutex.release();
}
