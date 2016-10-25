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

#ifndef _FILE_LOG_H_
#define _FILE_LOG_H_

#include "Log.h"

/**
 * @ingroup SipPlatform
 * @brief 파일 로그 저장 클래스
 */
class CFileLog
{
public:
	CFileLog();
	~CFileLog();

public:
	bool Open( const char * pszDirName );
	bool Close( );

	bool Print( EnumLogLevel iLevel, const char * fmt, ... );

	int GetLevel( );
	void SetLevel( int iLevel );
	bool IsPrintLogLevel( EnumLogLevel iLevel );

	void SetMaxLogSize( int iSize );
	void SetMaxFolderSize( int64_t iSize );

private:
	void DeleteOldFile( );

	std::string	m_strDirName;		// 로그 파일을 저장할 디렉토리 이름
	char				m_szDate[9];		// 현재 저장중인 로그 파일 이름
	FILE				* m_sttFd;			// 로그 파일 지정자

	CSipMutex		m_clsMutex;			// Thread Mutex
	
	int				m_iLevel;					// 로그 파일에 저장할 레벨
	int				m_iMaxLogSize;		// 하나의 로그 파일에 저장할 수 있는 최대 크기
	int				m_iLogSize;				// 현재까지 저장된 로그 크기
	int				m_iIndex;					// 로그 파일 인덱스
	int64_t		m_iMaxFolderSize;	// 로그 폴더의 최대 크기
	int64_t		m_iFolderSize;		// 로그 폴더 크기
};

#endif
