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
#include <time.h>

CTcpSessionListInfo::CTcpSessionListInfo() : m_iPort(0), m_iConnectTime(0), m_iRecvTime(0)
{

}

void CTcpSessionListInfo::Clear()
{
	m_strIp.clear();
	m_iPort = 0;

	m_clsSipBuf.Clear();

	m_iConnectTime = 0;
	m_iRecvTime = 0;
}

CTcpSessionList::CTcpSessionList(void) : m_psttPollFd(NULL), m_iPollFdMax(0), m_iPoolFdCount(0)
{
}

CTcpSessionList::~CTcpSessionList(void)
{
	if( m_psttPollFd ) free( m_psttPollFd );
}

/**
 * @brief TCP ���� ������ �ʱ�ȭ��Ų��.
 * @param iPollFdMax TCP ���� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief TCP ������ �߰��Ѵ�.
 * @param hSocket TCP ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSessionList::Insert( Socket hSocket )
{
	if( m_iPoolFdCount >= m_iPollFdMax ) return false;

	TcpSetPollIn( m_psttPollFd[m_iPoolFdCount], hSocket );
	++m_iPoolFdCount;

	return true;
}

/**
 * @brief TCP ���� ������ �����Ѵ�.
 * @param iIndex TCP ���� �ε���
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CTcpSessionList::Delete( int iIndex, CThreadListEntry * pclsEntry )
{
	if( iIndex >= m_iPoolFdCount || iIndex < 0 ) return false;

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
 * @brief TCP ���� timeout �� �߻��� TCP ������ �����Ų��.
 * @param iTimeout	TCP ���� timeout �ð� (�ʴ���)
 * @param pclsEntry TCP ������ ����
 */
void CTcpSessionList::DeleteTimeout( int iTimeout, CThreadListEntry * pclsEntry )
{
	time_t	iTime;

	time( &iTime );
	iTime -= iTimeout;

	for( int i = 1; i < m_iPoolFdCount; ++i )
	{
		if( m_clsList[i].m_iRecvTime == 0 ) continue;

		if( m_clsList[i].m_iRecvTime < iTime )
		{
			Delete( i, pclsEntry );
		}
	}
}
