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
#include "TcpSessionList.h"
#include "SipStack.h"
#include "Log.h"
#include <time.h>
#include "MemoryDebug.h"

CTcpSessionListInfo::CTcpSessionListInfo() : m_iPort(0)
#ifdef USE_TLS
, m_psttSsl(NULL)
#endif
, m_iConnectTime(0), m_iRecvTime(0), m_bUseTimeout(true)
{

}

/**
 * @ingroup SipStack
 * @brief 세션 정보를 초기화시킨다.
 */
void CTcpSessionListInfo::Clear()
{
	m_strIp.clear();
	m_iPort = 0;

	m_clsSipBuf.Clear();

	m_iConnectTime = 0;
	m_iRecvTime = 0;
}

CTcpSessionList::CTcpSessionList( CSipStack * pclsSipStack, ESipTransport eTransport ) : m_psttPollFd(NULL), m_iPollFdMax(0), m_iPoolFdCount(0)
	, m_pclsSipStack( pclsSipStack), m_eTransport( eTransport )
{
}

CTcpSessionList::~CTcpSessionList(void)
{
	if( m_psttPollFd ) free( m_psttPollFd );
}

/**
 * @ingroup SipStack
 * @brief TCP 세션 정보를 초기화시킨다.
 * @param iPollFdMax TCP 세션 개수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CTcpSessionList::Init( int iPollFdMax )
{
	m_iPollFdMax = iPollFdMax;

	m_psttPollFd = ( struct pollfd * )malloc( sizeof(struct pollfd) * m_iPollFdMax );
	if( m_psttPollFd == NULL )
	{
		return false;
	}

	for( int i = 0; i < m_iPollFdMax; ++i )
	{
		TcpSetPollIn( m_psttPollFd[i], INVALID_SOCKET );
	}

	for( int i = 0; i < m_iPollFdMax; ++i )
	{
		CTcpSessionListInfo	clsEntry;

		m_clsList.push_back( clsEntry );
	}

	return true;
}

/**
 * @ingroup SipStack
 * @brief TCP 소켓을 추가한다.
 * @param hSocket TCP 소켓
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CTcpSessionList::Insert( Socket hSocket )
{
	if( m_iPoolFdCount >= m_iPollFdMax ) return false;

	TcpSetPollIn( m_psttPollFd[m_iPoolFdCount], hSocket );
	++m_iPoolFdCount;

	return true;
}

/**
 * @ingroup SipStack
 * @brief TCP 세션 정보를 추가한다.
 * @param clsTcpComm	TCP 세션 정보 저장 객체
 * @param psttSsl			SSL 세션 정보 저장 구조체 - SSL 세션인 경우만 저장한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CTcpSessionList::Insert( CTcpComm & clsTcpComm, SSL * psttSsl )
{
	time_t	iTime;

	time( &iTime );

	if( m_iPoolFdCount >= m_iPollFdMax )
	{
		for( int i = 1; i < m_iPoolFdCount; ++i )
		{
			if( m_psttPollFd[i].fd == INVALID_SOCKET )
			{
				TcpSetPollIn( m_psttPollFd[i], clsTcpComm.m_hSocket );
				m_clsList[i].m_strIp = clsTcpComm.m_szIp;
				m_clsList[i].m_iPort = clsTcpComm.m_iPort;
				m_clsList[i].m_iConnectTime = iTime;
				m_clsList[i].m_iRecvTime = iTime;

#ifdef USE_TLS
				m_clsList[i].m_psttSsl = psttSsl;
#endif

				return true;
			}
		}

		return false;
	}

	TcpSetPollIn( m_psttPollFd[m_iPoolFdCount], clsTcpComm.m_hSocket );
	m_clsList[m_iPoolFdCount].m_strIp = clsTcpComm.m_szIp;
	m_clsList[m_iPoolFdCount].m_iPort = clsTcpComm.m_iPort;
	m_clsList[m_iPoolFdCount].m_iConnectTime = iTime;
	m_clsList[m_iPoolFdCount].m_iRecvTime = iTime;

#ifdef USE_TLS
	m_clsList[m_iPoolFdCount].m_psttSsl = psttSsl;
#endif

	++m_iPoolFdCount;

	return true;
}

/**
 * @ingroup SipStack
 * @brief TCP 세션 정보를 삭제한다.
 * @param iIndex		TCP 세션 인덱스
 * @param pclsEntry	TCP 쓰레드 정보 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CTcpSessionList::Delete( int iIndex, CThreadListEntry * pclsEntry )
{
	if( iIndex >= m_iPoolFdCount || iIndex < 0 ) return false;

	m_pclsSipStack->TcpSessionEnd( m_clsList[iIndex].m_strIp.c_str(), m_clsList[iIndex].m_iPort, m_eTransport );

#ifdef USE_TLS
	if( m_clsList[iIndex].m_psttSsl )
	{
		SSLClose( m_clsList[iIndex].m_psttSsl );
		m_clsList[iIndex].m_psttSsl = NULL;
	}
#endif

	closesocket( m_psttPollFd[iIndex].fd );

	m_psttPollFd[iIndex].fd = INVALID_SOCKET;
	m_psttPollFd[iIndex].events = 0;
	m_psttPollFd[iIndex].revents = 0;
	m_clsList[iIndex].Clear();

	if( iIndex == (m_iPoolFdCount - 1) )
	{
		for( int i = iIndex; i >= 0; --i )
		{
			if( m_psttPollFd[i].fd != INVALID_SOCKET ) break;
			m_iPoolFdCount = i;
		}
	}

	pclsEntry->DecreaseSocketCount();

	return true;
}

/**
 * @ingroup SipStack
 * @brief 모든 TCP 세션 정보를 삭제한다.
 * @param pclsEntry	TCP 쓰레드 정보 객체
 */
