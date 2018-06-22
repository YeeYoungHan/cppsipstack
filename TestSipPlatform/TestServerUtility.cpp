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

#include "FileUtility.h"
#include "ServerUtility.h"
#include "Directory.h"

#ifdef WIN32
#define TEST_FOLDER "c:\\temp\\sipserver"
#else
#include <unistd.h>

#define TEST_FOLDER "/tmp/sipserver"
#endif

bool TestServerUtility()
{
	int64_t iSize;
	FILE_LIST clsFileList;

	CDirectory::FileList( TEST_FOLDER, clsFileList );

	if( clsFileList.size() > 0 )
	{
		std::string strFileName = TEST_FOLDER;

		CDirectory::AppendName( strFileName, clsFileList.begin()->c_str() );

		iSize = GetFileSize( strFileName.c_str() );
		if( iSize == 0 )
		{
			printf( "[ERROR] GetFileSize(%s)\n", strFileName.c_str() );
			return false;
		}

		printf( "GetFileSize(%s) = " LONG_LONG_FORMAT "\n", strFileName.c_str(), iSize );
	}

	return true;
}

static bool gbEnd = false;

THREAD_API _TestThreadCount( LPVOID lpParameter )
{
	while( gbEnd == false )
	{
		sleep(1);
	}

	return 0;
}

void TestThreadCount()
{
	int iCount = 0;

	while( 1 )
	{
		if( StartThread( "_TestThreadCount", _TestThreadCount, NULL ) == false )
		{
			break;
		}

		++iCount;
		printf( "%d\n", iCount );
	}

	gbEnd = true;

	sleep(3);
}	
