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

#include "Directory.h"
#include <stdlib.h>

void PrintFileList( const char * pszName, FILE_LIST & clsList )
{
	FILE_LIST::iterator	it;

	printf( "%s\n", pszName );

	for( it = clsList.begin(); it != clsList.end(); ++it )
	{
		printf( "%s\n", it->c_str() );
	}
}

static bool LogFileCompare( const std::string & strFirst, const std::string & strSecond )
{
	int n = strncmp( strFirst.c_str(), strSecond.c_str(), 8 );

	if( n < 0 ) return true;
	if( n > 0 ) return false;

	int iFirstIndex = atoi( strFirst.c_str() + 9 );
	int iSecondIndex = atoi( strSecond.c_str() + 9 );

	return ( iFirstIndex < iSecondIndex );
}

bool TestStringSort()
{
	FILE_LIST	clsFileList;

	clsFileList.push_back( "20140608_10.txt" );
	clsFileList.push_back( "20140612_9.txt" );
	clsFileList.push_back( "20140612_22.txt" );
	clsFileList.push_back( "20140612_21.txt" );
	clsFileList.push_back( "20140612_32.txt" );
	clsFileList.push_back( "20140612_31.txt" );
	clsFileList.push_back( "20140612_12.txt" );
	clsFileList.push_back( "20140612_11.txt" );
	clsFileList.push_back( "20140612_2.txt" );
	clsFileList.push_back( "20140612_3.txt" );
	clsFileList.push_back( "20140612_4.txt" );
	clsFileList.push_back( "20140612_1.txt" );
	clsFileList.push_back( "20140608_2.txt" );
	clsFileList.push_back( "20140608_3.txt" );
	clsFileList.push_back( "20140608_1.txt" );

	PrintFileList( "before sort", clsFileList );

	//clsFileList.sort();

	clsFileList.sort( LogFileCompare );

	PrintFileList( "after sort", clsFileList );

	return true;
}
