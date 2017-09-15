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
#include <string.h>
#include <stdlib.h>

/**
 * @defgroup TestSipParser TestSipParser
 * SipParser / SdpParser / XmlParser 라이브러리 기능을 테스트한다.
 */

bool TestSipUri();
bool TestSipFrom();
bool TestSipVia();
bool TestSipMessage();
bool TestSdp();
bool TestInt();
bool TestMd5();
bool TestString();
bool TestSipHeader();
bool TestSipReason();
bool TestSipCallId();
void TestSpeed( int iLoopCount );
void TestTransactionSpeed( int iLoopCount );

/**
 * @ingroup TestSipParser
 * @brief SipParser / SdpParser / XmlParser 라이브러리 기능을 테스트한다.
 * @param argc 
 * @param argv 
 * @returns 0 을 리턴한다.
 */
int main( int argc, char * argv[] )
{
	if( argc == 3 )
	{
		if( !strcmp( argv[1], "speed" ) )
		{
			int iLoopCount = atoi( argv[2] );

			TestSpeed( iLoopCount );

			return 0;
		}
		else if( !strcmp( argv[1], "tspeed" ) )
		{
			int iLoopCount = atoi( argv[2] );

			TestTransactionSpeed( iLoopCount );

			return 0;
		}
	}

	if( TestSipCallId() == false ) goto FUNC_END;
	if( TestSipReason() == false ) goto FUNC_END;
	if( TestString() == false ) goto FUNC_END;
	if( TestMd5() == false ) goto FUNC_END;
	if( TestInt() == false ) goto FUNC_END;
	if( TestSipUri() == false ) goto FUNC_END;
	if( TestSipFrom() == false ) goto FUNC_END;
	if( TestSipVia() == false ) goto FUNC_END;
	if( TestSipMessage() == false ) goto FUNC_END;
	if( TestSdp() == false ) goto FUNC_END;

	printf( "All test is O.K.\n" );

FUNC_END:
	return 0;
}
