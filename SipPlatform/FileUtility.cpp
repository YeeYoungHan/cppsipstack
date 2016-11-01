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
#include <sys/stat.h>

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
	int iLen = strlen( pszFileName );

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
