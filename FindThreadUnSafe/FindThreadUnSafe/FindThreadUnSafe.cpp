#include "SipPlatformDefine.h"
#include <stdio.h>
#include "Directory.h"
#include "FileUtility.h"

bool CheckLine( const char * pszLine );

bool IsC( const char * pszExt )
{
	if( !strcmp( pszExt, "c" ) ||
			!strcmp( pszExt, "cpp" ) ||
			!strcmp( pszExt, "hpp" ) ||
			!strcmp( pszExt, "h" ) ||
			!strcmp( pszExt, "m" ) )
	{
		return true;
	}

	return false;
}

bool CheckFile( const char * pszFileName )
{
	FILE * fd = fopen( pszFileName, "r" );
	if( fd == NULL ) return false;

	char szBuf[8192];
	int iLine = 0;

	memset( szBuf, 0, sizeof(szBuf) );

	while( fgets( szBuf, sizeof(szBuf)-1, fd ) )
	{
		++iLine;

		if( CheckLine( szBuf ) == false )
		{
			printf( "file(%s) line(%d) %s\n", pszFileName, iLine, szBuf );
		}

		memset( szBuf, 0, sizeof(szBuf) );
	}

	fclose( fd );

	return true;
}

bool CheckDirectory( const char * pszDirName )
{
	FILE_LIST clsFileList;
	FILE_LIST::iterator itFL;
	std::string strFileName, strExt;

	CDirectory::List( pszDirName, clsFileList );

	for( itFL = clsFileList.begin(); itFL != clsFileList.end(); ++itFL )
	{
		strFileName = pszDirName;
		CDirectory::AppendName( strFileName, itFL->c_str() );

		if( CDirectory::IsDirectory( strFileName.c_str() ) == 0 )
		{
			CheckDirectory( strFileName.c_str() );
		}
		else
		{
			GetFileExt( itFL->c_str(), strExt );

			if( IsC( strExt.c_str() ) )
			{
				CheckFile( strFileName.c_str() );
			}
		}
	}

	return true;
}

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {dir path}\n", argv[0] );
		return 0;
	}

	CheckDirectory( argv[1] );

	return 0;
}
