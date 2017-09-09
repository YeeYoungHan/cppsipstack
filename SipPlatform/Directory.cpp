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

#include <string.h>
#include "Directory.h"
#include "Log.h"

#ifndef WIN32
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#endif

#include "MemoryDebug.h"

/** 
 * @ingroup SipPlatform
 * @brief 디렉토리를 생성한다. 
 *	
 *	이미 디렉토리가 생성되어 있으면 아무런 동작을 하지 않는다.
 *	"c:\temp\test" 와 같은 문자열을 입력하면 다음과 같이 동작하다.
 *	"c:\temp" 디렉토리가 존재하지 않으면 이를 생성한 후, "c:\temp\test" 디렉토리를 생성한다.
 *
 * @param	szDirName	[in] 생성할 디렉토리의 full pathname
 * @param iDirMode	[in] 생성할 디렉토리의 권한
 * @return	성공하면 true 을 리턴하고 실패하면 false 를 리턴한다.
 */
bool CDirectory::Create( const char * szDirName, int iDirMode )
{
	int		i, iLen, iCount, n;
	char	* pszName;
	char	fisNotDirectory = 0;	// 디렉토리가 아닌가?
	char	fisError = 0;			// 디렉토리 생성시에 에러가 발생하였는가?

	iLen = (int)strlen( szDirName );
	pszName = new char[ iLen + 1 ];

	memset( pszName, 0, iLen + 1 );
	for( i = 0, iCount = 0; i < iLen; i++ )
	{
		if( szDirName[i] == DIR_SEP )
		{
			iCount++;

			// 디렉토리 이름이 "c:\test\temp\" 또는 "/test/temp/" 이므로 두번째 디렉토리
			// 구분자 부터 디렉토리를 생성하면 된다.
			if( iCount >= 2 )
			{
				n = CDirectory::IsDirectoryCheck( pszName );
				if( n == -1 )
				{
					// 디렉토리가 아닌 경우
					fisNotDirectory = 1;
					break;
				}
				else if( n == -2 )
				{
					// 디렉토리가 존재하지 않는 경우
#ifdef WIN32
					if( CreateDirectory( pszName, NULL ) == FALSE )
#else
					if( mkdir( pszName, iDirMode ) != 0 )
#endif
					{
						fisError = 1;
						break;
					}
				}
			}
		}

		pszName[i] = szDirName[i];
	}

	delete [] pszName;

	if( fisNotDirectory == 1 ) return false;
	if( fisError == 1 ) return false;

	// 디렉토리 이름이 "c:\test\temp" 또는 "/test/temp" 일 경우, 위의 loop 에서 temp 디렉토리를 
	// 생성하지 않으므로 이를 생성하기 위해서 아래의 코드가 필요하다.
	if( szDirName[iLen-1] != DIR_SEP )
	{
		n = CDirectory::IsDirectoryCheck( szDirName );
		if( n == -1 )
		{
			// 디렉토리가 아닌 경우
			return false;
		}
		else if( n == -2 )
		{
			// 디렉토리가 존재하지 않는 경우
#ifdef WIN32
			if( CreateDirectory( szDirName, NULL ) == FALSE )
#else
			if( mkdir( szDirName, iDirMode ) != 0 )
#endif
			{
				return false;
			}
		}
	}

	return true;
}

/** 
 * @ingroup SipPlatform
 * @brief 사용자가 입력한 path 가 디렉토리인지를 점검한다.
 * @param	szDirName	[in] 디렉토리 이름
 * @return	입력된 path 가 디렉토리이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CDirectory::IsDirectory( const char * szDirName )
{
	if( CDirectory::IsDirectoryCheck( szDirName ) == 0 ) return true;

	return false;
}

/** 
 * @ingroup SipPlatform
 * @brief 사용자가 입력한 path 가 디렉토리인지를 점검한다.
 * @param	szDirName	[in] 디렉토리 이름
 * @return	입력된 path 가 디렉토리이면 0 을 리턴한다.
 *					존재하지 않으면 -2 을 리턴한다.
 *					디렉토리가 아니면 -1 을 리턴한다.
 */
