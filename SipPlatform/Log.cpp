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
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "Log.h"
#include "TimeUtility.h"

#ifdef WIN32
#include <time.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

#include "MemoryDebug.h"

// CLog 클래스의 정적 변수를 초기화 한다.
char * CLog::m_pszDirName = NULL;
char CLog::m_szDate[9] = { '\0' };
FILE * CLog::m_sttFd = NULL;
CSipMutex * CLog::m_pThreadMutex = NULL;
int CLog::m_iLevel = LOG_ERROR;
int CLog::m_iMaxLogSize = DEFAULT_LOG_FILE_SIZE;
int CLog::m_iLogSize = 0;
int64_t CLog::m_iMaxFolderSize = DEFAULT_LOG_FOLDER_SIZE;
int64_t CLog::m_iFolderSize = 0;
int CLog::m_iIndex = 1;
ILogCallBack * CLog::m_pclsCallBack = NULL;

/** 
 * @ingroup SipPlatform
 * @brief 로그 파일을 저장할 디렉토리를 설정한다. 만약 thread mutex 가 설정되어 있지 않으면 쓰레드 mutex 를 생성한다.
 * @param	pszDirName	[in] 로그 파일을 저장할 디렉토리
 * @return	성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CLog::SetDirectory( const char * pszDirName )
{
	if( pszDirName == NULL ) return false;

	if( m_pThreadMutex == NULL )
	{
		m_pThreadMutex = new CSipMutex();
		if( m_pThreadMutex == NULL ) return false;
	}

	if( m_pThreadMutex->acquire() == false ) return false;

	if( m_pszDirName == NULL )
	{
		int	iLen = (int)strlen(pszDirName);
		m_pszDirName = new char[ iLen + 1 ];
		if( m_pszDirName == NULL ) return false;

		sprintf( m_pszDirName, "%s", pszDirName );
#ifdef WIN32
		if( m_pszDirName[iLen-1] == '\\' )
#else
		if( m_pszDirName[iLen-1] == '/' )
#endif
		{
			m_pszDirName[iLen-1] = '\0';
		}
	}

	m_pThreadMutex->release();

	if( CDirectory::Create( m_pszDirName ) == false ) return false;

	m_iFolderSize = CDirectory::GetSize( m_pszDirName );

	return true;
}

/** 
 * @ingroup SipPlatform
 * @brief 클래스 변수를 초기화시킨다.
 */
void CLog::Release()
{
	if( CLog::m_pszDirName )
	{
		delete [] m_pszDirName;
		m_pszDirName = NULL;
	}

	if( m_pThreadMutex )
	{
		delete CLog::m_pThreadMutex;
		m_pThreadMutex = NULL;
	}
}

/**
 * @ingroup SipPlatform
 * @brief 로그 출력 callback 인터페이스를 등록한다.
 * @param pclsCallBack 로그 출력 callback 인터페이스
 */
void CLog::SetCallBack( ILogCallBack * pclsCallBack )
{
	m_pclsCallBack = pclsCallBack;
}

/** 
 * @ingroup SipPlatform
 * @brief 로그 파일에 로그를 저장한다.
 * @param	iLevel	[in] 로그 파일 레벨
 * @param	fmt		[in] 로그 파일에 저장할 포맷 문자열
 * @param	...		[in] fmt 포맷에 입력할 인자들
 * @return	입력에 성공하면 0 을 리턴한다.
 *			입력값이 잘못된 경우에는 -1을 리턴한다.
 *			mutex lock 에 실패한 경우에는 -1을 리턴한다.
 *			로그 파일 Open 에 실패하면 -1을 리턴한다.
 *			로그 레벨이 설정된 로그 레벨과 일치하지 않는 경우에는 1을 리턴한다.
 */
