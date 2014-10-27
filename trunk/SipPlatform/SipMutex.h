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

#ifndef _SIP_MUTEX_H_
#define _SIP_MUTEX_H_

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

/** 
 * @ingroup SipPlatform
 * @brief mutex ����� �����ϴ� Ŭ����
 */
class CSipMutex
{
public:
	CSipMutex();
	~CSipMutex();
	
	bool acquire();
	bool release();

protected:
#ifdef WIN32
	CRITICAL_SECTION m_sttMutex;
#else
	pthread_mutex_t	 m_sttMutex;
#endif
};

/** 
 * @ingroup SipPlatform
 * @brief mutex ��� �� wait/signal ����� �����ϴ� Ŭ����. �������� ����ȭ�Ǿ� �ְ� �����쿡�� ����ȭ�Ǿ� ���� ����.
 */
class CSipMutexSignal : public CSipMutex
{
public:
	CSipMutexSignal();
	~CSipMutexSignal();
	
	bool wait();
	bool signal();
	bool broadcast();

private:
#ifdef WIN32
	CONDITION_VARIABLE		m_sttCond;
#else
	pthread_cond_t		m_sttCond;
#endif
};

#endif
