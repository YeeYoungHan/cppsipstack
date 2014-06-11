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
#include "MemoryDebug.h"

CTcpConnectMap::CTcpConnectMap()
{
}

CTcpConnectMap::~CTcpConnectMap()
{
}

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

		m_clsMap.insert( TCP_CONNECT_MAP::value_type( strKey, clsInfo ) );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

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
		}

		m_clsMap.erase( itMap );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @brief 클라이언트 IP 주소와 포트 번호로 자료구조의 키를 생성하여 저장한다.
 * @param pszIp		클라이언트 IP 주소
 * @param iPort		클라이언트 포트 번호
 * @param strKey TCP/TLS 세션 정보를 저장하는 자료구조의 키 저장 변수
 */
void CTcpConnectMap::GetKey( const char * pszIp, int iPort, std::string & strKey )
{
	char	szKey[51];

	snprintf( szKey, sizeof(szKey), "%s:%d", pszIp, iPort );
	strKey = szKey;
}
