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

#include "SipFrom.h"

static bool Test( const char * pszText, const char * pszResult )
{
	CSipFrom clsFrom;
	int		iTextLen = strlen( pszText );
	int		iPos, iLen;
	char	szResult[255];

	memset( szResult, 0, sizeof(szResult) );

	iPos = clsFrom.Parse( pszText, iTextLen );
	if( iPos == -1 )
	{
		printf( "sip from(%s) parse error\n", pszText );
		return false;
	}

	if( iPos != iTextLen ) 
	{
		printf( "sip from(%s) parse len(%d) != real len(%d)\n", pszText, iPos, iTextLen );
		return false;
	}

	iLen = clsFrom.ToString( szResult, sizeof(szResult) );
	if( strcmp( pszResult, szResult ) )
	{
		printf( "sip from(%s) != result(%s)\n", pszResult, szResult );
		return false;
	}

	return true;
}

bool TestSipFrom()
{
	if( Test( "<sip:1000@203.255.208.41:5062>;expires=300", "<sip:1000@203.255.208.41:5062>;expires=300" ) == false ) return false; 
	if( Test( "\"Bob\" <sips:bob@biloxi.com:5060> ;tag=a48s", "\"Bob\" <sips:bob@biloxi.com:5060>;tag=a48s" ) == false ) return false;
	if( Test( "\"Bob\" <sips:bob@biloxi.com> ;tag=a48s", "\"Bob\" <sips:bob@biloxi.com>;tag=a48s" ) == false ) return false;
	if( Test( "\"Bob \" <sips:bob@biloxi.com> ;tag=a48s", "\"Bob \" <sips:bob@biloxi.com>;tag=a48s" ) == false ) return false;
	if( Test( "\" Bob\" <sips:bob@biloxi.com> ;tag=a48s", "\" Bob\" <sips:bob@biloxi.com>;tag=a48s" ) == false ) return false;
	if( Test( "\"Bob\" <sips:bob@biloxi.com>;tag=a48s;ttl=22343", "\"Bob\" <sips:bob@biloxi.com>;tag=a48s;ttl=22343" ) == false ) return false;
	if( Test( "sip:+12125551212@phone2net.com;tag=887s", "<sip:+12125551212@phone2net.com>;tag=887s" ) == false ) return false;
	if( Test( "<sip:+12125551212@phone2net.com>;tag=887s", "<sip:+12125551212@phone2net.com>;tag=887s" ) == false ) return false;
	if( Test( "Anonymous <sip:c8oqz84zk7z@privacy.org>;tag=hyh8", "\"Anonymous\" <sip:c8oqz84zk7z@privacy.org>;tag=hyh8" ) == false ) return false;
	if( Test( "Anonymous   <sip:c8oqz84zk7z@privacy.org>;tag=hyh8", "\"Anonymous\" <sip:c8oqz84zk7z@privacy.org>;tag=hyh8" ) == false ) return false;
	if( Test( "sip:alice@atlanta.com", "<sip:alice@atlanta.com>" ) == false ) return false;
	if( Test( "sip:alice:secretword@atlanta.com;transport=tcp", "<sip:alice:secretword@atlanta.com>;transport=tcp" ) == false ) return false;
	if( Test( "<sip:alice:secretword@atlanta.com;transport=tcp>", "<sip:alice:secretword@atlanta.com;transport=tcp>" ) == false ) return false;
	if( Test( "sips:alice@atlanta.com?subject=project%20x&priority=urgent", "<sips:alice@atlanta.com?subject=project%20x&priority=urgent>" ) == false ) return false;
	if( Test( "<sips:alice@atlanta.com?subject=project%20x&priority=urgent>", "<sips:alice@atlanta.com?subject=project%20x&priority=urgent>" ) == false ) return false;
	if( Test( "sip:+1-212-555-1212:1234@gateway.com;user=phone", "<sip:+1-212-555-1212:1234@gateway.com>;user=phone" ) == false ) return false;
	if( Test( "<sip:+1-212-555-1212:1234@gateway.com;user=phone>", "<sip:+1-212-555-1212:1234@gateway.com;user=phone>" ) == false ) return false;
	if( Test( "<sip:+1-212-555-1212:1234@gateway.com;user=phone>", "<sip:+1-212-555-1212:1234@gateway.com;user=phone>" ) == false ) return false;
	if( Test( "sips:1212@gateway.com", "<sips:1212@gateway.com>" ) == false ) return false;
	if( Test( "sip:alice@192.0.2.4", "<sip:alice@192.0.2.4>" ) == false ) return false;

	// 아래의 테스트는 SIP From 규격에 적합하지 않는다. SIP URI 규격에만 적합하다.
	//if( Test( "sip:atlanta.com;method=REGISTER?to=alice%40atlanta.com", "<sip:atlanta.com>;method=REGISTER?to=alice%40atlanta.com" ) == false ) return false;
	if( Test( "<sip:atlanta.com;method=REGISTER?to=alice%40atlanta.com>", "<sip:atlanta.com;method=REGISTER?to=alice%40atlanta.com>" ) == false ) return false;
	if( Test( "<sip:alice;day=tuesday@atlanta.com>", "<sip:alice;day=tuesday@atlanta.com>" ) == false ) return false;
	if( Test( "<sip:alice@atlanta.com;maddr=239.255.255.1;ttl=15>", "<sip:alice@atlanta.com;maddr=239.255.255.1;ttl=15>" ) == false ) return false;

	return true;
}
