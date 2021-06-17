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

#include "FileLog.h"
#include "TimeUtility.h"
#include "FileUtility.h"
#include <stdarg.h>

#ifndef WIN32
#include <unistd.h>
#endif

extern bool LogFileCompare( const std::string & strFirst, const std::string & strSecond );

CFileLog::CFileLog() : m_sttFd(NULL), m_iLevel(LOG_ERROR), m_iMaxLogSize(DEFAULT_LOG_FILE_SIZE), m_iLogSize(0)
	, m_iIndex(1), m_iMaxFolderSize(DEFAULT_LOG_FOLDER_SIZE), m_iFolderSize(0)
{
	memset( m_szDate, 0, sizeof(m_szDate) );
}

CFileLog::~CFileLog()
{
	Close();
}

/** 
 * @ingroup SipPlatform
 * @brief �α� ������ ������ ���丮�� �����Ѵ�.
 * @param	pszDirName	�α� ������ ������ ���丮
 * @return �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CFileLog::Open( const char * pszDirName )
{
	if( pszDirName == NULL ) return false;

	int iLen = (int)strlen( pszDirName );
	bool bRes = false;

	if( iLen < 3 ) return false;

	m_clsMutex.acquire();

	if( m_strDirName.empty() )
	{
#ifdef WIN32
		if( pszDirName[iLen-1] == '\\' )
#else
		if( pszDirName[iLen-1] == '/' )
#endif
		{
			m_strDirName.append( pszDirName, iLen - 1 );
		}
		else
		{
			m_strDirName = pszDirName;
		}

		if( CDirectory::Create( m_strDirName.c_str() ) )
		{
			m_iFolderSize = CDirectory::GetSize( m_strDirName.c_str() );
			bRes = true;
		}
	}

	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipPlatform
 * @brief �α� ���� ������ �����Ѵ�.
 * @returns true �� �����Ѵ�.
 */
bool CFileLog::Close( )
{
	m_clsMutex.acquire();
	if( m_strDirName.empty() == false )
	{
		if( m_sttFd != NULL )
		{
			fclose( m_sttFd );
			m_sttFd = NULL;
		}

		m_strDirName.clear();
	}
	m_clsMutex.release();

	return true;
}

/** 
 * @ingroup SipPlatform
 * @brief �α� ���Ͽ� �α׸� �����Ѵ�.
 * @param	iLevel	�α� ���� ����
 * @param	fmt			�α� ���Ͽ� ������ ���� ���ڿ�
 * @param	...			fmt ���˿� �Է��� ���ڵ�
 * @return �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CFileLog::Print( EnumLogLevel iLevel, const char * fmt, ... )
{
	if( ( m_iLevel & iLevel ) == 0 ) return false;

	va_list		ap;
	char		szBuf[LOG_MAX_SIZE];
	char		szDate[9], szHeader[30];
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

	if( m_clsMutex.acquire() == false ) return false;

	if( m_strDirName.empty() == false && strcmp( m_szDate, szDate ) )
	{
		// ������ ��¥�� Ŭ������ ����� ��¥�� �ٸ� ��쿡�� �α� ������ ���� �����Ͽ��� Open �Ѵ�.
		char	szFileName[FULLPATH_FILENAME_MAX_SIZE];
		
		m_iIndex = 1;

OPEN_FILE:
		m_iLogSize = 0;

		snprintf( szFileName, sizeof(szFileName), "%s%c%04d%02d%02d_%d.txt", m_strDirName.c_str(), DIR_SEP, sttTm.tm_year + 1900, sttTm.tm_mon + 1, sttTm.tm_mday, m_iIndex );
		
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
		
		m_sttFd = fopen( szFileName, "ab" );
		if( m_sttFd == NULL )
		{
			printf( "log file(%s) open error\n", szFileName );
		}

		snprintf( m_szDate, sizeof(m_szDate), "%s", szDate );
	}
	else if( m_iMaxLogSize > 0 && m_iLogSize >= m_iMaxLogSize )
	{
		++m_iIndex;
		goto OPEN_FILE;
	}

	m_clsMutex.release();

	va_start( ap, fmt );
	vsnprintf( szBuf, sizeof(szBuf)-1, fmt, ap );
	va_end( ap );

	if( m_clsMutex.acquire() == false ) return false;

	if( m_sttFd )
	{
		int iWrite;

#ifdef WIN32
		iWrite = fprintf( m_sttFd, "[%02d:%02d:%02d.%06u] %s[%X] %s\r\n"
			, sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec, sttTime.tv_usec, szHeader, GetCurrentThreadId(), szBuf );
#else
		iWrite = fprintf( m_sttFd, "[%02d:%02d:%02d.%06u] %s[%lX] %s\n"
			, sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec, (unsigned int)sttTime.tv_usec, szHeader, (unsigned long)pthread_self(), szBuf );
#endif
		fflush( m_sttFd );

		m_iLogSize += iWrite;
		m_iFolderSize += iWrite;
	}
	else
	{
#ifdef WIN32
		printf( "[%02d:%02d:%02d] %s%s\r\n", sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec, szHeader, szBuf );
#else
		printf( "[%02d:%02d:%02d] %s%s\n", sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec, szHeader, szBuf );
#endif
	}

	m_clsMutex.release();

	return true;
}

/**	
 * @ingroup SipPlatform
 * @brief	�α� ������ �����´�.
 * @return �α� ������ �����Ѵ�.
 */
int CFileLog::GetLevel( )
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
void CFileLog::SetLevel( int iLevel )
{
	m_iLevel = LOG_ERROR | LOG_SYSTEM;
	m_iLevel |= iLevel;
}

/** 
 * @ingroup SipPlatform
 * @brief �Է��� �α� ������ ���� ����� �� �ִ� �α� �������� �м��Ͽ� �ش�.
 * @param	iLevel	[in] �α� ����
 * @return	���� ����� �� �ִ� �α� ������ ��쿡�� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CFileLog::IsPrintLogLevel( EnumLogLevel iLevel )
{
	if( ( m_iLevel & iLevel ) == 0 ) return false;

	return true;
}

/** 
 * @ingroup SipPlatform
 * @brief �α׸� ������ �ִ� ���� ũ�⸦ �����Ѵ�. 
 * @param iSize	�α׸� ������ �ִ� ���� ũ��
 */
void CFileLog::SetMaxLogSize( int iSize )
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
void CFileLog::SetMaxFolderSize( int64_t iSize )
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
 * @brief �α� ������ ũ�Ⱑ ������ ũ�⺸�� ū ���, ������ �α� ������ �����Ѵ�.
 */
void CFileLog::DeleteOldFile( )
{
	if( m_iMaxFolderSize == 0 ) return;
	if( m_strDirName.empty() ) return;

	if( m_iFolderSize >= m_iMaxFolderSize )
	{
		int64_t iWantSize = m_iMaxFolderSize * 8 / 10;
		FILE_LIST clsFileList;
		FILE_LIST::iterator	itList;
		int64_t iFileSize;

		CDirectory::FileList( m_strDirName.c_str(), clsFileList );

		clsFileList.sort( LogFileCompare );

		for( itList = clsFileList.begin(); itList != clsFileList.end(); ++itList )
		{
			std::string strFileName = m_strDirName;

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