int CDirectory::IsDirectoryCheck( const char * szDirName )
{
#ifdef WIN32
	// ACE 에서 S_ISDIR 을 지원하지 않기 때문에 윈도우의 경우를 위하여 WINAPI 를 사용하였음.
	DWORD iAttribute = GetFileAttributes( szDirName );

	if( iAttribute != -1 ) 
	{
		if( iAttribute & FILE_ATTRIBUTE_DIRECTORY )
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		// 파일이 존재하지 않는 경우.
		return -2;	
	}
#else
	struct stat		clsStat;

	if( stat( szDirName, &clsStat ) == 0 )
	{
		if( S_ISDIR( clsStat.st_mode ) ) 
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		// 파일이 존재하지 않는 경우.
		return -2;	
	}
#endif

	return 0;
}

/**
 * @ingroup SipPlatform
 * @brief 파일 경로에 파일 이름을 추가한다.
 * @param strFileName 파일 경로
 * @param pszAppend		추가할 파일 이름
 */
void CDirectory::AppendName( std::string & strFileName, const char * pszAppend )
{
#ifdef WIN32
	strFileName.append( "\\" );
	strFileName.append( pszAppend );
#else
	strFileName.append( "/" );
	strFileName.append( pszAppend );
#endif
}

/**
 * @ingroup SipPlatform
 * @brief 폴더에 존재하는 모든 파일/폴더 리스트를 가져온다.
 * @param pszDirName	폴더 경로
 * @param clsFileList 파일/폴더 리스트를 저장할 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CDirectory::List( const char * pszDirName, FILE_LIST & clsFileList )
{
	clsFileList.clear();

#ifdef WIN32
	WIN32_FIND_DATA	sttFindData;
	HANDLE			hFind;
	BOOL				bNext = TRUE;
	std::string	strPath = pszDirName;

	strPath.append( "\\*.*" );

	hFind = FindFirstFile( strPath.c_str(), &sttFindData );
	if( hFind == INVALID_HANDLE_VALUE )
	{
		CLog::Print( LOG_ERROR, "FindFirstFile(%s) error(%d)", pszDirName, GetLastError() );
		return false;
	}

	for( ; bNext == TRUE; bNext = FindNextFile( hFind, &sttFindData ) )
	{
		if( !strcmp( sttFindData.cFileName, "." ) || !strcmp( sttFindData.cFileName, ".." ) ) continue;
		clsFileList.push_back( sttFindData.cFileName );
	}

	FindClose( hFind );
#else
	DIR						* psttDir;
	struct dirent	* psttDirent, sttDirent;
	int	n;

	psttDir = opendir( pszDirName );
	if( psttDir == NULL )
	{
		CLog::Print( LOG_ERROR, "opendir(%s) error(%d)", pszDirName, errno );
		return false;
	}

	for( n = readdir_r( psttDir, &sttDirent, &psttDirent ); psttDirent && n == 0; n = readdir_r( psttDir, &sttDirent, &psttDirent ) )
	{
		if( !strcmp( psttDirent->d_name, "." ) || !strcmp( psttDirent->d_name, ".." ) ) continue;

		clsFileList.push_back( psttDirent->d_name );
	}

	closedir( psttDir );
#endif

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief 폴더에 존재하는 모든 파일 리스트를 가져온다.
 * @param pszDirName	폴더 경로
 * @param clsFileList 파일 리스트를 저장할 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CDirectory::FileList( const char * pszDirName, FILE_LIST & clsFileList )
{
	clsFileList.clear();

#ifdef WIN32
	WIN32_FIND_DATA	sttFindData;
	HANDLE			hFind;
	BOOL				bNext = TRUE;
	std::string	strPath = pszDirName;

	strPath.append( "\\*.*" );

	hFind = FindFirstFile( strPath.c_str(), &sttFindData );
	if( hFind == INVALID_HANDLE_VALUE )
	{
		CLog::Print( LOG_ERROR, "FindFirstFile(%s) error(%d)", pszDirName, GetLastError() );
		return false;
	}

	for( ; bNext == TRUE; bNext = FindNextFile( hFind, &sttFindData ) )
	{
		if( !strcmp( sttFindData.cFileName, "." ) || !strcmp( sttFindData.cFileName, ".." ) ) continue;
		if( sttFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) continue;

		clsFileList.push_back( sttFindData.cFileName );
	}

	FindClose( hFind );
#else
	DIR						* psttDir;
	struct dirent	* psttDirent, sttDirent;
	struct stat		sttStat;
	int		n;
	std::string		strFileName;

	psttDir = opendir( pszDirName );
	if( psttDir == NULL )
	{
		CLog::Print( LOG_ERROR, "opendir(%s) error(%d)", pszDirName, errno );
		return false;
	}

	for( n = readdir_r( psttDir, &sttDirent, &psttDirent ); psttDirent && n == 0; n = readdir_r( psttDir, &sttDirent, &psttDirent ) )
	{
		if( !strcmp( psttDirent->d_name, "." ) || !strcmp( psttDirent->d_name, ".." ) ) continue;
		strFileName = pszDirName;
		AppendName( strFileName, psttDirent->d_name );

		if( lstat( strFileName.c_str(), &sttStat ) < 0 ) continue;
		if( S_ISDIR( sttStat.st_mode ) ) continue;

		clsFileList.push_back( psttDirent->d_name );
	}

	closedir( psttDir );
#endif

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief 프로그램을 시작한 폴더를 가져온다.
 * @returns 프로그램을 시작한 폴더 경로를 리턴한다.
 */