int CLog::Print( EnumLogLevel iLevel, const char * fmt, ... )
{
	if( ( m_iLevel & iLevel ) == 0 ) return 1;

	va_list		ap;
	char		szBuf[LOG_MAX_SIZE];
	char		szDate[9], szHeader[30];
	int			iResult = 0;
	struct tm	sttTm;

	switch( iLevel )
	{
	case LOG_ERROR:
		snprintf( szHeader, sizeof(szHeader), "[ERROR] " );
		break;
	case LOG_INFO:
		snprintf( szHeader, sizeof(szHeader), "[INFO] " );
		break;
	case LOG_DEBUG:
		snprintf( szHeader, sizeof(szHeader), "[DEBUG] " );
		break;
	case LOG_NETWORK:
		snprintf( szHeader, sizeof(szHeader), "[NETWORK] " );
		break;
	case LOG_SYSTEM:
		snprintf( szHeader, sizeof(szHeader), "[SYSTEM] " );
		break;
	case LOG_SQL:
		snprintf( szHeader, sizeof(szHeader), "[SQL] " );
		break;
	default:
		memset( szHeader, 0, sizeof(szHeader) );
		break;
	}
	
	// 현재의 시간을 구한다.
	struct timeval	sttTime;

	gettimeofday( &sttTime, NULL );
#ifdef WIN32
	_localtime32_s( &sttTm, &sttTime.tv_sec );
#else
	localtime_r( &sttTime.tv_sec, &sttTm );	
#endif

	snprintf( szDate, sizeof(szDate), "%04d%02d%02d", sttTm.tm_year + 1900, sttTm.tm_mon + 1, sttTm.tm_mday );

	if( m_pThreadMutex && m_pThreadMutex->acquire() == false ) return -1;

	if( m_pszDirName && strcmp( m_szDate, szDate ) )
	{
		// 현재의 날짜와 클래스에 저장된 날짜가 다른 경우에는 로그 파일을 새로
		// 제작하여서 Open 한다.
		char	szFileName[FULLPATH_FILENAME_MAX_SIZE];
		
		m_iIndex = 1;

OPEN_FILE:
		m_iLogSize = 0;

		snprintf( szFileName, sizeof(szFileName), "%s%c%04d%02d%02d_%d.txt", m_pszDirName, DIR_SEP, sttTm.tm_year + 1900, sttTm.tm_mon + 1, sttTm.tm_mday, m_iIndex );
		
		// 이미 Open 된 파일이 있는 경우에는 이를 닫는다.
		if( m_sttFd ) 
		{
			fclose( m_sttFd );
			m_sttFd = NULL;
		}

		DeleteOldFile();

		if( m_iMaxLogSize > 0 )
		{
			int64_t iFileSize = GetFileSize( szFileName );
			if( iFileSize >= m_iMaxLogSize )
			{
				++m_iIndex;
				goto OPEN_FILE;
			}

			m_iLogSize = (int)iFileSize;
		}
		
		m_sttFd = fopen( szFileName, "ab" );
		if( m_sttFd == NULL )
		{
			printf( "log file(%s) open error\n", szFileName );
			iResult = -1;
		}

		snprintf( m_szDate, sizeof(m_szDate), "%s", szDate );
	}
	else if( m_iMaxLogSize > 0 && m_iLogSize >= m_iMaxLogSize )
	{
		++m_iIndex;
		goto OPEN_FILE;
	}

	if( m_pThreadMutex ) m_pThreadMutex->release();

	va_start( ap, fmt );
	vsnprintf( szBuf, sizeof(szBuf)-1, fmt, ap );
	va_end( ap );

	if( m_pThreadMutex && m_pThreadMutex->acquire() == false ) return -1;

	if( m_sttFd )
	{
		int iWrite;

#ifdef WIN32
		iWrite = fprintf( m_sttFd, "[%02d:%02d:%02d.%06u] %s[%u] %s\r\n"
			, sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec, sttTime.tv_usec, szHeader, GetCurrentThreadId(), szBuf );
#else
		iWrite = fprintf( m_sttFd, "[%02d:%02d:%02d.%06u] %s[%lu] %s\n"
			, sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec, (unsigned int)sttTime.tv_usec, szHeader, (unsigned long)pthread_self(), szBuf );
#endif
		fflush( m_sttFd );

		m_iLogSize += iWrite;
		m_iFolderSize += iWrite;
	}
	else if( m_pclsCallBack )
	{
#ifdef WIN32
		m_pclsCallBack->Print( iLevel, "[%u] %s", GetCurrentThreadId(), szBuf );
#else
		m_pclsCallBack->Print( iLevel, "[%u] %s", (unsigned long)pthread_self(), szBuf );
#endif
	}
	else
	{
#ifdef WIN32
		printf( "[%02d:%02d:%02d] %s%s\r\n"
			, sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec, szHeader, szBuf );
#else
		printf( "[%02d:%02d:%02d] %s%s\n"
			, sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec, szHeader, szBuf );
#endif
	}

	if( m_pThreadMutex ) m_pThreadMutex->release();

	return iResult;
}

/**
 * @ingroup SipPlatform
 * @brief 응용에서 로그를 출력하는 경우, FILE 핸들을 응용 함수로 전달한다.
 * @param func 로그 출력 함수
 */
void CLog::Print( void (* func)( FILE * fd ) )
{
	if( m_sttFd == NULL ) return;

	if( m_pThreadMutex && m_pThreadMutex->acquire() == false ) return;

	func( m_sttFd );
	fflush( m_sttFd );

	if( m_pThreadMutex ) m_pThreadMutex->release();
}

/**	
 * @ingroup SipPlatform
 * @brief	로그 레벨을 가져온다.
 * @return 로그 레벨을 리턴한다.
 */
int CLog::GetLevel( )
{
	return m_iLevel;
}

/**	
 * @ingroup SipPlatform
 * @brief 로그 파일에 저장할 로그 레벨을 설정한다. 여러 로그를 저장할 경우, '|' 연산자를 이용하여서 여러 로그 레벨을 설정할 수 있다.
 * @param	iLevel	[in] 디버그 로그를 저장할 경우, LOG_DEBUG 를 설정한다.
 *				정보 로그를 저장할 경우, LOG_INFO 를 설정한다.
 *				에러 로그를 저장할 경우, LOG_ERROR 를 설정한다.
 */
