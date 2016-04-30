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
 * @brief ���丮�� �����Ѵ�. 
 *	
 *	�̹� ���丮�� �����Ǿ� ������ �ƹ��� ������ ���� �ʴ´�.
 *	"c:\temp\test" �� ���� ���ڿ��� �Է��ϸ� ������ ���� �����ϴ�.
 *	"c:\temp" ���丮�� �������� ������ �̸� ������ ��, "c:\temp\test" ���丮�� �����Ѵ�.
 *
 * @param	szDirName	[in] ������ ���丮�� full pathname
 * @param iDirMode	[in] ������ ���丮�� ����
 * @return	�����ϸ� 0 �� �����Ѵ�. �̹� ���丮�� �����Ͽ��� 0 �� �����Ѵ�.
 *			�����ϸ� -1 �� �����Ѵ�. 
 *			���丮 full pathname �߰��� ���丮�� �ƴ� �׸��� ���Ե� ���, -1 �� �����Ѵ�.
 */
int CDirectory::Create( const char * szDirName, int iDirMode )
{
	int		i, iLen, iCount, n;
	char	* pszName;
	char	fisNotDirectory = 0;	// ���丮�� �ƴѰ�?
	char	fisError = 0;			// ���丮 �����ÿ� ������ �߻��Ͽ��°�?

	iLen = (int)strlen( szDirName );
	pszName = new char[ iLen + 1 ];

	memset( pszName, 0, iLen + 1 );
	for( i = 0, iCount = 0; i < iLen; i++ )
	{
		if( szDirName[i] == DIR_SEP )
		{
			iCount++;

			// ���丮 �̸��� "c:\test\temp\" �Ǵ� "/test/temp/" �̹Ƿ� �ι�° ���丮
			// ������ ���� ���丮�� �����ϸ� �ȴ�.
			if( iCount >= 2 )
			{
				n = CDirectory::IsDirectory( pszName );
				if( n == -1 )
				{
					// ���丮�� �ƴ� ���
					fisNotDirectory = 1;
					break;
				}
				else if( n == -2 )
				{
					// ���丮�� �������� �ʴ� ���
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

	if( fisNotDirectory == 1 ) return -1;
	if( fisError == 1 ) return -1;

	// ���丮 �̸��� "c:\test\temp" �Ǵ� "/test/temp" �� ���, ���� loop ���� temp ���丮�� 
	// �������� �����Ƿ� �̸� �����ϱ� ���ؼ� �Ʒ��� �ڵ尡 �ʿ��ϴ�.
	if( szDirName[iLen-1] != DIR_SEP )
	{
		n = CDirectory::IsDirectory( szDirName );
		if( n == -1 )
		{
			// ���丮�� �ƴ� ���
			return -1;
		}
		else if( n == -2 )
		{
			// ���丮�� �������� �ʴ� ���
#ifdef WIN32
			if( CreateDirectory( szDirName, NULL ) == FALSE )
#else
			if( mkdir( szDirName, iDirMode ) != 0 )
#endif
			{
				return -1;
			}
		}
	}

	return 0;
}

/** 
 * @ingroup SipPlatform
 * @brief ����ڰ� �Է��� path �� ���丮������ �����Ѵ�.
 * @param	szDirName	[in] ���丮 �̸�
 * @return	�Էµ� path �� ���丮�̸� 0 �� �����Ѵ�.
 *			�������� ������ -2 �� �����Ѵ�.
 *			���丮�� �ƴϸ� -1 �� �����Ѵ�.
 */

int CDirectory::IsDirectory( const char * szDirName )
{
#ifdef WIN32
	// ACE ���� S_ISDIR �� �������� �ʱ� ������ �������� ��츦 ���Ͽ� WINAPI �� ����Ͽ���.
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
		// ������ �������� �ʴ� ���.
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
		// ������ �������� �ʴ� ���.
		return -2;	
	}
#endif

	return 0;
}

/**
 * @ingroup SipPlatform
 * @brief ���� ��ο� ���� �̸��� �߰��Ѵ�.
 * @param strFileName ���� ���
 * @param pszAppend		�߰��� ���� �̸�
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
 * @brief ������ �����ϴ� ��� ����/���� ����Ʈ�� �����´�.
 * @param pszDirName	���� ���
 * @param clsFileList ����/���� ����Ʈ�� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief ������ �����ϴ� ��� ���� ����Ʈ�� �����´�.
 * @param pszDirName	���� ���
 * @param clsFileList ���� ����Ʈ�� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief ���α׷��� ������ ������ �����´�.
 * @returns ���α׷��� ������ ���� ��θ� �����Ѵ�.
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
 * @brief ���� ũ�⸦ �����´�.
 * @param pszDirName ���� fulll path
 * @returns ���� ũ�⸦ �����Ѵ�.
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
 * @brief ������ ���Ե� ���ϵ��� ��� �����Ѵ�.
 * @param pszDirName ���� full path
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
 * @brief ���� ��ο��� ���� �̸��� �����´�.
 * @param pszFilePath ���� ���
 * @param strDirName	���� �̸��� ������ ����
 */
void CDirectory::GetDirName( const char * pszFilePath, std::string & strDirName )
{
	int iLen = strlen( pszFilePath );

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
 * @brief ���� ��ο��� ���� �̸��� �����´�.
 * @param pszFilePath ���� ���
 * @param strFileName ���� �̸��� ������ ����
 */
void CDirectory::GetFileName( const char * pszFilePath, std::string & strFileName )
{
	int iLen = strlen( pszFilePath );

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
 * @brief ������ ��� �����Ѵ�.
 * @param pszDirName ���� �̸�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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

