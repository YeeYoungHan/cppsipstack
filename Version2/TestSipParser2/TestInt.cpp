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

#include "SipParserDefine.h"
#include <stdio.h>
#include <string.h>

bool TestInt()
{
	uint64_t i64 = 12345678901234567890ULL;
	char	szBuf[255];

#ifdef LINUX_64
	snprintf( szBuf, sizeof(szBuf), "%lu", i64 );
#else
	snprintf( szBuf, sizeof(szBuf), "%llu", i64 );
#endif

	if( strcmp( szBuf, "12345678901234567890" ) )
	{
		printf( "snprintf uint64 is error\n" );
		return false;
	}
	
	return true;
}