void CLog::SetLevel( int iLevel )
{
	m_iLevel = LOG_ERROR | LOG_SYSTEM;
	m_iLevel |= iLevel;
}

/** 
 * @ingroup SipPlatform
 * @brief 로그 파일에 저장할 로그 레벨을 모두 삭제한다.
 *	- 로그 파일에 로그가 저장되지 않는다.
 */
void CLog::SetNullLevel()
{
	m_iLevel = 0;
}

/** 
 * @ingroup SipPlatform
 * @brief 디버그 로그 레벨을 설정한다.
 */
void CLog::SetDebugLevel( )
{
	m_iLevel |= LOG_DEBUG;
}

/** 
 * @ingroup SipPlatform
 * @brief 입력한 로그 레벨이 현재 출력할 수 있는 로그 레벨인지 분석하여 준다.
 * @param	iLevel	[in] 로그 레벨
 * @return	현재 출력할 수 있는 로그 레벨인 경우에는 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CLog::IsPrintLogLevel( EnumLogLevel iLevel )
{
	if( ( m_iLevel & iLevel ) == 0 ) return false;

	return true;
}

/** 
 * @ingroup SipPlatform
 * @brief 로그를 저장할 최대 파일 크기를 설정한다. 
 * @param iSize	로그를 저장할 최대 파일 크기
 */
void CLog::SetMaxLogSize( int iSize )
{
	if( iSize < MIN_LOG_FILE_SIZE )
	{
		iSize = MIN_LOG_FILE_SIZE;
	}
	else if( iSize > MAX_LOG_FILE_SIZE )
	{
		iSize = MAX_LOG_FILE_SIZE;
	}
	m_iMaxLogSize = iSize;
}

/**
 * @ingroup SipPlatform
 * @brief 로그 폴더 최대 크기를 설정한다.
 * @param iSize 로그 폴더 최대 크기
 */
void CLog::SetMaxFolderSize( int64_t iSize )
{
	// 최소 30일 로그 파일 저장 공간을 확보한다.
	if( m_iMaxLogSize == 0 )
	{
		if( iSize < ( MIN_LOG_FILE_SIZE * 30 ) )
		{
			iSize = MIN_LOG_FILE_SIZE * 30;
		}
	}
	else
	{
		if( iSize < m_iMaxLogSize * 30 )
		{
			iSize = m_iMaxLogSize * 30;
		}
	}

	m_iMaxFolderSize = iSize;
}

/** 
 * @ingroup SipPlatform
 * @brief 로그파일의 인덱스 번호를 리턴한다.
 * @return 로그파일의 인덱스 번호를 리턴한다.
 */
int CLog::GetLogIndex()
{
	return m_iIndex;
}

bool LogFileCompare( const std::string & strFirst, const std::string & strSecond )
{
	int iFirstLen = (int)strFirst.length();
	int iSecondLen = (int)strSecond.length();

	if( iFirstLen < 10 ) return true;
	if( iSecondLen < 10 ) return false;

	int n = strncmp( strFirst.c_str(), strSecond.c_str(), 8 );

	if( n < 0 ) return true;
	if( n > 0 ) return false;

	int iFirstIndex = atoi( strFirst.c_str() + 9 );
	int iSecondIndex = atoi( strSecond.c_str() + 9 );

	return ( iFirstIndex < iSecondIndex );
}

/**
 * @ingroup SipPlatform
 * @brief 로그 폴더의 크기가 설정된 크기보다 큰 경우, 오래된 로그 파일을 삭제한다.
 */
void CLog::DeleteOldFile( )
{
	if( m_iMaxFolderSize == 0 ) return;
	if( m_pszDirName == NULL ) return;

	if( m_iFolderSize >= m_iMaxFolderSize )
	{
		int64_t iWantSize = m_iMaxFolderSize * 8 / 10;
		FILE_LIST clsFileList;
		FILE_LIST::iterator	itList;
		int64_t iFileSize;

		CDirectory::FileList( m_pszDirName, clsFileList );

		clsFileList.sort( LogFileCompare );

		for( itList = clsFileList.begin(); itList != clsFileList.end(); ++itList )
		{
			std::string strFileName = m_pszDirName;

			CDirectory::AppendName( strFileName, itList->c_str() );

			iFileSize = GetFileSize( strFileName.c_str() );

	#ifdef WIN32
			DeleteFile( strFileName.c_str() );
	#else
			unlink( strFileName.c_str() );
	#endif

			m_iFolderSize -= iFileSize;
		
			if( m_iFolderSize < iWantSize ) break;
		}
	}
}

/**
 * @ingroup SipPlatform
 * @brief 로그 파일 리스트를 오래된 날짜에서 최근 날짜로 정렬한다.
 * @param clsFileList 로그 파일 리스트
 */
void CLog::SortFileList( FILE_LIST & clsFileList )
{
	clsFileList.sort( LogFileCompare );
}
