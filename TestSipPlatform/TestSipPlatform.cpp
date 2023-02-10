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

#include <stdio.h>
#include "Directory.h"
#include "TestSipPlatform.h"

/**
 * @ingroup TestSipParser
 * @brief SipParser / SdpParser / XmlParser ���̺귯�� ����� �׽�Ʈ�Ѵ�.
 * @param argc 
 * @param argv 
 * @returns 0 �� �����Ѵ�.
 */
int main( int argc, char * argv[] )
{
	if( argc == 2 )
	{
		if( !strcmp( argv[1], "-dls" ) )
		{
			// 1���� �����忡�� dead lock �˻�
			TestSipMutexDeadLockSingleThread();
			return 0;
		}
		else if( !strcmp( argv[1], "-dlm" ) )
		{
			// dead lock �� �����ϴ� 2���� ������ ����
			TestSipMutexDeadLockMultiThread();
			return 0;
		}
		else if( !strcmp( argv[1], "-nm" ) )
		{
			// dead lock �� ���� 2���� ������ ����
			TestSipMutexNoDeadLockMultiThread();
			return 0;
		}
		else if( !strcmp( argv[1], "-m" ) )
		{
			TestSipMutexList();
			return 0;
		}
		else if( !strcmp( argv[1], "-f" ) )
		{
			TestFileUtility();
			return 0;
		}
#ifndef WIN32
		else if( !strcmp( argv[1], "-fd" ) )
		{
			TestFileFd();
			TestSocketFd();
			return 0;
		}
#endif
		else if( !strcmp( argv[1], "-t" ) )
		{
			TestThreadCount();
			return 0;
		}
	}
	else if( argc == 3 )
	{
		if( !strcmp( argv[1], "-d" ) )
		{
			CDirectory::Delete( argv[2] );
			return 0;
		}
	}

	bool bRes = false;

	//if( TestLog() == false ) goto FUNC_END;
	if( TestRandom() == false ) goto FUNC_END;
	if( TestLogMacro() == false ) goto FUNC_END;
	if( TestStringUtility() == false ) goto FUNC_END;
	if( TestServerUtility() == false ) goto FUNC_END;
	if( TestStringSort() == false ) goto FUNC_END;
	if( TestDirectory() == false ) goto FUNC_END;
	if( TestSipMutex() == false ) goto FUNC_END;

	bRes = true;
	
FUNC_END:
	if( bRes )
	{
		printf( "All test is O.K.\n" );
	}
	else
	{
		printf( "ERROR!!!\n" );
	}

	return 0;
}
