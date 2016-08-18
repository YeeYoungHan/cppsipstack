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
#include "SipTlsMessage.h"
#include "Log.h"
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
 * @brief 클라이언트 정보를 저장한다.
 * @param pszIp		클라이언트 IP 주소
 * @param iPort		클라이언트 포트 번호
 * @param hSocket TCP 소켓
 * @param psttSsl	SSL 구조체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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
 * @brief 클라이언트 IP 주소와 포트 번호와 연관된 TCP 소켓 핸들을 검색한다.
 * @param pszIp		클라이언트 IP 주소
 * @param iPort		클라이언트 포트 번호
 * @param hSocket TCP 소켓을 저장할 변수
 * @returns 검색에 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
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
 * @brief TLS 메시지를 전송한다.
 * @param pszIp		클라이언트 IP 주소
 * @param iPort		클라이언트 포트 번호
 * @param pclsMessage SIP 메시지
 * @param iLocalPort	로컬 포트 번호
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CTcpSocketMap::SendTls( const char * pszIp, int iPort, CSipMessage * pclsMessage )
{
	bool	bRes = false;
	std::string	strKey;
	TCP_SOCKET_MAP::iterator	it;
	
	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	it = m_clsMap.find( strKey );
	if( it != m_clsMap.end() )
	{
		if( SipTlsSend( it->second.m_hSocket, it->second.m_psttSsl, pszIp, iPort, pclsMessage ) == false )
		{
			CLog::Print( LOG_DEBUG, "%s(%s,%d) TLS deleted", __FUNCTION__, pszIp, iPort );
			m_clsMap.erase( it );			
		}
		else
		{
			bRes = true;
		}
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipStack
 * @brief TLS 메시지를 전송한다.
 * @param pszIp		클라이언트 IP 주소
 * @param iPort		클라이언트 포트 번호
 * @param pszMessage	 SIP 메시지
 * @param iMessageSize SIP 메시지 길이
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CTcpSocketMap::SendTls( const char * pszIp, int iPort, const char * pszMessage, int iMessageSize )
{
	bool	bRes = false;
	std::string	strKey;
	TCP_SOCKET_MAP::iterator	it;
	
	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	it = m_clsMap.find( strKey );
	if( it != m_clsMap.end() )
	{
		if( SSLSend( it->second.m_psttSsl, pszMessage, iMessageSize ) != iMessageSize )
		{
			CLog::Print( LOG_DEBUG, "%s(%s,%d) TLS deleted", __FUNCTION__, pszIp, iPort );
			m_clsMap.erase( it );
		}
		else
		{
			bRes = true;
		}
	}
	m_clsMutex.release();

	return bRes;
}
#endif

/**
 * @ingroup SipStack
 * @brief 자료구조에서 클라이언트 IP 주소와 포트번호에 해당하는 정보를 삭제한다.
 * @param pszIp 클라이언트 IP 주소
 * @param iPort 클라이언트 포트 번호
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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
 * @brief 자료구조를 초기화한다.
 */
void CTcpSocketMap::DeleteAll( )
{
	m_clsMutex.acquire();
	m_clsMap.clear();
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief TCP 세션들의 정보를 하나의 문자열에 저장한다.
 * @param strBuf TCP 세션들의 정보를 저장할 문자열 변수
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
 * @brief 클라이언트 IP 주소와 포트 번호로 자료구조의 키를 생성하여 저장한다.
 * @param pszIp		클라이언트 IP 주소
 * @param iPort		클라이언트 포트 번호
 * @param strKey TCP/TLS 세션 정보를 저장하는 자료구조의 키 저장 변수
 */
void CTcpSocketMap::GetKey( const char * pszIp, int iPort, std::string & strKey )
{
	char	szKey[51];

	snprintf( szKey, sizeof(szKey), "%s:%d", pszIp, iPort );
	strKey = szKey;
}
