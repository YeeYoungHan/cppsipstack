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

#include "SipPlatformDefine.h"
#include "TcpSessionMap.h"
#include "TcpStackCallBack.h"
#include "MemoryDebug.h"

CTcpSessionMap::CTcpSessionMap() : m_iThreadIndex(0)
{
}

CTcpSessionMap::~CTcpSessionMap()
{
}

/**
 * @ingroup TcpStack
 * @brief TCP 세션 정보를 저장한다.
 * @param pszIp		클라이언트 IP 주소
 * @param iPort		클라이언트 포트 번호
 * @param pclsSessionInfo TCP 세션 정보
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CTcpSessionMap::Insert( const char * pszIp, int iPort, CTcpSessionInfo * pclsSessionInfo )
{
	TCP_SESSION_MAP::iterator itTSM;
	std::string strKey;
	bool bRes = false;

	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	itTSM = m_clsMap.find( strKey );
	if( itTSM == m_clsMap.end() )
	{
		pclsSessionInfo->m_iThreadIndex = GetThreadIndex();
		m_clsMap.insert( TCP_SESSION_MAP::value_type( strKey, pclsSessionInfo ) );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TcpStack
 * @brief TCP 세션 정보를 삭제한다.
 * @param pszIp		클라이언트 IP 주소
 * @param iPort		클라이언트 포트 번호
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CTcpSessionMap::Delete( const char * pszIp, int iPort )
{
	TCP_SESSION_MAP::iterator itTSM;
	std::string strKey;
	bool bRes = false;

	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	itTSM = m_clsMap.find( strKey );
	if( itTSM != m_clsMap.end() )
	{
		// // thread 의 스택 변수의 포인터가 second 이기 때문에 delete 하면 안 된다.
		// delete itTSM->second;
		m_clsMap.erase( itTSM );

		int iMaxIndex = 0;

		for( itTSM = m_clsMap.begin(); itTSM != m_clsMap.end(); ++itTSM )
		{
			if( itTSM->second->m_iThreadIndex > iMaxIndex )
			{
				iMaxIndex = itTSM->second->m_iThreadIndex;
			}
		}

		m_iThreadIndex = iMaxIndex;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TcpStack
 * @brief 세션 개수를 리턴한다.
 * @returns 세션 개수를 리턴한다.
 */
int CTcpSessionMap::GetCount( )
{
	int iCount;

	m_clsMutex.acquire();
	iCount = m_clsMap.size();
	m_clsMutex.release();

	return iCount;
}

