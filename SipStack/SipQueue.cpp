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

#include "SipQueue.h"

CSipQueue gclsSipQueue;

CSipQueue::CSipQueue()
{
}

CSipQueue::~CSipQueue()
{
}

/**
 * @ingroup SipStack
 * @brief SIP 수신 메시지를 저장한다.
 * @param pszBuf			SIP 수신 메시지
 * @param iBufLen			SIP 수신 메시지 길이
 * @param pszIp				SIP 전송 IP 주소
 * @param sPort				SIP 전송 포트 번호
 * @param eTransport	SIP 전송 프로토콜
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipQueue::Insert( const char * pszBuf, int iBufLen, const char * pszIp, unsigned short sPort, ESipTransport eTransport )
{
	CSipQueueEntry * pclsEntry = new CSipQueueEntry();
	if( pclsEntry == NULL ) return false;

	pclsEntry->m_strBuf.append( pszBuf, iBufLen );
	pclsEntry->m_strIp = pszIp;
	pclsEntry->m_sPort = sPort;
	pclsEntry->m_eTransport = eTransport;

	m_clsMutex.acquire();
	m_clsList.push_back( pclsEntry );
	m_clsMutex.signal();
	m_clsMutex.release();

	return true;
}

/**
 * @ingroup SipStack
 * @brief SIP 수신 메시지를 가져온다.
 * @param ppclsEntry SIP 수신 메시지를 저장할 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipQueue::Select( CSipQueueEntry ** ppclsEntry )
{
	bool bRes = false;

	m_clsMutex.acquire();
	if( m_clsList.empty() )
	{
		m_clsMutex.wait();
	}
	
	if( m_clsList.empty() == false )
	{
		*ppclsEntry = m_clsList.front();
		m_clsList.pop_front();
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipStack
 * @brief SIP 수신 메시지를 대기하는 쓰레드를 모두 깨운다.
 */
void CSipQueue::BroadCast( )
{
	m_clsMutex.broadcast();
}
