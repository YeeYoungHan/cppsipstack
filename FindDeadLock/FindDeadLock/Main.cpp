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
#include "../../SipPlatform/Directory.h"
#include "../../SipPlatform/FileUtility.h"
#include "FindDeadLock.h"

bool IsSourceCode( const char * pszExt )
{
	static char * arrExt[] = { "cpp", "c", "hpp", "h", NULL };

	for( int i = 0; arrExt[i]; ++i )
	{
		if( !strcmp( arrExt[i], pszExt ) )
		{
			return true;
		}
	}

	return false;
}

void FindDeadLockWithDir( const char * pszDir )
{
	FILE_LIST clsFileList;
	FILE_LIST::iterator itFL;
	std::string strFileName, strExt;

	CDirectory::List( pszDir, clsFileList );

	for( itFL = clsFileList.begin(); itFL != clsFileList.end(); ++itFL )
	{
		strFileName = pszDir;
		CDirectory::AppendName( strFileName, itFL->c_str() );

		if( CDirectory::IsDirectory( strFileName.c_str() ) )
		{
			FindDeadLockWithDir( strFileName.c_str() );
		}
		else if( GetFileExt( itFL->c_str(), strExt ) && IsSourceCode( strExt.c_str() ) )
		{
			FindDeadLock( strFileName.c_str() );
		}
	}
}

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {folder path}\n", argv[0] );
		return 0;
	}

	FindDeadLockWithDir( argv[1] );

	return 0;
}