char * CDirectory::GetProgramDirectory( )
{
	static char	szDir[1024];

	if( strlen(szDir) == 0 )
	{
#ifdef WIN32
		int		i;
		HMODULE	hThis;

		hThis = GetModuleHandle( NULL );

		GetModuleFileName( hThis, szDir, sizeof(szDir));
		for( i = (int)strlen( szDir) - 1; i >= 0; i-- )
		{
			if( szDir[i] == '\\' ) 
			{
				szDir[i] = '\0';
				break;
			}
		}
#endif
	}

	return szDir;
}

#define MAX_INT	4294967296L

/**
 * @ingroup SipPlatform
 * @brief 폴더 크기를 가져온다.
 * @param pszDirName 폴더 fulll path
 * @returns 폴더 크기를 리턴한다.
 */
int64_t CDirectory::GetSize( const char * pszDirName )
{
	uint64_t iTotalSize = 0;

#ifdef WIN32
	WIN32_FIND_DATA	sttFindData;
	HANDLE			hFind;
	BOOL				bNext = TRUE;
	std::string	strPath = pszDirName;

	strPath.append( "\\*.*" );

	hFind = FindFirstFile( strPath.c_str(), &sttFindData );
	if( hFind == INVALID_HANDLE_VALUE )
	{
		CLog::Print( LOG_ERROR, "FindFirstFile(%s) error(%d)", pszDirName, GetLastError() );
		return false;
	}

	for( ; bNext == TRUE; bNext = FindNextFile( hFind, &sttFindData ) )
	{
		if( !strcmp( sttFindData.cFileName, "." ) || !strcmp( sttFindData.cFileName, ".." ) ) continue;
		if( sttFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			std::string	strFolder = pszDirName;
			
			AppendName( strFolder, sttFindData.cFileName );
			iTotalSize += GetSize( strFolder.c_str() );
			continue;
		}
		
		if( sttFindData.nFileSizeHigh > 0 )
		{
			iTotalSize += (uint64_t)sttFindData.nFileSizeHigh * MAX_INT;
		}

		iTotalSize += sttFindData.nFileSizeLow;
	}

	FindClose( hFind );
#else
	DIR						* psttDir;
	struct dirent	* psttDirent, sttDirent;
	struct stat		sttStat;
	int		n;
	std::string		strFileName;

	psttDir = opendir( pszDirName );
	if( psttDir == NULL )
	{
		CLog::Print( LOG_ERROR, "opendir(%s) error(%d)", pszDirName, errno );
		return false;
	}

	for( n = readdir_r( psttDir, &sttDirent, &psttDirent ); psttDirent && n == 0; n = readdir_r( psttDir, &sttDirent, &psttDirent ) )
	{
		if( !strcmp( psttDirent->d_name, "." ) || !strcmp( psttDirent->d_name, ".." ) ) continue;
		strFileName = pszDirName;
		AppendName( strFileName, psttDirent->d_name );

		if( lstat( strFileName.c_str(), &sttStat ) < 0 ) continue;
		if( S_ISDIR( sttStat.st_mode ) )
		{
			std::string	strFolder = pszDirName;
			
			AppendName( strFolder, psttDirent->d_name );
			iTotalSize += GetSize( strFolder.c_str() );
			continue;
		}

		iTotalSize += sttStat.st_size;
	}

	closedir( psttDir );
#endif

	return iTotalSize;
}

/**
 * @ingroup SipPlatform
 * @brief 폴더에 포함된 파일들을 모두 삭제한다.
 * @param pszDirName 폴더 full path
 */
