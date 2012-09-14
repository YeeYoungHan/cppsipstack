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

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <string>

char * gpszLicense = "Copyright (C) 2012 Yee Young Han <websearch@naver.com>";
char * garrExt[] = { ".h", ".hpp", ".cpp", NULL };
char * garrExcludeFolder[] = { "Curio", "MySQL Server 5.0", NULL };

bool IsFileExt( const char * pszFileName )
{
	int iLen = strlen( pszFileName );

	for( int i = 0; garrExt[i]; ++i )
	{
		int iExtLen = strlen( garrExt[i] );

		if( iLen <= iExtLen ) continue;

		if( !strcmp( pszFileName + iLen - iExtLen, garrExt[i] ) )
		{
			return true;
		}
	}

	return false;
}

bool IsExcludeFolder( const char * pszFileName )
{
	for( int i = 0; garrExcludeFolder[i]; ++i )
	{
		if( !strcmp( pszFileName, garrExcludeFolder[i] ) )
		{
			return true;
		}
	}

	return false;
}

bool CheckLicense( const char * pszFileName )
{
	FILE	* fd;
	bool	bFound = false;
	char	szBuf[1024];

	fd = fopen( pszFileName, "r" );
	if( fd == NULL )
	{
		printf( "fopen(%s) error\n", pszFileName );
		return false;
	}

	while( fgets( szBuf, sizeof(szBuf), fd ) )
	{
		if( strstr( szBuf, gpszLicense ) )
		{
			bFound = true;
			break;
		}
	}

	fclose( fd );

	if( bFound == false )
	{
		printf( "file(%s) is no license\n", pszFileName );
	}

	return bFound;
}

void FindNoLicenseFile( const char * pszDirName )
{
	WIN32_FIND_DATA	sttFindData;
	HANDLE			hFind;
	BOOL				bNext = TRUE;
	std::string	strPath = pszDirName;
	std::string strFileName;

	strPath.append( "\\*.*" );

	hFind = FindFirstFile( strPath.c_str(), &sttFindData );
	if( hFind == INVALID_HANDLE_VALUE )
	{
		printf( "FindFirstFile(%s) error(%d)", pszDirName, GetLastError() );
		return;
	}

	for( ; bNext == TRUE; bNext = FindNextFile( hFind, &sttFindData ) )
	{
		if( !strcmp( sttFindData.cFileName, "." ) || !strcmp( sttFindData.cFileName, ".." ) ) continue;

		strFileName = pszDirName;
		strFileName.append( "\\" );
		strFileName.append( sttFindData.cFileName );

		if( sttFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if( IsExcludeFolder( sttFindData.cFileName ) ) continue;

			FindNoLicenseFile( strFileName.c_str() );
		}
		else
		{
			if( IsFileExt( sttFindData.cFileName ) )
			{
				CheckLicense( strFileName.c_str() );				
			}
		}
	}

	FindClose( hFind );
}

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {folder path}\n", argv[0] );
		return -1;
	}

	FindNoLicenseFile( argv[1] );

	return 0;
}

