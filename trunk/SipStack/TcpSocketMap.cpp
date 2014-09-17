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
#include "TcpSocketMap.h"
#include "MemoryDebug.h"

CTcpSocketInfo::CTcpSocketInfo() : m_hSocket(INVALID_SOCKET)
#ifdef USE_TLS
	, m_psttSsl(NULL)
#endif
{

}

CTcpSocketMap::CTcpSocketMap()
{
}

CTcpSocketMap::~CTcpSocketMap()
{
}

/**
 * @ingroup SipStack
 * @brief Ŭ���̾�Ʈ ������ �����Ѵ�.
 * @param pszIp		Ŭ���̾�Ʈ IP �ּ�
 * @param iPort		Ŭ���̾�Ʈ ��Ʈ ��ȣ
 * @param hSocket TCP ����
 * @param psttSsl	SSL ����ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSocketMap::Insert( const char * pszIp, int iPort, Socket hSocket, SSL * psttSsl )
{
	std::string	strKey;
	CTcpSocketInfo	clsTcpSocketInfo;
	TCP_SOCKET_MAP::iterator	it;
	
	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	it = m_clsMap.find( strKey );
	if( it == m_clsMap.end() )
	{
		clsTcpSocketInfo.m_hSocket = hSocket;

#ifdef USE_TLS
		clsTcpSocketInfo.m_psttSsl = psttSsl;
#endif

		m_clsMap.insert( TCP_SOCKET_MAP::value_type( strKey, clsTcpSocketInfo ) );
	}
	m_clsMutex.release();

	return true;
}

/**
 * @ingroup SipStack
 * @brief Ŭ���̾�Ʈ IP �ּҿ� ��Ʈ ��ȣ�� ������ TCP ���� �ڵ��� �˻��Ѵ�.
 * @param pszIp		Ŭ���̾�Ʈ IP �ּ�
 * @param iPort		Ŭ���̾�Ʈ ��Ʈ ��ȣ
 * @param hSocket TCP ������ ������ ����
 * @returns �˻��� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpSocketMap::Select( const char * pszIp, int iPort, Socket & hSocket )
{
	bool	bRes = false;
	std::string	strKey;
	TCP_SOCKET_MAP::iterator	it;
	
	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	it = m_clsMap.find( strKey );
	if( it != m_clsMap.end() )
	{
		hSocket = it->second.m_hSocket;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

#ifdef USE_TLS
/**
 * @ingroup SipStack
 * @brief Ŭ���̾�Ʈ IP �ּҿ� ��Ʈ ��ȣ�� ������ TCP ���� ������ �˻��Ѵ�.
 * @param pszIp		Ŭ���̾�Ʈ IP �ּ�
 * @param iPort		Ŭ���̾�Ʈ ��Ʈ ��ȣ
 * @param ppclsInfo TCP ���� ������ ������ ������ ������
 * @returns �˻��� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CTcpSocketMap::Select( const char * pszIp, int iPort, CTcpSocketInfo ** ppclsInfo )
{
	bool	bRes = false;
	std::string	strKey;
	TCP_SOCKET_MAP::iterator	it;
	
	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	it = m_clsMap.find( strKey );
	if( it != m_clsMap.end() )
	{
		*ppclsInfo = &it->second;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}
#endif

/**
 * @ingroup SipStack
 * @brief �ڷᱸ������ Ŭ���̾�Ʈ IP �ּҿ� ��Ʈ��ȣ�� �ش��ϴ� ������ �����Ѵ�.
 * @param pszIp Ŭ���̾�Ʈ IP �ּ�
 * @param iPort Ŭ���̾�Ʈ ��Ʈ ��ȣ
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSocketMap::Delete( const char * pszIp, int iPort )
{
	bool	bRes = false;
	std::string	strKey;
	TCP_SOCKET_MAP::iterator	it;
	
	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	it = m_clsMap.find( strKey );
	if( it != m_clsMap.end() )
	{
		m_clsMap.erase( it );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipStack
 * @brief TCP ���ǵ��� ������ �ϳ��� ���ڿ��� �����Ѵ�.
 * @param strBuf TCP ���ǵ��� ������ ������ ���ڿ� ����
 */
void CTcpSocketMap::GetString( CMonitorString & strBuf )
{
	TCP_SOCKET_MAP::iterator	it;

	strBuf.Clear();

	m_clsMutex.acquire();
	for( it = m_clsMap.begin(); it != m_clsMap.end(); ++it )
	{
		strBuf.AddRow( it->first );
	}
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief Ŭ���̾�Ʈ IP �ּҿ� ��Ʈ ��ȣ�� �ڷᱸ���� Ű�� �����Ͽ� �����Ѵ�.
 * @param pszIp		Ŭ���̾�Ʈ IP �ּ�
 * @param iPort		Ŭ���̾�Ʈ ��Ʈ ��ȣ
 * @param strKey TCP/TLS ���� ������ �����ϴ� �ڷᱸ���� Ű ���� ����
 */
void CTcpSocketMap::GetKey( const char * pszIp, int iPort, std::string & strKey )
{
	char	szKey[51];

	snprintf( szKey, sizeof(szKey), "%s:%d", pszIp, iPort );
	strKey = szKey;
}
