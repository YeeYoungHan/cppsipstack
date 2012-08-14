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

#include <string>

#ifdef WIN32
# include <windows.h>
# define DIR_SEP	'\\'
#else
# include <sys/stat.h>
# define DIR_SEP	'/'
# define DIR_MODE		S_IRWXU
#endif

/** @brief 디렉토리 관련 연산을 수행하는 클래스
 *
 */

class CDirectory
{
public:
	static int Create( const char * szDirName );
	static int IsDirectory( const char * szDirName );
	static void AppendName( std::string & strFileName, const char * pszAppend );
};

#endif

