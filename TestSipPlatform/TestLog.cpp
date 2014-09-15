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

#include "Log.h"
#include "Directory.h"

bool TestLog()
{
	const char * pszFolder;

#ifdef WIN32
	pszFolder = "c:\\temp\\log_test";
#else
	pszFolder = "/tmp/log_test";
#endif

	// 기존 로그를 모두 삭제한다.
	CDirectory::DeleteAllFile( pszFolder );
	int64_t iFolderSize = CDirectory::GetSize( pszFolder );
	if( iFolderSize != 0 )
	{
		printf( "%s folder(%s) size(" LONG_LONG_FORMAT ") is not 0\n", __FUNCTION__, pszFolder, iFolderSize );
		return false;
	}

	CLog::SetDirectory( pszFolder );
	CLog::SetMaxLogSize( 1024*1024 );
	CLog::SetMaxFolderSize( 1024*1024*30 );
	CLog::SetLevel( LOG_DEBUG );

	char	szBuf[1024];

	memset( szBuf, 0, sizeof(szBuf) );

	for( int i = 0; i < 24; ++i )
	{
		for( int j = 0; j < 30; ++j )
		{
			szBuf[i+24*j] = 'a' + i;
		}
	}

	for( int i = 0; i < 200000; ++i )
	{
		CLog::Print( LOG_DEBUG, "%s", szBuf );
		iFolderSize = CDirectory::GetSize( pszFolder );

		if( iFolderSize >= 1024*1024*31 )
		{
			printf( "%s folder(%s) size(" LONG_LONG_FORMAT ") error\n", __FUNCTION__, pszFolder, iFolderSize );
			return false;
		}
	}

	return true;
}

#define LOGE(fmt,...) CLog::Print( LOG_ERROR, fmt " (%s:%d)", ##__VA_ARGS__, __FUNCTION__, __LINE__ )

bool TestLogMacro()
{
	LOGE( "Test int(%d) string(%s)", 1, "test" );

	return true;
}
