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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static bool bInit = false;
static char garrChar[64];

void InitRandomString()
{
	if( bInit == false )
	{
		srand( (unsigned int)time(NULL) );

		int i, iPos = 0;

		for( i = 0; i < 26; ++i )
		{
			garrChar[iPos++] = 'a' + i;
			garrChar[iPos++] = 'A' + i;
		}

		for( i = 0; i < 10; ++i )
		{
			garrChar[iPos++] = '0' + i;
		}

		garrChar[iPos++] = '_';
		garrChar[iPos++] = '-';

		bInit = true;
	}
}

void GetRandomString( char * pszOutput, int iOutputSize )
{
	if( bInit == false )
	{
		InitRandomString();
	}

	for( int i = 0; i < iOutputSize; ++i )
	{
		unsigned short sRand = rand();

		pszOutput[i++] = garrChar[ (sRand >> 8) & 63 ];
		pszOutput[i] = garrChar[ (sRand & 0xFF) & 63 ];
	}
}

int main( int argc, char * argv[] )
{
	InitRandomString();

	printf( "%d\n", rand() );
	printf( "%d\n", rand() );
	printf( "%d\n", rand() );
	printf( "%d\n", rand() );

	return 0;
}
