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

/**
 * @defgroup TestSipParser TestSipParser
 * SipParser �� SdpParser ���̺귯�� ����� �׽�Ʈ�Ѵ�.
 */

bool TestSipUri();
bool TestSipFrom();
bool TestSipVia();
bool TestSipMessage();
bool TestSdp();

/**
 * @ingroup TestSipParser
 * @brief 
 * @param argc 
 * @param argv 
 * @returns 
 */
int main( int argc, char * argv[] )
{
	if( TestSipUri() == false ) goto FUNC_END;
	if( TestSipFrom() == false ) goto FUNC_END;
	if( TestSipVia() == false ) goto FUNC_END;
	if( TestSipMessage() == false ) goto FUNC_END;
	if( TestSdp() == false ) goto FUNC_END;

	printf( "All test is O.K.\n" );

FUNC_END:
	return 0;
}
