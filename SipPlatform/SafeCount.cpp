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

#include "SafeCount.h"

CSafeCount::CSafeCount() : m_iCount(0)
{
}

CSafeCount::~CSafeCount()
{
}

/**
 * @ingroup SipPlatform
 * @brief 카운트를 1 증가시킨다.
 */
void CSafeCount::Increase()
{
	m_clsMutex.acquire();
	++m_iCount;
	m_clsMutex.release();
}

/**
 * @ingroup SipPlatform
 * @brief 카운트를 1 감소시킨다.
 */
void CSafeCount::Decrease()
{
	m_clsMutex.acquire();
	--m_iCount;
	m_clsMutex.release();
}

/**
 * @ingroup SipPlatform
 * @brief 카운트를 재설정한다.
 * @param iCount 카운트
 */
void CSafeCount::SetCount( int iCount )
{
	m_clsMutex.acquire();
	m_iCount = iCount;
	m_clsMutex.release();
}

/**
 * @ingroup SipPlatform
 * @brief 카운트를 리턴한다.
 * @returns 카운트를 리턴한다.
 */
int CSafeCount::GetCount()
{
	int iCount;

	m_clsMutex.acquire();
	iCount = m_iCount;
	m_clsMutex.release();

	return iCount;
}
