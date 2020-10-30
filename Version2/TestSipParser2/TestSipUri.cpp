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

#include "SipUri.h"
#include "TimeUtility.h"
#include <string.h>
#include <stdio.h>

static bool Test( const char * pszText )
{
	CSipUri clsUri;
	int		iTextLen = (int)strlen( pszText );
	int		iPos;
	char	szResult[255];

	memset( szResult, 0, sizeof(szResult) );

	iPos = clsUri.Parse( pszText, iTextLen );
	if( iPos == -1 )
	{
		printf( "sip uri(%s) parse error\n", pszText );
		return false;
	}

	if( iPos != iTextLen ) 
	{
		printf( "sip uri(%s) parse len(%d) != real len(%d)\n", pszText, iPos, iTextLen );
		return false;
	}

	clsUri.ToString( szResult, sizeof(szResult) );
	if( strcmp( pszText, szResult ) )
	{
		printf( "sip uri(%s) != result(%s)\n", pszText, szResult );
		return false;
	}

	return true;
}

bool TestSipUri()
{
	if( Test( "sip:alice@atlanta.com:5060" ) == false ) return false;
	if( Test( "sip:alice@atlanta.com" ) == false ) return false;
	if( Test( "sip:alice:secretword@atlanta.com;transport=tcp" ) == false ) return false;
	if( Test( "sips:alice@atlanta.com?subject=project%20x&priority=urgent" ) == false ) return false;
	if( Test( "sip:+1-212-555-1212:1234@gateway.com;user=phone" ) == false ) return false;
	if( Test( "sip:+1-212-555-1212:1234@gateway.com:5080;user=phone" ) == false ) return false;
	if( Test( "sips:1212@gateway.com" ) == false ) return false;
	if( Test( "sip:alice@192.0.2.4" ) == false ) return false;
	if( Test( "sip:atlanta.com;method=REGISTER?to=alice%40atlanta.com" ) == false ) return false;
	if( Test( "sip:alice;day=tuesday@atlanta.com" ) == false ) return false;
	if( Test( "sip:alice@atlanta.com;maddr=239.255.255.1;ttl=15" ) == false ) return false;
	if( Test( "sip:alice@atlanta.com" ) == false ) return false;
	if( Test( "tel:1212@gateway.com" ) == false ) return false;

	// IPv6 Å×½ºÆ®
	if( Test( "sip:alice@[5f05:2000:80ad:5800:58:800:2023:1d71]:5060" ) == false ) return false;
	if( Test( "sip:alice@[5f05:2000:80ad:5800:58:800:2023:1d71]" ) == false ) return false;
	if( Test( "sip:alice:secretword@[5f05:2000:80ad:5800:58:800:2023:1d71];transport=tcp" ) == false ) return false;
	if( Test( "sips:alice@[5f05:2000:80ad:5800:58:800:2023:1d71]?subject=project%20x&priority=urgent" ) == false ) return false;
	if( Test( "sip:+1-212-555-1212:1234@[5f05:2000:80ad:5800:58:800:2023:1d71];user=phone" ) == false ) return false;
	if( Test( "sip:+1-212-555-1212:1234@[5f05:2000:80ad:5800:58:800:2023:1d71]:5080;user=phone" ) == false ) return false;
	if( Test( "sips:1212@[5f05:2000:80ad:5800:58:800:2023:1d71]" ) == false ) return false;
	if( Test( "sip:alice@[5f05:2000:80ad:5800:58:800:2023:1d71]" ) == false ) return false;
	if( Test( "sip:[5f05:2000:80ad:5800:58:800:2023:1d71];method=REGISTER?to=alice%40atlanta.com" ) == false ) return false;
	if( Test( "sip:alice;day=tuesday@[5f05:2000:80ad:5800:58:800:2023:1d71]" ) == false ) return false;
	if( Test( "sip:alice@[5f05:2000:80ad:5800:58:800:2023:1d71];maddr=239.255.255.1;ttl=15" ) == false ) return false;
	if( Test( "sip:alice@[5f05:2000:80ad:5800:58:800:2023:1d71]" ) == false ) return false;
	if( Test( "tel:1212@[5f05:2000:80ad:5800:58:800:2023:1d71]" ) == false ) return false;

	return true;
}

void TestSipUriSpeed( int iLoopCount )
{
	const char * pszText = "sip:1000@192.168.0.1:5060";
	int iTextLen = strlen(pszText);
	int iPos;
	uint64_t iStartTime, iEndTime;

	iStartTime = GetCurrentMiliSecond();

	for( int i = 0; i < iLoopCount; ++i )
	{
		CSipUri clsUri;

		iPos = clsUri.Parse( pszText, iTextLen );
		if( iPos == -1 )
		{
			printf( "CSipUri.Parse error\n" );
			break;
		}
	}

	iEndTime = GetCurrentMiliSecond();

	int iTime = (int)(iEndTime - iStartTime);

	printf( "test time = %d ms\n", iTime );
	if( iTime > 0 )
	{
		printf( "parse time = %.2f pps\n", (double)iLoopCount * 1000 / iTime );
	}
}
