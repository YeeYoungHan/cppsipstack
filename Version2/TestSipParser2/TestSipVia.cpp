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

#include "SipVia.h"
#include <string.h>
#include <stdio.h>

static bool Test( const char * pszText, const char * pszResult, int iTextLen )
{
	CSipVia clsVia;
	int		iPos;
	char	szResult[255];

	memset( szResult, 0, sizeof(szResult) );

	iPos = clsVia.Parse( pszText, iTextLen );
	if( iPos == -1 )
	{
		printf( "sip via(%s) parse error\n", pszText );
		return false;
	}

	if( iPos != iTextLen ) 
	{
		printf( "sip via(%s) parse len(%d) != real len(%d)\n", pszText, iPos, iTextLen );
		return false;
	}

	clsVia.ToString( szResult, sizeof(szResult) );
	if( strcmp( pszResult, szResult ) )
	{
		printf( "sip via(%s) != result(%s)\n", pszResult, szResult );
		return false;
	}

	return true;
}

bool TestSipVia()
{
	if( Test( 
		"SIP/2.0/UDP 192.168.1.100",
	  "SIP/2.0/UDP 192.168.1.100",
		25 ) == false ) return false;
	if( Test( 
		"SIP/2.0/UDP 192.168.1.100:5060",
	  "SIP/2.0/UDP 192.168.1.100:5060",
		30 ) == false ) return false;
	if( Test( 
		"SIP/2.0/UDP 192.168.1.100:5060;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
	  "SIP/2.0/UDP 192.168.1.100:5060;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
		97 ) == false ) return false;
	if( Test(  
		"SIP/2.0/UDP 192.168.1.100:5060 ;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
	  "SIP/2.0/UDP 192.168.1.100:5060;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
		98 ) == false ) return false;
	if( Test(  
		"SIP/2.0/UDP 192.168.1.100:5060 ; ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
	  "SIP/2.0/UDP 192.168.1.100:5060;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
		99 ) == false ) return false;
	if( Test(  
		"SIP/2.0/UDP 192.168.1.100;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
	  "SIP/2.0/UDP 192.168.1.100;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
		92 ) == false ) return false;
	if( Test( 		
		"SIP/2.0/UDP 192.168.1.100:5060;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890,SIP/2.0/UDP 1.1.1.1:5060;ttl=60;maddr=2.2.2.2;received=3.3.3.3;branch=0987654321",
	  "SIP/2.0/UDP 192.168.1.100:5060;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
		97 ) == false ) return false;

	// IPv6 Å×½ºÆ®
	if( Test( 
		"SIP/2.0/UDP [5f05:2000:80ad:5800:58:800:2023:1d71]",
	  "SIP/2.0/UDP [5f05:2000:80ad:5800:58:800:2023:1d71]",
		50 ) == false ) return false;
	if( Test( 
		"SIP/2.0/UDP [5f05:2000:80ad:5800:58:800:2023:1d71]:5060",
	  "SIP/2.0/UDP [5f05:2000:80ad:5800:58:800:2023:1d71]:5060",
		55 ) == false ) return false;
	if( Test( 
		"SIP/2.0/UDP [5f05:2000:80ad:5800:58:800:2023:1d71]:5060;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
	  "SIP/2.0/UDP [5f05:2000:80ad:5800:58:800:2023:1d71]:5060;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
		122 ) == false ) return false;
	if( Test(  
		"SIP/2.0/UDP [5f05:2000:80ad:5800:58:800:2023:1d71]:5060 ;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
	  "SIP/2.0/UDP [5f05:2000:80ad:5800:58:800:2023:1d71]:5060;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
		123 ) == false ) return false;
	if( Test(  
		"SIP/2.0/UDP [5f05:2000:80ad:5800:58:800:2023:1d71]:5060 ; ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
	  "SIP/2.0/UDP [5f05:2000:80ad:5800:58:800:2023:1d71]:5060;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
		124 ) == false ) return false;
	if( Test(  
		"SIP/2.0/UDP [5f05:2000:80ad:5800:58:800:2023:1d71];ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
	  "SIP/2.0/UDP [5f05:2000:80ad:5800:58:800:2023:1d71];ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
		117 ) == false ) return false;
	if( Test( 		
		"SIP/2.0/UDP [5f05:2000:80ad:5800:58:800:2023:1d71]:5060;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890,SIP/2.0/UDP 1.1.1.1:5060;ttl=60;maddr=2.2.2.2;received=3.3.3.3;branch=0987654321",
	  "SIP/2.0/UDP [5f05:2000:80ad:5800:58:800:2023:1d71]:5060;ttl=60;maddr=200.201.203.205;received=203.44.2.2;branch=1234567890",
		122 ) == false ) return false;

	return true;
}
