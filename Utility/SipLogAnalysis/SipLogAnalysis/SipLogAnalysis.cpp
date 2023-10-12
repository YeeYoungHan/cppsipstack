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

#include "SipLogAnalysis.h"

void PrintUsage( int argc, char * argv[] )
{
	printf( "[Usage] %s file {log file full path}\n", argv[0] );
	//printf( "        %s dir {log folder full path}\n", argv[0] );
}

int main( int argc, char * argv[] )
{
	if( argc != 3 )
	{
		PrintUsage( argc, argv );
		return 0;
	}

	const char * pszCommand = argv[1];

	if( !strcmp( pszCommand, "file" ) )
	{
		ReadLogFile( argv[2] );
	}
	else
	{
		printf( "undefined command[%s]\n", pszCommand );
	}

	return 0;
}
