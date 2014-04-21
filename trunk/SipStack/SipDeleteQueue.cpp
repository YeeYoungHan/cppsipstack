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
#include "MemoryDebug.h"

CSipDeleteQueue gclsSipDeleteQueue;
extern time_t giTime;

/**
 * @ingroup SipStack
 * @brief ������
 */
CSipDeleteQueue::CSipDeleteQueue()
{
}

/**
 * @ingroup SipStack
 * @brief �Ҹ���
 */
CSipDeleteQueue::~CSipDeleteQueue()
{
}

/**
 * @ingroup SipStack
 * @brief ���� �ð� �Ŀ� ������ SIP �޽����� �����Ѵ�.
 * @param pclsMessage ����� SIP �޽���
 */
void CSipDeleteQueue::Insert( CSipMessage * pclsMessage )
{
	if( pclsMessage->m_iUseCount == 0 )
	{
		delete pclsMessage;
		return;
	}

	CSipDeleteInfo clsInfo;

	clsInfo.m_pclsMessage = pclsMessage;
	clsInfo.m_iDeleteTime = giTime + SIP_DELETE_TIME;

	m_clsMutex.acquire();
	m_clsList.push_back( clsInfo );
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief ��� �ð��� �ʰ��� SIP �޽����� �����Ѵ�.
 */
void CSipDeleteQueue::DeleteTimeout( )
{
	int			iCount;
	time_t	iTime = giTime;

	m_clsMutex.acquire();
	iCount = (int)m_clsList.size();
	if( iCount > 0 )
	{
		for( int i = 0; i < iCount; ++i )
		{
			CSipDeleteInfo & clsInfo = m_clsList.front();

			if( clsInfo.m_iDeleteTime > iTime ) break;
			if( clsInfo.m_pclsMessage->m_iUseCount > 0 ) break;

			delete clsInfo.m_pclsMessage;
			m_clsList.pop_front();
		}
	}
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief ��� SIP �޽����� ��� �����Ѵ�.
 */
void CSipDeleteQueue::DeleteAll( )
{
	SIP_DELETE_QUEUE::iterator	it;

	m_clsMutex.acquire();
	for( it = m_clsList.begin(); it != m_clsList.end(); ++it )
	{
		delete it->m_pclsMessage;
	}
	m_clsList.clear();
	m_clsMutex.release();
}

/**
 * @ingroup SipStack
 * @brief		�ڷᱸ���� ����� SIP �޽����� ������ �����Ѵ�.
 * @returns �ڷᱸ���� ����� SIP �޽����� ������ �����Ѵ�.
 */
int CSipDeleteQueue::GetSize()
{
	int iSize;

	m_clsMutex.acquire();
	iSize = (int)m_clsList.size();
	m_clsMutex.release();

	return iSize;
}
