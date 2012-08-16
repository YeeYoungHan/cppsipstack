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

#include "SipDeleteQueue.h"

CSipDeleteQueue gclsSipDeleteQueue;
extern time_t giTime;

/**
 * @ingroup SipStack
 * @brief 생성자
 */
CSipDeleteQueue::CSipDeleteQueue()
{
}

/**
 * @ingroup SipStack
 * @brief 소멸자
 */
CSipDeleteQueue::~CSipDeleteQueue()
{
}

/**
 * @ingroup SipStack
 * @brief 일정 시간 후에 삭제할 SIP 메시지를 저장한다.
 * @param pclsMessage 만료된 SIP 메시지
 */
void CSipDeleteQueue::Insert( CSipMessage * pclsMessage )
{
	CSipDeleteInfo clsInfo;

	clsInfo.m_pclsMessage = pclsMessage;
	clsInfo.m_iDeleteTime = giTime + SIP_DELETE_TIME;

	m_clsMutex.acquire();
	m_clsList.push_back( clsInfo );
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief 대기 시간이 초과한 SIP 메시지를 삭제한다.
 */
void CSipDeleteQueue::DeleteTimeout( )
{
	int			iCount;
	time_t	iTime = giTime;

	m_clsMutex.acquire();
	iCount = m_clsList.size();
	if( iCount > 0 )
	{
		for( int i = 0; i < iCount; ++i )
		{
			if( m_clsList.front().m_iDeleteTime > iTime ) break;
			if( m_clsList.front().m_pclsMessage->m_iUseCount > 0 ) break;

			delete m_clsList.front().m_pclsMessage;

			m_clsList.pop_front();
		}
	}
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief		자료구조에 저장된 SIP 메시지의 개수를 리턴한다.
 * @returns 자료구조에 저장된 SIP 메시지의 개수를 리턴한다.
 */
int CSipDeleteQueue::GetSize()
{
	return m_clsList.size();
}
