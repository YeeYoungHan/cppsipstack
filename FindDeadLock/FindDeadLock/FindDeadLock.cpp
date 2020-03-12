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

#include "FindDeadLock.h"
#include "Lock.h"
#include <stdio.h>
#include <string.h>

void GetVar( const char * pszPos, std::string & strVar )
{
	const char * pszStart;

	for( pszStart = pszPos; pszStart; --pszStart )
	{
		if( isspace( pszStart[0] ) )
		{
			strVar.append( pszStart + 1, pszPos - pszStart -1 );
			break;
		}
	}
}

void FindDeadLock( const char * pszFileName )
{
	FILE * fd = fopen( pszFileName, "r" );
	if( fd == NULL ) return;

	char szBuf[8192];
	const char * pszPos;
	int iLine = 0;
	bool bError = false;
	CLock clsLock( pszFileName );

	while( 1 )
	{
		if( fgets( szBuf, sizeof(szBuf), fd ) == NULL ) break;

		++iLine;

		if( ( pszPos = strstr( szBuf, ".acquire(" ) ) || ( pszPos = strstr( szBuf, "->acquire(" ) ) )
		{
			std::string strVar;

			GetVar( pszPos, strVar );

			if( clsLock.Add( strVar.c_str(), iLine ) == false )
			{
				bError = true;
				break;
			}
		}
		else if( ( pszPos = strstr( szBuf, ".release(" ) ) || ( pszPos = strstr( szBuf, "->release(" ) ) )
		{
			std::string strVar;

			GetVar( pszPos, strVar );

			if( clsLock.Del( strVar.c_str(), iLine ) == false )
			{
				bError = true;
				break;
			}
		}
	}

	if( bError == false )
	{
		clsLock.Print();
	}

	fclose( fd );
}
