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

#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include "SipPlatformDefine.h"
#include <string>
#include <list>

#ifdef WIN32
# include <windows.h>
# define DIR_SEP	'\\'
# define DIR_MODE		0
#else
# include <sys/stat.h>
# define DIR_SEP	'/'
# define DIR_MODE		S_IRWXU
#endif

typedef std::list< std::string > FILE_LIST;

/** 
 * @ingroup SipPlatform
 * @brief 디렉토리 관련 연산을 수행하는 클래스
 */
class CDirectory
{
public:
	static bool Create( const char * szDirName, int iDirMode = DIR_MODE );
	static bool IsDirectory( const char * szDirName );
	static int IsDirectoryCheck( const char * szDirName );
	static void AppendName( std::string & strFileName, const char * pszAppend );
	static bool List( const char * pszDirName, FILE_LIST & clsFileList );
	static bool FileList( const char * pszDirName, FILE_LIST & clsFileList );
	static char * GetProgramDirectory( );
	static int64_t GetSize( const char * pszDirName );
	static void DeleteAllFile( const char * pszDirName );
	static void GetDirName( const char * pszFilePath, std::string & strDirName );
	static void GetFileName( const char * pszFilePath, std::string & strFileName );
	static bool Delete( const char * pszDirName );
};

#endif
