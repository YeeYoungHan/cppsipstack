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
#include "DeadLockDetectMutex.h"
#include "Log.h"

CDeadLockDetectMutex::CDeadLockDetectMutex()
{
}

CDeadLockDetectMutex::~CDeadLockDetectMutex()
{
}

bool CDeadLockDetectMutex::acquire()
{
	THREAD_ID_LIST::iterator itTL;
	THREAD_ID_TYPE iThreadId = GetCurrentThreadId();

	m_clsPrivateMutex.acquire();

	// Window CRITICAL_SECTION 은 하나의 쓰레드에서 2번 이상 lock 을 허용하므로 현재 쓰레드에 대한 dead lock 검사를 수행하지 않는다.
#ifndef WIN32
	for( itTL = m_clsThreadIdList.begin(); itTL != m_clsThreadIdList.end(); ++itTL )
	{
		if( *itTL == iThreadId )
		{
			CLog::Print( LOG_ERROR, "dead lock in single thread" );
			CLog::PrintCallStack( LOG_ERROR );
			break;
		}
	}

	m_clsThreadIdList.push_back( iThreadId );
#endif
	m_clsPrivateMutex.release();

	return m_clsMutex.acquire();
}

bool CDeadLockDetectMutex::release()
{
	THREAD_ID_LIST::reverse_iterator itTL;
	THREAD_ID_TYPE iThreadId = GetCurrentThreadId();

	m_clsPrivateMutex.acquire();

	for( itTL = m_clsThreadIdList.rbegin(); itTL != m_clsThreadIdList.rend(); ++itTL )
	{
		if( *itTL == iThreadId )
		{
			m_clsThreadIdList.erase( --itTL.base() );
			break;
		}
	}

	m_clsPrivateMutex.release();

	return m_clsMutex.release();
}
