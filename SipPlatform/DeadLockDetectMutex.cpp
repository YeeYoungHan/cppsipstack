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

#ifdef WIN32
#define THREAD_ID_FORMAT "%u"
#else
#define THREAD_ID_FORMAT "%lu"
#endif

typedef std::map< CDeadLockDetectMutex *, int > MUTEX_POINTER_MAP;

class CMutexPointerMap
{
public:
	MUTEX_POINTER_MAP m_clsMap;
};

static CSipMutex * gpclsMapMutex = NULL;
static CMutexPointerMap * gpclsMutexPointMap = NULL;

CDeadLockDetectMutex::CDeadLockDetectMutex()
{
	if( gpclsMapMutex == NULL )
	{
		gpclsMapMutex = new CSipMutex();
	}

	if( gpclsMapMutex )
	{
		gpclsMapMutex->acquire();
		if( gpclsMutexPointMap == NULL )
		{
			gpclsMutexPointMap = new CMutexPointerMap();
		}

		if( gpclsMutexPointMap )
		{
			gpclsMutexPointMap->m_clsMap.insert( MUTEX_POINTER_MAP::value_type( this, 1 ) );
		}
		gpclsMapMutex->release();
	}
}

CDeadLockDetectMutex::~CDeadLockDetectMutex()
{
	if( gpclsMapMutex )
	{
		MUTEX_POINTER_MAP::iterator itMap;

		gpclsMapMutex->acquire();
		itMap = gpclsMutexPointMap->m_clsMap.find( this );
		if( itMap != gpclsMutexPointMap->m_clsMap.end() )
		{
			gpclsMutexPointMap->m_clsMap.erase( itMap );
		}
		gpclsMapMutex->release();
	}
}

bool CDeadLockDetectMutex::acquire()
{
	THREAD_ID_LIST::iterator itTL;
	THREAD_ID_TYPE iThreadId = GetCurrentThreadId();

	m_clsListMutex.acquire();

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
#endif

	m_clsThreadIdList.push_back( iThreadId );

	if( m_clsThreadIdList.size() >= 2 && gpclsMapMutex && gpclsMutexPointMap )
	{
		// 2개의 쓰레드에서 dead lock 되었는지 검사한다.
		MUTEX_POINTER_MAP::iterator itMap;
		THREAD_ID_LIST::iterator itCTL;

		gpclsMapMutex->acquire();
		for( itMap = gpclsMutexPointMap->m_clsMap.begin(); itMap != gpclsMutexPointMap->m_clsMap.end(); ++itMap )
		{
			// 현재 mutex 에 대해서는 검사하지 않는다.
			if( itMap->first == this ) continue;

			// 다른 mutex 에서 현재 thread 의 lock 다음에 존재하는 thread 중에서 현재 mutex 에 존재하는 쓰레드가 존재하면 dead lock 이다.
			bool bFoundMe = false;

			for( itTL = itMap->first->m_clsThreadIdList.begin(); itTL != itMap->first->m_clsThreadIdList.end(); ++itTL )
			{
				if( *itTL == iThreadId )
				{
					bFoundMe = true;
				}
				else if( bFoundMe )
				{
					for( itCTL = m_clsThreadIdList.begin(); itCTL != m_clsThreadIdList.end(); ++itCTL )
					{
						if( *itCTL == *itTL )
						{
							CLog::Print( LOG_ERROR, "dead lock in multi thread - thread#1(" THREAD_ID_FORMAT ") thread#2(" THREAD_ID_FORMAT ")", iThreadId, *itTL );
							CLog::PrintCallStack( LOG_ERROR );
						}
					}
				}
			}
		}
		gpclsMapMutex->release();
	}

	m_clsListMutex.release();

	return m_clsMutex.acquire();
}

bool CDeadLockDetectMutex::release()
{
	THREAD_ID_LIST::reverse_iterator itTL;
	THREAD_ID_TYPE iThreadId = GetCurrentThreadId();

	m_clsListMutex.acquire();

	for( itTL = m_clsThreadIdList.rbegin(); itTL != m_clsThreadIdList.rend(); ++itTL )
	{
		if( *itTL == iThreadId )
		{
			m_clsThreadIdList.erase( --itTL.base() );
			break;
		}
	}

	m_clsListMutex.release();

	return m_clsMutex.release();
}
