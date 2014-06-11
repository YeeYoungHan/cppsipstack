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

/**
 * @ingroup SipStack
 * @brief TCP 서버에 접속중 정보를 저장한다.
 * @param pszIp TCP 서버 IP 주소
 * @param iPort TCP 서버 포트 번호
 * @returns 저장에 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
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

		m_clsMap.insert( TCP_CONNECT_MAP::value_type( strKey, clsInfo ) );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipStack
 * @brief TCP 서버에 접속한 후, 전송할 SIP 메시지를 저장한다.
 * @param pszIp TCP 서버 IP 주소
 * @param iPort TCP 서버 포트 번호
 * @param pclsMessage SIP 메시지
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
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
 * @brief TCP 서버로 전송할 SIP 메시지 리스트를 검색한 후, 해당 정보를 자료구조에서 삭제한다.
 * @param pszIp TCP 서버 IP 주소
 * @param iPort TCP 서버 포트 번호
 * @param clsList TCP 서버로 전송할 SIP 메시지 리스트 저장을 위한 변수
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
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
 * @brief TCP 서버로 전송할 SIP 메시지 리스트 및 해당 정보를 자료구조에서 삭제한다.
 * @param pszIp TCP 서버 IP 주소
 * @param iPort TCP 서버 포트 번호
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
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
		}

		m_clsMap.erase( itMap );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipStack
 * @brief 자료구조에 저장된 개수를 리턴한다.
 * @returns 자료구조에 저장된 개수를 리턴한다.
 */
int CTcpConnectMap::GetSize()
{
	int iSize;

	m_clsMutex.acquire();
	iSize = m_clsMap.size();
	m_clsMutex.release();

	return iSize;
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
