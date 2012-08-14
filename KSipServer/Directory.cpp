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

/** 디렉토리를 생성한다. 
 *	
 *	이미 디렉토리가 생성되어 있으면 아무런 동작을 하지 않는다.
 *	"c:\temp\test" 와 같은 문자열을 입력하면 다음과 같이 동작하다.
 *	"c:\temp" 디렉토리가 존재하지 않으면 이를 생성한 후, "c:\temp\test" 디렉토리를 생성한다.
 *
 *	@param	szDirName	[in] 생성할 디렉토리의 full pathname
 *	@return	성공하면 0 을 리턴한다. 이미 디렉토리가 존재하여도 0 을 리턴한다.
 *			실패하면 -1 을 리턴한다. 
 *			디렉토리 full pathname 중간에 디렉토리가 아닌 항목이 포함된 경우, -1 을 리턴한다.
 */

int CDirectory::Create( const char * szDirName )
{
	int		i, iLen, iCount, n;
	char	* pszName;
	char	fisNotDirectory = 0;	// 디렉토리가 아닌가?
	char	fisError = 0;			// 디렉토리 생성시에 에러가 발생하였는가?

	iLen = (int)strlen( szDirName );
	pszName = new char[ iLen + 1 ];

	memset( pszName, 0, iLen + 1 );
	for( i = 0, iCount = 0; i < iLen; i++ )
	{
		if( szDirName[i] == DIR_SEP )
		{
			iCount++;

			// 디렉토리 이름이 "c:\test\temp\" 또는 "/test/temp/" 이므로 두번째 디렉토리
			// 구분자 부터 디렉토리를 생성하면 된다.
			if( iCount >= 2 )
			{
				n = CDirectory::IsDirectory( pszName );
				if( n == -1 )
				{
					// 디렉토리가 아닌 경우
					fisNotDirectory = 1;
					break;
				}
				else if( n == -2 )
				{
					// 디렉토리가 존재하지 않는 경우
#ifdef WIN32
					if( CreateDirectory( pszName, NULL ) == FALSE )
#else
					if( mkdir( pszName, DIR_MODE ) != 0 )
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

	// 디렉토리 이름이 "c:\test\temp" 또는 "/test/temp" 일 경우, 위의 loop 에서 temp 디렉토리를 
	// 생성하지 않으므로 이를 생성하기 위해서 아래의 코드가 필요하다.
	if( szDirName[iLen-1] != DIR_SEP )
	{
		n = CDirectory::IsDirectory( szDirName );
		if( n == -1 )
		{
			// 디렉토리가 아닌 경우
			return -1;
		}
		else if( n == -2 )
		{
			// 디렉토리가 존재하지 않는 경우
#ifdef WIN32
			if( CreateDirectory( szDirName, NULL ) == FALSE )
#else
			if( mkdir( szDirName, DIR_MODE ) != 0 )
#endif
			{
				return -1;
			}
		}
	}

	return 0;
}

/** 사용자가 입력한 path 가 디렉토리인지를 점검한다.
 *
 *	@param	szDirName	[in] 디렉토리 이름
 *	@return	입력된 path 가 디렉토리이면 0 을 리턴한다.
 *			존재하지 않으면 -2 을 리턴한다.
 *			디렉토리가 아니면 -1 을 리턴한다.
 */

int CDirectory::IsDirectory( const char * szDirName )
{
#ifdef WIN32
	// ACE 에서 S_ISDIR 을 지원하지 않기 때문에 윈도우의 경우를 위하여 WINAPI 를 사용하였음.
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
		// 파일이 존재하지 않는 경우.
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
		// 파일이 존재하지 않는 경우.
		return -2;	
	}
#endif

	return 0;
}

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