/**
 * @ingroup TcpStack
 * @brief 특정 세션에 TCP 패킷을 전송한다.
 * @param pszIp				IP 주소
 * @param iPort				포트 번호
 * @param pszPacket		패킷
 * @param iPacketLen	패킷 길이
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CTcpSessionMap::Send( const char * pszIp, int iPort, const char * pszPacket, int iPacketLen )
{
	TCP_SESSION_MAP::iterator itTSM;
	std::string strKey;
	bool bRes = false;

	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	itTSM = m_clsMap.find( strKey );
	if( itTSM != m_clsMap.end() )
	{
		bRes = itTSM->second->Send( pszPacket, iPacketLen );
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TcpStack
 * @brief 특정 세션에 TCP 패킷을 전송한다.
 * @param iThreadIndex	TCP 쓰레드 번호
 * @param pszPacket			패킷
 * @param iPacketLen		패킷 길이
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CTcpSessionMap::Send( int iThreadIndex, const char * pszPacket, int iPacketLen )
{
	TCP_SESSION_MAP::iterator itTSM;
	bool bRes = false;

	m_clsMutex.acquire();
	for( itTSM = m_clsMap.begin(); itTSM != m_clsMap.end(); ++itTSM )
	{
		if( itTSM->second->m_iThreadIndex == iThreadIndex )
		{
			bRes = itTSM->second->Send( pszPacket, iPacketLen );
			break;
		}
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TcpStack
 * @brief 모든 세션에 TCP 패킷을 전송한다.
 * @param pszPacket			패킷
 * @param iPacketLen		패킷 길이
 * @param pclsCallBack	세션별로 전송 유무를 결정하는 callback 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CTcpSessionMap::SendAll( const char * pszPacket, int iPacketLen, ITcpStackCallBack * pclsCallBack )
{
	TCP_SESSION_MAP::iterator itTSM;

	m_clsMutex.acquire();
	for( itTSM = m_clsMap.begin(); itTSM != m_clsMap.end(); ++itTSM )
	{
		if( pclsCallBack->IsSendAll( itTSM->second ) )
		{
			itTSM->second->Send( pszPacket, iPacketLen );
			pclsCallBack->AfterSendAllPerSession( itTSM->second, pszPacket, iPacketLen );
		}
	}
	m_clsMutex.release();

	return true;
}

/**
 * @ingroup TcpStack
 * @brief 특정 세션을 제외한 모든 세션에 TCP 패킷을 전송한다.
 * @param pszPacket			패킷
 * @param iPacketLen		패킷 길이
 * @param pclsCallBack	세션별로 전송 유무를 결정하는 callback 객체
 * @param iThreadIndex	전송하지 않을 세션의 쓰레드 인덱스
 * @param iSessionIndex 전송하지 않을 세션 인덱스
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CTcpSessionMap::SendAllExcept( const char * pszPacket, int iPacketLen, ITcpStackCallBack * pclsCallBack, int iThreadIndex, int iSessionIndex )
{
	TCP_SESSION_MAP::iterator itTSM;

	m_clsMutex.acquire();
	for( itTSM = m_clsMap.begin(); itTSM != m_clsMap.end(); ++itTSM )
	{
		if( pclsCallBack->IsSendAll( itTSM->second ) )
		{
			if( itTSM->second->m_iThreadIndex == iThreadIndex && itTSM->second->m_iSessionIndex == iSessionIndex ) continue;

			itTSM->second->Send( pszPacket, iPacketLen );
			pclsCallBack->AfterSendAllPerSession( itTSM->second, pszPacket, iPacketLen );
		}
	}
	m_clsMutex.release();

	return true;
}

/**
 * @ingroup TcpStack
 * @brief 자료구조의 키를 생성한다.
 * @param pszIp TCP 서버 IP 주소
 * @param iPort TCP 서버 포트 번호
 * @param strKey 자료구조의 키를 저장할 변수
 */
void CTcpSessionMap::GetKey( const char * pszIp, int iPort, std::string & strKey )
{
	char szPort[6];

	snprintf( szPort, sizeof(szPort), "%d", iPort );

	strKey = pszIp;
	strKey.append( ":" );
	strKey.append( szPort );
}

/**
 * @ingroup TcpStack
 * @brief 새로 사용할 쓰레드 번호를 가져온다.
 * @returns 새로 사용할 쓰레드 번호를 리턴한다.
 */
int CTcpSessionMap::GetThreadIndex()
{
	++m_iThreadIndex;
	if( m_iThreadIndex > 2000000000 )
	{
		m_iThreadIndex = 1;
	}

	while( SelectThreadIndex( m_iThreadIndex ) )
	{
		++m_iThreadIndex;
		if( m_iThreadIndex > 2000000000 )
		{
			m_iThreadIndex = 1;
		}
	}

	return m_iThreadIndex;
}

/**
 * @ingroup TcpStack
 * @brief 쓰레드 번호가 사용중인지 검사한다.
 * @param iThreadIndex 쓰레드 번호
 * @returns 쓰레드 번호가 사용중이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CTcpSessionMap::SelectThreadIndex( int iThreadIndex )
{
	TCP_SESSION_MAP::iterator itTSM;

	for( itTSM = m_clsMap.begin(); itTSM != m_clsMap.end(); ++itTSM )
	{
		if( itTSM->second->m_iThreadIndex == iThreadIndex )
		{
			return true;
		}
	}

	return false;
}
