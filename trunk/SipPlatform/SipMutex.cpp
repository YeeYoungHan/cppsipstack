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

#include "SipMutex.h"
#include "MemoryDebug.h"

/**
 * @ingroup SipPlatform
 * @brief ������
 */
CSipMutex::CSipMutex( )
{
#ifdef WIN32
	InitializeCriticalSection( &m_sttMutex );
#else
	pthread_mutex_init( &m_sttMutex, NULL );
#endif
}

/**
 * @ingroup SipPlatform
 * @brief �Ҹ���
 */
CSipMutex::~CSipMutex()
{
#ifdef WIN32
	DeleteCriticalSection( &m_sttMutex );
#else
	pthread_mutex_destroy( &m_sttMutex );
#endif
}

/**
 * @ingroup SipPlatform
 * @brief mutex lock �Ѵ�.
 * @return true �� �����Ѵ�.
 */
bool CSipMutex::acquire()
{
#ifdef WIN32
	EnterCriticalSection( &m_sttMutex );
#else
	if( pthread_mutex_lock( &m_sttMutex ) != 0 )
	{
		return false;
	}
#endif

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief mutex unlock �Ѵ�.
 * @return true �� �����Ѵ�.
 */
bool CSipMutex::release()
{
#ifdef WIN32
	LeaveCriticalSection( &m_sttMutex );
#else
	if( pthread_mutex_unlock( &m_sttMutex ) != 0 )
	{
		return false;
	}
#endif

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief ������
 */
CSipMutexSignal::CSipMutexSignal()
{
#ifdef WIN32
	InitializeConditionVariable( &m_sttCond );
#else
	pthread_cond_init( &m_sttCond, NULL );
#endif
}

/**
 * @ingroup SipPlatform
 * @brief �Ҹ���
 */
CSipMutexSignal::~CSipMutexSignal()
{
#ifdef WIN32
#else
	pthread_cond_destroy( &m_sttCond );
#endif
}

/**
 * @ingroup SipPlatform
 * @brief signal �Ǵ� broadcast �޼ҵ尡 ȣ��� ������ ����Ѵ�.
 * @return true �� �����Ѵ�.
 */
bool CSipMutexSignal::wait()
{
#ifdef WIN32
	SleepConditionVariableCS( &m_sttCond, &m_sttMutex, INFINITE );

	return true;
#else
	int n = pthread_cond_wait( &m_sttCond, &m_sttMutex );
	if( n != 0 )
	{
		return false;
	}
#endif

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief signal �Ǵ� broadcast �޼ҵ尡 ȣ��� ������ ����ϴ� ������ 1���� ��� ������Ų��.
 * @return true �� �����Ѵ�.
 */
bool CSipMutexSignal::signal()
{
#ifdef WIN32
	WakeConditionVariable( &m_sttCond );
#else
	int n = pthread_cond_signal( &m_sttCond );
	if( n != 0 )
	{
		return false;
	}
#endif

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief signal �Ǵ� broadcast �޼ҵ尡 ȣ��� ������ ����ϴ� ��� �����带 ��� ������Ų��.
 * @return true �� �����Ѵ�.
 */
bool CSipMutexSignal::broadcast()
{
#ifdef WIN32
	WakeAllConditionVariable( &m_sttCond );
#else
	int n = pthread_cond_broadcast( &m_sttCond );
	if( n != 0 )
	{
		return false;
	}
#endif

	return true;
}