void CDirectory::DeleteAllFile( const char * pszDirName )
{
	FILE_LIST::iterator	itFile;
	FILE_LIST clsFileList;

	FileList( pszDirName, clsFileList );

	for( itFile = clsFileList.begin(); itFile != clsFileList.end(); ++itFile )
	{
		std::string strFileName = pszDirName;

		CDirectory::AppendName( strFileName, itFile->c_str() );

#ifdef WIN32
		DeleteFile( strFileName.c_str() );
#else
		unlink( strFileName.c_str() );
#endif		
	}
}

/**
 * @ingroup SipPlatform
 * @brief 파일 경로에서 폴더 이름을 가져온다.
 * @param pszFilePath 파일 경로
 * @param strDirName	폴더 이름을 저장할 변수
 */
void CDirectory::GetDirName( const char * pszFilePath, std::string & strDirName )
{
	int iLen = (int)strlen( pszFilePath );

	strDirName.clear();

	for( int i = iLen - 1; i >= 0; --i )
	{
#ifdef WIN32
		if( pszFilePath[i] == '\\' )
#else
		if( pszFilePath[i] == '/' )
#endif
		{
			strDirName.append( pszFilePath, i );
			break;
		}
	}
}

/**
 * @ingroup SipPlatform
 * @brief 파일 경로에서 파일 이름을 가져온다.
 * @param pszFilePath 파일 경로
 * @param strFileName 파일 이름을 저장할 변수
 */
void CDirectory::GetFileName( const char * pszFilePath, std::string & strFileName )
{
	int iLen = (int)strlen( pszFilePath );

	strFileName.clear();

	for( int i = iLen - 1; i >= 0; --i )
	{
#ifdef WIN32
		if( pszFilePath[i] == '\\' )
#else
		if( pszFilePath[i] == '/' )
#endif
		{
			strFileName = pszFilePath + i + 1;
			break;
		}
	}
}

/**
 * @ingroup SipPlatform
 * @brief 폴더를 모두 삭제한다.
 * @param pszDirName 폴더 이름
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CDirectory::Delete( const char * pszDirName )
{
#ifdef WIN32
	WIN32_FIND_DATA	sttFindData;
	HANDLE			hFind;
	BOOL				bNext = TRUE;
	std::string	strPath = pszDirName;
	std::string strFileName;

	strPath.append( "\\*.*" );

	hFind = FindFirstFile( strPath.c_str(), &sttFindData );
	if( hFind == INVALID_HANDLE_VALUE )
	{
		CLog::Print( LOG_ERROR, "FindFirstFile(%s) error(%d)", pszDirName, GetLastError() );
		return false;
	}

	for( ; bNext == TRUE; bNext = FindNextFile( hFind, &sttFindData ) )
	{
		if( !strcmp( sttFindData.cFileName, "." ) || !strcmp( sttFindData.cFileName, ".." ) ) continue;

		strFileName = pszDirName;
		CDirectory::AppendName( strFileName, sttFindData.cFileName );

		if( sttFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			CDirectory::Delete( strFileName.c_str() );
			RemoveDirectory( strFileName.c_str() );
		}
		else
		{
			DeleteFile( strFileName.c_str() );
		}
	}

	FindClose( hFind );

	RemoveDirectory( pszDirName );
#else
	DIR						* psttDir;
	struct dirent	* psttDirent, sttDirent;
	struct stat		sttStat;
	int		n;
	std::string		strFileName;

	psttDir = opendir( pszDirName );
	if( psttDir == NULL )
	{
		CLog::Print( LOG_ERROR, "opendir(%s) error(%d)", pszDirName, errno );
		return false;
	}

	for( n = readdir_r( psttDir, &sttDirent, &psttDirent ); psttDirent && n == 0; n = readdir_r( psttDir, &sttDirent, &psttDirent ) )
	{
		if( !strcmp( psttDirent->d_name, "." ) || !strcmp( psttDirent->d_name, ".." ) ) continue;

		strFileName = pszDirName;
		AppendName( strFileName, psttDirent->d_name );

		if( lstat( strFileName.c_str(), &sttStat ) < 0 ) continue;
		if( S_ISDIR( sttStat.st_mode ) )
		{
			CDirectory::Delete( strFileName.c_str() );
			rmdir( strFileName.c_str() );
		}
		else
		{
			unlink( strFileName.c_str() );
		}
	}

	closedir( psttDir );
	rmdir( pszDirName );
#endif

	return true;
}

