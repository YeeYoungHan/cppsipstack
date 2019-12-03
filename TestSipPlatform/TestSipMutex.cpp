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
#include "ServerUtility.h"
#include "DeadLockDetectMutex.h"
#include <map>

#define TEST_COUNT 10

static CSipMutexSignal gclsMutexSignal;

#ifdef WIN32
void PrintTickCount( const char * pszName )
{
	printf( "%s tick[%u]\n", pszName, GetTickCount() );
}

THREAD_API TestSipMutexThread( LPVOID lpParameter )
{
	for( int i = 0; i < TEST_COUNT; ++i )
	{
		Sleep(20);
		gclsMutexSignal.acquire();
		PrintTickCount( "signal" );
		gclsMutexSignal.signal();
		gclsMutexSignal.release();
	}

	return 0;
}
#endif

bool TestSipMutex()
{
#ifdef WIN32
	if( StartThread( "TestSipMutexThread", TestSipMutexThread, NULL ) == false ) return false;

	for( int i = 0; i < TEST_COUNT; ++i )
	{
		PrintTickCount( "Start" );
		gclsMutexSignal.acquire();
		PrintTickCount( "wait" );
		gclsMutexSignal.wait();
		gclsMutexSignal.release();
		PrintTickCount( "End  " );
	}
#endif

	return true;
}

class CTestMutex
{
public:
	CSipMutex m_clsMutex;
};

typedef std::map< int, CTestMutex > TEST_MUTEX_MAP;
TEST_MUTEX_MAP clsMap;

void TestSipMutexDeadInsert()
{
	CTestMutex clsMutex;

	/* 리눅스에서 CTestMutex 객체의 내용은 다음과 같다.
(gdb) p clsMutex
$2 = {m_clsMutex = {m_sttMutex = {__data = {__lock = 1, __count = 0,
        __owner = 4230464, __nusers = 0, __kind = 0, __spins = 0, __list = {
          __prev = 0x401c3b, __next = 0x7fffffffe5f8}},
      __size = "\001\000\000\000\000\000\000\000@\215@", '\000' <repeats 13 times>, ";\034@\000\000\000\000\000\370\345\377\377\377\177\000", __align = 1}}}
	*/

	clsMap.insert( TEST_MUTEX_MAP::value_type( 1, clsMutex ) );

	/* 리눅스에서 std::map 에 저장된 CSipMutex 는 아래와 같이 정상적이지 않은 값들이 저장되어 있는 것을 확인할 수 있다.
(gdb) p clsMap
$5 = std::map with 1 elements = {
  [1] = {m_clsMutex = {m_sttMutex = {__data = {__lock = 0, __count = 0,
          __owner = 0, __nusers = 0, __kind = 0, __spins = 0, __list = {
            __prev = 0x0, __next = 0x0}}, __size = '\000' <repeats 39 times>,
        __align = 0}}}
}
	*/
}

void TestSipMutexDeadSelect()
{
	TEST_MUTEX_MAP::iterator itMap;

	itMap = clsMap.find( 1 );
	if( itMap != clsMap.end() )
	{
		itMap->second.m_clsMutex.acquire();
		itMap->second.m_clsMutex.release();
	}
}

/**
 * @ingroup TestSipPlatform
 * @brief CSipMutex 변수를 std::map 또는 std::list 등과 같은 자료구조의 값으로 저장할 때의 문제점 분석을 위한 테스트 함수
 */
void TestSipMutexList()
{
	TestSipMutexDeadInsert();
	TestSipMutexDeadSelect();
}

void TestSipMutexDeadLock()
{
	CDeadLockDetectMutex clsMutex;

	clsMutex.acquire();
	clsMutex.acquire();
	clsMutex.release();
	clsMutex.release();
}