void CTcpSessionList::DeleteAll( CThreadListEntry * pclsEntry )
{
	for( int i = 0; i < m_iPoolFdCount; ++i )
	{
#ifdef USE_TLS
		if( m_clsList[i].m_psttSsl )
		{
			SSLClose( m_clsList[i].m_psttSsl );
			m_clsList[i].m_psttSsl = NULL;
		}
#endif

		if( m_psttPollFd[i].fd != INVALID_SOCKET )
		{
			closesocket( m_psttPollFd[i].fd );

			m_psttPollFd[i].fd = INVALID_SOCKET;
			m_psttPollFd[i].events = 0;
			m_psttPollFd[i].revents = 0;
			m_clsList[i].Clear();

			pclsEntry->DecreaseSocketCount();
		}
	}
}

/**
 * @ingroup SipStack
 * @brief TCP 수신 timeout 이 발생한 TCP 세션을 종료시킨다.
 * @param iTimeout	TCP 수신 timeout 시간 (초단위)
 * @param pclsEntry TCP 쓰레드 정보
 */
void CTcpSessionList::DeleteTimeout( int iTimeout, CThreadListEntry * pclsEntry )
{
	time_t	iTime;

	time( &iTime );
	iTime -= iTimeout;

	for( int i = 1; i < m_iPoolFdCount; ++i )
	{
		if( m_clsList[i].m_bUseTimeout == false ) continue;
		if( m_clsList[i].m_iRecvTime == 0 ) continue;

		if( m_clsList[i].m_iRecvTime < iTime )
		{
			CLog::Print( LOG_DEBUG, "%s (%s,%d) timeout", __FUNCTION__, m_clsList[i].m_strIp.c_str(), m_clsList[i].m_iPort );

			if( m_eTransport == E_SIP_TCP )
			{
				m_pclsSipStack->m_clsTcpSocketMap.Delete( m_clsList[i].m_strIp.c_str(), m_clsList[i].m_iPort );
			}
			else if( m_eTransport == E_SIP_TLS )
			{
				m_pclsSipStack->m_clsTlsSocketMap.Delete( m_clsList[i].m_strIp.c_str(), m_clsList[i].m_iPort );
			}

			Delete( i, pclsEntry );
		}
	}
}
