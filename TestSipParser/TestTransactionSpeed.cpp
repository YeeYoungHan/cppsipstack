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

#include "SipUtility.h"
#include "TimeUtility.h"
#include <map>
#include <list>
#include <string>

typedef std::map< std::string, int > TR_MAP;
typedef std::list< std::string > TR_LIST;

void TestTransactionSpeed( int iLoopCount )
{
	TR_MAP clsMap;
	TR_LIST clsList;
	char	szText[255];

	char	szId[21], szValue[10];
	struct timeval sttTime;
	
	memset( szId, 0, sizeof(szId) );
	memset( szValue, 0, sizeof(szValue) );
	gettimeofday( &sttTime, NULL );

	srand( sttTime.tv_usec );

	uint32_t iIp = inet_addr("127.0.0.1"); 
	uint16_t iPort = 5060;

	memcpy( szValue, &iIp, 4 );
	memcpy( szValue+4, &iPort, 2 );
	memcpy( szValue+6, &sttTime.tv_sec, 2 );
	memcpy( szValue+8, &sttTime.tv_usec, 2 );

	SipMakePrintString( (unsigned char *)szValue, sizeof(szValue), szId, sizeof(szId) );

	SipSetSystemId( szId );

	for( int i = 0; i < 10000; ++i )
	{
		SipMakeBranch( szText, sizeof(szText) );

		clsMap.insert( TR_MAP::value_type( szText, 1 ) );
		clsList.push_back( szText );
	}

	printf( "map size(%d) list size(%d)\n", clsMap.size(), clsList.size() );

	uint64_t iStartTime, iEndTime;
	iLoopCount = iLoopCount / clsList.size() + 1;

	iStartTime = GetCurrentMiliSecond();

	TR_LIST::iterator	itList;
	TR_MAP::iterator	itMap;
	int iCount = 0;

	for( int i = 0; i < iLoopCount; ++i )
	{
		for( itList = clsList.begin(); itList != clsList.end(); ++itList )
		{
			itMap = clsMap.find( itList->c_str() );
			if( itMap == clsMap.end() )
			{
				printf( "find error\n" );
				return;
			}

			++itMap->second;
			++iCount;
		}
	}

	iEndTime = GetCurrentMiliSecond();

	int iTime = (int)(iEndTime - iStartTime);

	printf( "test time = %d ms, count = %d\n", iTime, iCount );
}
