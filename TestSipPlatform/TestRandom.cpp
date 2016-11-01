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

#include "Random.h"
#include <stdio.h>

#define MAX_COUNT	1000

bool TestRandom()
{
	int arrValue[MAX_COUNT];
	CRandom clsRandom;

	for( int i = 0; i < MAX_COUNT; ++i )
	{
		arrValue[i] = clsRandom.Get();
	}

	for( int i = 0; i < MAX_COUNT; ++i )
	{
		for( int j = 0; j < MAX_COUNT; ++j )
		{
			if( i == j ) continue;

			if( arrValue[j] == arrValue[i] )
			{
				printf( "%s error\n", __FUNCTION__ );
				return false;
			}
		}
	}

	return true;
}
