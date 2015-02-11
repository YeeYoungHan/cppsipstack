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

// CLog Ŭ������ ���� ������ �ʱ�ȭ �Ѵ�.
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
 * @brief �α� ������ ������ ���丮�� �����Ѵ�. ���� thread mutex �� �����Ǿ� ���� ������ ������ mutex �� �����Ѵ�.
 * @param	pszDirName	[in] �α� ������ ������ ���丮
 * @return	�����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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

	if( CDirectory::Create( m_pszDirName ) != 0 ) return false;

	m_iFolderSize = CDirectory::GetSize( m_pszDirName );

	return true;
}

/** 
 * @ingroup SipPlatform
 * @brief Ŭ���� ������ �ʱ�ȭ��Ų��.
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
 * @brief �α� ��� callback �������̽��� ����Ѵ�.
 * @param pclsCallBack �α� ��� callback �������̽�
 */
void CLog::SetCallBack( ILogCallBack * pclsCallBack )
{
	m_pclsCallBack = pclsCallBack;
}

/** 
 * @ingroup SipPlatform
 * @brief �α� ���Ͽ� �α׸� �����Ѵ�.
 * @param	iLevel	[in] �α� ���� ����
 * @param	fmt		[in] �α� ���Ͽ� ������ ���� ���ڿ�
 * @param	...		[in] fmt ���˿� �Է��� ���ڵ�
 * @return	�Է¿� �����ϸ� 0 �� �����Ѵ�.
 *			�Է°��� �߸��� ��쿡�� -1�� �����Ѵ�.
 *			mutex lock �� ������ ��쿡�� -1�� �����Ѵ�.
 *			�α� ���� Open �� �����ϸ� -1�� �����Ѵ�.
 *			�α� ������ ������ �α� ������ ��ġ���� �ʴ� ��쿡�� 1�� �����Ѵ�.
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
	
	// ������ �ð��� ���Ѵ�.
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
		// ������ ��¥�� Ŭ������ ����� ��¥�� �ٸ� ��쿡�� �α� ������ ����
		// �����Ͽ��� Open �Ѵ�.
		char	szFileName[FULLPATH_FILENAME_MAX_SIZE];
		
		m_iIndex = 1;

OPEN_FILE:
		m_iLogSize = 0;

		snprintf( szFileName, sizeof(szFileName), "%s%c%04d%02d%02d_%d.txt", m_pszDirName, DIR_SEP, sttTm.tm_year + 1900, sttTm.tm_mon + 1, sttTm.tm_mday, m_iIndex );
		
		// �̹� Open �� ������ �ִ� ��쿡�� �̸� �ݴ´�.
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
		
		m_sttFd = fopen( szFileName, "a" );
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
		iWrite = fprintf( m_sttFd, "[%02d:%02d:%02d.%06u] %s[%u] %s\n"
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
 * @brief ���뿡�� �α׸� ����ϴ� ���, FILE �ڵ��� ���� �Լ��� �����Ѵ�.
 * @param func �α� ��� �Լ�
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
 * @brief	�α� ������ �����´�.
 * @return �α� ������ �����Ѵ�.
 */
int CLog::GetLevel( )
{
	return m_iLevel;
}

/**	
 * @ingroup SipPlatform
 * @brief �α� ���Ͽ� ������ �α� ������ �����Ѵ�. ���� �α׸� ������ ���, '|' �����ڸ� �̿��Ͽ��� ���� �α� ������ ������ �� �ִ�.
 * @param	iLevel	[in] ����� �α׸� ������ ���, LOG_DEBUG �� �����Ѵ�.
 *				���� �α׸� ������ ���, LOG_INFO �� �����Ѵ�.
 *				���� �α׸� ������ ���, LOG_ERROR �� �����Ѵ�.
 */
void CLog::SetLevel( int iLevel )
{
	m_iLevel = LOG_ERROR | LOG_SYSTEM;
	m_iLevel |= iLevel;
}

/** 
 * @ingroup SipPlatform
 * @brief �α� ���Ͽ� ������ �α� ������ ��� �����Ѵ�.
 *	- �α� ���Ͽ� �αװ� ������� �ʴ´�.
 */
void CLog::SetNullLevel()
{
	m_iLevel = 0;
}

/** 
 * @ingroup SipPlatform
 * @brief ����� �α� ������ �����Ѵ�.
 */
void CLog::SetDebugLevel( )
{
	m_iLevel |= LOG_DEBUG;
}

/** 
 * @ingroup SipPlatform
 * @brief �Է��� �α� ������ ���� ����� �� �ִ� �α� �������� �м��Ͽ� �ش�.
 * @param	iLevel	[in] �α� ����
 * @return	���� ����� �� �ִ� �α� ������ ��쿡�� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CLog::IsPrintLogLevel( EnumLogLevel iLevel )
{
	if( ( m_iLevel & iLevel ) == 0 ) return false;

	return true;
}

/** 
 * @ingroup SipPlatform
 * @brief �α׸� ������ �ִ� ���� ũ�⸦ �����Ѵ�. 
 * @param iSize	�α׸� ������ �ִ� ���� ũ��
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
 * @brief �α� ���� �ִ� ũ�⸦ �����Ѵ�.
 * @param iSize �α� ���� �ִ� ũ��
 */
void CLog::SetMaxFolderSize( int64_t iSize )
{
	// �ּ� 30�� �α� ���� ���� ������ Ȯ���Ѵ�.
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
 * @brief �α������� �ε��� ��ȣ�� �����Ѵ�.
 * @return �α������� �ε��� ��ȣ�� �����Ѵ�.
 */
int CLog::GetLogIndex()
{
	return m_iIndex;
}

static bool LogFileCompare( const std::string & strFirst, const std::string & strSecond )
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
 * @brief �α� ������ ũ�Ⱑ ������ ũ�⺸�� ū ���, ������ �α� ������ �����Ѵ�.
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
 * @brief �α� ���� ����Ʈ�� ������ ��¥���� �ֱ� ��¥�� �����Ѵ�.
 * @param clsFileList �α� ���� ����Ʈ
 */
void CLog::SortFileList( FILE_LIST & clsFileList )
{
	clsFileList.sort( LogFileCompare );
}
