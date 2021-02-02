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

#include "TcpConnectMap.h"
#include "TimeUtility.h"
#include "Log.h"
#include "MemoryDebug.h"

CTcpConnectInfo::CTcpConnectInfo() : m_iCreateTime(0)
{
}

CTcpConnectMap::CTcpConnectMap()
{
}

CTcpConnectMap::~CTcpConnectMap()
{
}

/**
 * @ingroup SipStack
 * @brief TCP ������ ������ ������ �����Ѵ�.
 * @param pszIp TCP ���� IP �ּ�
 * @param iPort TCP ���� ��Ʈ ��ȣ
 * @returns ���忡 �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpConnectMap::Insert( const char * pszIp, int iPort )
{
	bool bRes = false;
	TCP_CONNECT_MAP::iterator	itMap;
	std::string strKey;

	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap == m_clsMap.end() )
	{
		CTcpConnectInfo clsInfo;

		time( &clsInfo.m_iCreateTime );
		m_clsMap.insert( TCP_CONNECT_MAP::value_type( strKey, clsInfo ) );
		bRes = true;
	}
	else
	{
		time_t iTime;

		time( &iTime );

		if( iTime >= ( itMap->second.m_iCreateTime + 120 ) )
		{
			CLog::Print( LOG_ERROR, "%s connect(%s:%d) too long time", __FUNCTION__, pszIp, iPort );

			SIP_MESSAGE_LIST::iterator	itList;

			for( itList = itMap->second.m_clsSipMessageList.begin(); itList != itMap->second.m_clsSipMessageList.end(); ++itList )
			{
				--(*itList)->m_iUseCount;

				if( m_bStateful == false && (*itList)->m_iUseCount == 0 )
				{
					delete *itList;
				}
			}
			
			itMap->second.m_clsSipMessageList.clear();
			bRes = true;
			time( &itMap->second.m_iCreateTime );
		}
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipStack
 * @brief TCP ������ ������ ��, ������ SIP �޽����� �����Ѵ�.
 * @param pszIp TCP ���� IP �ּ�
 * @param iPort TCP ���� ��Ʈ ��ȣ
 * @param pclsMessage SIP �޽���
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpConnectMap::Insert( const char * pszIp, int iPort, CSipMessage * pclsMessage )
{
	bool bRes = false;
	TCP_CONNECT_MAP::iterator	itMap;
	std::string strKey;

	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap != m_clsMap.end() )
	{
		++pclsMessage->m_iUseCount;
		itMap->second.m_clsSipMessageList.push_back( pclsMessage );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipStack
 * @brief TCP ������ ������ SIP �޽��� ����Ʈ�� �˻��� ��, �ش� ������ �ڷᱸ������ �����Ѵ�.
 * @param pszIp TCP ���� IP �ּ�
 * @param iPort TCP ���� ��Ʈ ��ȣ
 * @param clsList TCP ������ ������ SIP �޽��� ����Ʈ ������ ���� ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpConnectMap::Delete( const char * pszIp, int iPort, SIP_MESSAGE_LIST & clsList )
{
	bool bRes = false;
	TCP_CONNECT_MAP::iterator	itMap;
	std::string strKey;

	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap != m_clsMap.end() )
	{
		clsList = itMap->second.m_clsSipMessageList;
		m_clsMap.erase( itMap );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipStack
 * @brief TCP ������ ������ SIP �޽��� ����Ʈ �� �ش� ������ �ڷᱸ������ �����Ѵ�.
 * @param pszIp TCP ���� IP �ּ�
 * @param iPort TCP ���� ��Ʈ ��ȣ
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpConnectMap::Delete( const char * pszIp, int iPort )
{
	bool bRes = false;
	TCP_CONNECT_MAP::iterator	itMap;
	std::string strKey;

	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap != m_clsMap.end() )
	{
		SIP_MESSAGE_LIST::iterator	itList;

		for( itList = itMap->second.m_clsSipMessageList.begin(); itList != itMap->second.m_clsSipMessageList.end(); ++itList )
		{
			--(*itList)->m_iUseCount;

			if( m_bStateful == false && (*itList)->m_iUseCount == 0 )
			{
				delete *itList;
			}
		}

		m_clsMap.erase( itMap );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipStack
 * @brief �ڷᱸ���� ����� ������ �����Ѵ�.
 * @returns �ڷᱸ���� ����� ������ �����Ѵ�.
 */
int CTcpConnectMap::GetSize()
{
	int iSize;

	m_clsMutex.acquire();
	iSize = (int)m_clsMap.size();
	m_clsMutex.release();

	return iSize;
}

/**
 * @ingroup SipStack
 * @brief stateful ���θ� �����Ѵ�.
 * @param bStateful stateful �̸� true �̴�.
 */
void CTcpConnectMap::SetStateful( bool bStateful )
{
	m_bStateful = bStateful;
}

/**
 * @ingroup SipStack
 * @brief �ڷᱸ�� ����͸� ���ڿ��� �����Ѵ�.
 * @param strBuf ���ڿ� ����
 */
void CTcpConnectMap::GetString( CMonitorString & strBuf )
{
	TCP_CONNECT_MAP::iterator	itMap;

	strBuf.Clear();

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		strBuf.AddCol( itMap->first );
		strBuf.AddRow( (int)itMap->second.m_clsSipMessageList.size() );
	}
	m_clsMutex.release();
}

/**
 * @brief Ŭ���̾�Ʈ IP �ּҿ� ��Ʈ ��ȣ�� �ڷᱸ���� Ű�� �����Ͽ� �����Ѵ�.
 * @param pszIp		Ŭ���̾�Ʈ IP �ּ�
 * @param iPort		Ŭ���̾�Ʈ ��Ʈ ��ȣ
 * @param strKey TCP/TLS ���� ������ �����ϴ� �ڷᱸ���� Ű ���� ����
 */
void CTcpConnectMap::GetKey( const char * pszIp, int iPort, std::string & strKey )
{
	char	szKey[51];

	snprintf( szKey, sizeof(szKey), "%s:%d", pszIp, iPort );
	strKey = szKey;
}