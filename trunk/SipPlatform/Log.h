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

#ifndef _LOG_H_
#define _LOG_H_

#include "SipPlatformDefine.h"
#include <stdio.h>
#include "SipMutex.h"
#include "Directory.h"

#define LOG_MAX_SIZE								1024*8
#define FULLPATH_FILENAME_MAX_SIZE	1024
#define MIN_LOG_FILE_SIZE						1024*1024
#define MAX_LOG_FILE_SIZE						1024*1024*1024
#define DEFAULT_LOG_FILE_SIZE				1024*1024*10
#define DEFAULT_LOG_FOLDER_SIZE			3145728000UL

/** 
 * @ingroup SipPlatform
 * @brief �α� ���� ���� ENUM
 *	CLog ���� ����ϴ� ENUM �̴�.
 */
enum EnumLogLevel
{
	/** ���� �α� */
	LOG_ERROR = 0x0001,
	/** ���� �α� */
	LOG_INFO  = 0x0010,
	/** ����� �α� */
	LOG_DEBUG = 0x0100,
	/** ��Ʈ��ũ ����Ÿ ����� �α� */
	LOG_NETWORK = 0x0200,
	/** �ý��� �α� */
	LOG_SYSTEM = 0x400,
	/** SQL �α� */
	LOG_SQL = 0x800
};

/**
 * @ingroup SipPlatform
 * @brief �α� �޽����� ���α׷����� ó���ϰ� ���� ��쿡 ����Ѵ�.
 */
class ILogCallBack
{
public:
	virtual ~ILogCallBack(){};

	virtual void Print( EnumLogLevel eLevel, const char * fmt, ... ) = 0;
};

/** 
 * @ingroup SipPlatform
 * @brief �α� ���� Ŭ����
 */
class CLog
{
private:
	static char				* m_pszDirName;		// �α� ������ ������ ���丮 �̸�
	static char				m_szDate[9];		// ���� �������� �α� ���� �̸�
	static FILE				* m_sttFd;			// �α� ���� ������

	static CSipMutex	* m_pThreadMutex;	// Thread Mutex
	
	static int				m_iLevel;				// �α� ���Ͽ� ������ ����
	static int				m_iMaxLogSize;	// �ϳ��� �α� ���Ͽ� ������ �� �ִ� �ִ� ũ��
	static int				m_iLogSize;			// ������� ����� �α� ũ��
	static int				m_iIndex;				// �α� ���� �ε���
	static int64_t		m_iMaxFolderSize;	// �α� ������ �ִ� ũ��
	static int64_t		m_iFolderSize;		// �α� ���� ũ��

	static ILogCallBack * m_pclsCallBack;	// �α� callback

public:
	static bool SetDirectory( const char * pszDirName );
	static void Release();

	static void SetCallBack( ILogCallBack * pclsCallBack );

	static int Print( EnumLogLevel iLevel, const char * fmt, ... );
	static void Print( void (* func)( FILE * fd ) );

	static int GetLevel( );
	static void SetLevel( int iLevel );
	static void SetNullLevel();

	static void SetDebugLevel( );
	static bool IsPrintLogLevel( EnumLogLevel iLevel );
	static void SetMaxLogSize( int iSize );
	static void SetMaxFolderSize( int64_t iSize );
	static int GetLogIndex();
	static void DeleteOldFile( );

	static void SortFileList( FILE_LIST & clsFileList );
};

#endif
