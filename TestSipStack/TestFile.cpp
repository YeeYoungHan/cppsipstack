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

#include "SipMessage.h"
#include "SipBuffer.h"
#include <string.h>
#include <stdio.h>

bool TestFile( const char * pszFileName )
{
	FILE * fd = fopen( pszFileName, "rb" );
	if( fd == NULL )
	{
		printf( "%s fopen(%s) error\n", __FUNCTION__, pszFileName );
		return false;
	}

	char szBuf[8192];
	std::string strBuf;
	int n;

	while( 1 )
	{
		n = fread( szBuf, 1, sizeof(szBuf), fd );
		if( n <= 0 ) break;

		strBuf.append( szBuf, n );
	}

	fclose( fd );

	CSipBuffer clsSipBuf;
	char * pszBuf;

	for( int j = 0; j < 1000; ++j )
	{
		int iLen = strBuf.length();

		if( iLen >= SIP_MAX_BUF_SIZE )
		{
			iLen = SIP_MAX_BUF_SIZE-1;
		}

		clsSipBuf.AddBuf( strBuf.c_str(), iLen );

		while( clsSipBuf.GetSipMessage( &pszBuf, &iLen ) )
		{
			clsSipBuf.ShiftBuf( iLen );
		}
	}

	return true;
}
