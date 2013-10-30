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
#include "ServerUtility.h"

#define TEST_COUNT 10

static CSipMutexSignal gclsMutexSignal;

void PrintTickCount( const char * pszName )
{
	printf( "%s tick[%u]\n", pszName, GetTickCount() );
}

DWORD WINAPI TestSipMutexThread( LPVOID lpParameter )
{
	for( int i = 0; i < TEST_COUNT; ++i )
	{
		Sleep(20);
		gclsMutexSignal.signal();
	}

	return 0;
}

bool TestSipMutex()
{
	if( StartThread( "TestSipMutexThread", TestSipMutexThread, NULL ) == false ) return false;

	for( int i = 0; i < TEST_COUNT; ++i )
	{
		PrintTickCount( "Start" );
		gclsMutexSignal.wait();
		PrintTickCount( "End  " );
	}

	return true;
}
