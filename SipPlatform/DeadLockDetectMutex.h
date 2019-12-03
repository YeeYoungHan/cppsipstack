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

#ifndef _DEAD_LOCK_DETECT_MUTEX_H_
#define _DEAD_LOCK_DETECT_MUTEX_H_

#include "SipMutex.h"
#include <list>

#ifdef WIN32
#define THREAD_ID_TYPE DWORD
#else
#define THREAD_ID_TYPE pthread_t
#define GetCurrentThreadId pthread_self
#endif

typedef std::list< THREAD_ID_TYPE > THREAD_ID_LIST;

class CDeadLockDetectMutex
{
public:
	CDeadLockDetectMutex();
	~CDeadLockDetectMutex();

	bool acquire();
	bool release();

private:
	CSipMutex m_clsMutex;
	CSipMutex m_clsPrivateMutex;
	THREAD_ID_LIST m_clsThreadIdList;
};

#endif
