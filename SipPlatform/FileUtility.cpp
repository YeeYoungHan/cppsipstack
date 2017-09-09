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
#include "SipTcp.h"
#include "Log.h"
#include "TimeString.h"
#include <sys/stat.h>

#ifndef WIN32
#include <utime.h>
#endif

#include "MemoryDebug.h"

/**
 * @ingroup SipPlatform
 * @brief 파일이 존재하는지 검사한다.
 * @param pszFileName 파일 이름
 * @returns 파일이 존재하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsExistFile( const char * pszFileName )
{
	struct stat sttStat;

	if( stat( pszFileName, &sttStat ) == -1 )
	{
		return false;
	}

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief 파일 크기를 리턴한다.
 * @param pszFileName 파일 이름
 * @returns 파일이 존재하면 파일 크기를 리턴하고 그렇지 않으면 0 을 리턴한다.
 */
int64_t GetFileSize( const char * pszFileName )
{
	struct stat sttStat;

	if( stat( pszFileName, &sttStat ) == -1 )
	{
		return 0;
	}

	return sttStat.st_size;
}

/**
 * @ingroup SipPlatform
 * @brief 파일 이름에서 파일 확장자를 가져온다.
 * @param pszFileName 파일 이름
 * @param strExt			파일 확장자 저장 변수
 * @returns 파일 확장자가 존재하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool GetFileExt( const char * pszFileName, std::string & strExt )
{
	int iLen = (int)strlen( pszFileName );

	strExt.clear();

	for( int i = iLen - 1; i >= 0; --i )
	{
		if( pszFileName[i] == '.' )
		{
			strExt = pszFileName + i + 1;
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipPlatform
 * @brief 파일 경로에서 폴더 경로 가져오기
 * @param pszFilePath	파일 경로
 * @param strFolder		폴더 경로 저장 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool GetFolderPathOfFilePath( const char * pszFilePath, std::string & strFolder )
{
	strFolder.clear();

	if( pszFilePath == NULL ) return false;

	int iLen = (int)strlen( pszFilePath );
	if( iLen < 2 ) return false;

#ifdef WIN32
	if( pszFilePath[iLen-1] == '\\' ) return false;
#else
	if( pszFilePath[iLen-1] == '/' ) return false;
#endif

	for( int i = iLen - 2; i >= 0; --i )
	{
#ifdef WIN32
		if( pszFilePath[i] == '\\' )
#else
		if( pszFilePath[i] == '/' )
#endif
		{

#ifndef WIN32
			if( i == 0 )
			{
				strFolder = "/";
				return true;
			}
#endif

			strFolder.append( pszFilePath, i );
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipPlatform
 * @brief 파일 경로에서 파일 이름 가져오기
 * @param pszFilePath 파일 경로
 * @param strFileName 파일 이름 저장 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool GetFileNameOfFilePath( const char * pszFilePath, std::string & strFileName )
{
	strFileName.clear();

	if( pszFilePath == NULL ) return false;

	int iLen = (int)strlen( pszFilePath );
	if( iLen < 2 ) return false;

#ifdef WIN32
	if( pszFilePath[iLen-1] == '\\' ) return false;
#else
	if( pszFilePath[iLen-1] == '/' ) return false;
#endif

	for( int i = iLen - 2; i >= 0; --i )
	{
#ifdef WIN32
		if( pszFilePath[i] == '\\' )
#else
		if( pszFilePath[i] == '/' )
#endif
		{
			strFileName.append( pszFilePath + i + 1 );
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipPlatform
 * @brief 파일의 수정 시간을 가져온다.
 * @param pszFileName 파일 이름
 * @param strDate			[out] 파일 수정 시간 저장 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool GetFileDate( const char * pszFileName, std::string & strDate )
{
	strDate.clear();

	if( pszFileName == NULL ) return false;

	struct stat sttStat;
	char szDate[21];

	if( stat( pszFileName, &sttStat ) == -1 )
	{
		return false;
	}

	GetDateTimeString( sttStat.st_mtime, szDate, sizeof(szDate) );
	strDate = szDate;

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief 파일의 수정 시간을 설정한다.
 * @param pszFileName 파일 이름
 * @param pszDate			파일 수정 시간
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool SetFileDate( const char * pszFileName, const char * pszDate )
{
	if( pszFileName == NULL || pszDate == NULL ) return false;
	if( strlen( pszDate ) != 14 ) return false;

	time_t iTime = ParseDateTimeString( pszDate );

#ifdef WIN32
	LONGLONG ll;
	FILETIME sttTime;

	ll = Int32x32To64( iTime, 10000000 ) + 116444736000000000;
	sttTime.dwLowDateTime = (DWORD)ll;
	sttTime.dwHighDateTime = ll >> 32;
	
	HANDLE hFile = CreateFile( pszFileName, FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE ) return false;

	SetFileTime( hFile, NULL, NULL, &sttTime );
	CloseHandle( hFile );
#else
	struct utimbuf sttTime;

	sttTime.actime = iTime;
	sttTime.modtime = iTime;
	
	if( utime( pszFileName, &sttTime ) == -1 ) return false;
#endif

	return true;
}

/**
 * @ingroup SipPlatform
 * @brief 파일을 삭제한다.
 * @param pszFileName 파일 이름
 */
void DelFile( const char * pszFileName )
{
#ifdef WIN32
	DeleteFile( pszFileName );
#else
	unlink( pszFileName );
#endif
}

/**
 * @ingroup SipPlatform
 * @brief 파일을 복사한다.
 * @param pszSrcFileName	원본 파일 path
 * @param pszDestFileName 복사본 파일 path
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CpFile( const char * pszSrcFileName, const char * pszDestFileName )
{
	FILE * fdSrc = fopen( pszSrcFileName, "rb" );
	if( fdSrc == NULL )
	{
		CLog::Print( LOG_ERROR, "%s src file(%s) open error(%d)", __FUNCTION__, pszSrcFileName, GetError() );
		return false;
	}

	FILE * fdDest = fopen( pszDestFileName, "wb" );
	if( fdDest == NULL )
	{
		fclose( fdSrc );
		CLog::Print( LOG_ERROR, "%s dest file(%s) open error(%d)", __FUNCTION__, pszDestFileName, GetError() );
		return false;
	}

	size_t iRead;
	char szBuf[8192];
	bool bError = false;

	while( 1 )
	{
		iRead = fread( szBuf, 1, sizeof(szBuf), fdSrc );
		if( iRead <= 0 ) break;

		if( fwrite( szBuf, 1, iRead, fdDest ) != iRead )
		{
			CLog::Print( LOG_ERROR, "%s dest file(%s) write error(%d)", __FUNCTION__, pszDestFileName, GetError() );
			bError = true;
			break;
		}
	}

	fclose( fdDest );
	fclose( fdSrc );

	if( bError )
	{
		DelFile( pszDestFileName );
		return false;
	}

	return true;
}
