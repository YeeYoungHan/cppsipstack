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

int main( int argc, char * argv[] )
{
	// 동일한 초에 본 프로그램을 실행하면 동일한 값이 출력된다.
	srand( (unsigned int)time(NULL) );

	printf( "%d\n", rand() );
	printf( "%d\n", rand() );
	printf( "%d\n", rand() );
	printf( "%d\n", rand() );

	return 0;
}
