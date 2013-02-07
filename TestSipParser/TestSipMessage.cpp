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
#include "SipMessage.h"
#include <string.h>
#include <stdio.h>

static bool Test( const char * pszText, const char * pszResult )
{
	CSipMessage clsMessage;
	int		iTextLen = (int)strlen( pszText );
	int		iPos, iLen;
	char	szResult[2000];

	memset( szResult, 0, sizeof(szResult) );

	iPos = clsMessage.Parse( pszText, iTextLen );
	if( iPos == -1 )
	{
		printf( "sip message(%s) parse error\n", pszText );
		return false;
	}

	iLen = clsMessage.ToString( szResult, sizeof(szResult) );
	if( strcmp( pszResult, szResult ) )
	{
		printf( "sip message(%s) != result(%s)\n", pszResult, szResult );
		return false;
	}

	return true;
}

bool TestSipMessage()
{
	// Body
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"Content-Length: 3\r\n"
			"\r\n"
			"123",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Content-Length: 3\r\n"
			"\r\n"
			"123" ) == false ) return false;
	
	// Contact 에서 LSS_From 저장시 expires 필드 길이를 처리하지 못 하는 버그 점검
	if( Test(
			"SIP/2.0 200 OK\r\n"
			"Via: SIP/2.0/UDP 203.255.208.41:5062;rport=5062;branch=z9hG4bKLss4c3bb3ef441d8671f-b0ce901;received=192.168.216.1\r\n"
			"From: <sip:1000@192.168.216.133>;tag=12345678\r\n"
			"To: <sip:1000@192.168.216.133>;tag=Nr51y7H661NXK\r\n"
			"Call-ID: 12345678@203.255.208.41\r\n"
			"CSeq: 2 REGISTER\r\n"
			"Contact: <sip:1000@203.255.208.41:5062>;expires=300\r\n"
			"Date: Tue, 13 Jul 2010 00:32:10 GMT\r\n"
			"User-Agent: FreeSWITCH-mod_sofia/1.0.6-svn-exported\r\n"
			"Allow: INVITE, ACK, BYE, CANCEL, OPTIONS, MESSAGE, UPDATE, INFO, REGISTER, REFER, NOTIFY, PUBLISH, SUBSCRIBE\r\n"
			"Supported: timer, precondition, path, replaces\r\n"
			"Content-Length: 0\r\n"
			"\r\n",
			"SIP/2.0 200 OK\r\n"
			"Via: SIP/2.0/UDP 203.255.208.41:5062;rport=5062;branch=z9hG4bKLss4c3bb3ef441d8671f-b0ce901;received=192.168.216.1\r\n"
			"From: <sip:1000@192.168.216.133>;tag=12345678\r\n"
			"To: <sip:1000@192.168.216.133>;tag=Nr51y7H661NXK\r\n"
			"Call-ID: 12345678@203.255.208.41\r\n"
			"CSeq: 2 REGISTER\r\n"
			"Contact: <sip:1000@203.255.208.41:5062>;expires=300\r\n"
			"Content-Length: 0\r\n"
			"User-Agent: FreeSWITCH-mod_sofia/1.0.6-svn-exported\r\n"
			"Date: Tue, 13 Jul 2010 00:32:10 GMT\r\n"
			"Allow: INVITE, ACK, BYE, CANCEL, OPTIONS, MESSAGE, UPDATE, INFO, REGISTER, REFER, NOTIFY, PUBLISH, SUBSCRIBE\r\n"
			"Supported: timer, precondition, path, replaces\r\n"
			"\r\n" ) == false ) return false;
	
	// Min-Expires
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Max-Forwards: 70\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Accept: application/sdp; level=1, application/x-private; level=2; g=1, text/html\r\n"
			"Accept-Encoding: gzip;q=1.0, identity; q=0.5, *;q=0\r\n"
			"Accept-Language: da, en-gb;q=0.8, en;q=0.7\r\n"
			"Expires: 7200\r\n"
			"Min-Expires: 60\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Max-Forwards: 70\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Content-Length: 0\r\n"
			"Accept: application/sdp;level=1, application/x-private;level=2;g=1, text/html\r\n"
			"Accept-Encoding: gzip;q=1.0, identity;q=0.5, *;q=0\r\n"
			"Accept-Language: da, en-gb;q=0.8, en;q=0.7\r\n"
			"Expires: 7200\r\n"
			"Min-Expires: 60\r\n"
			"\r\n" ) == false ) return false;
	
	// Expires
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Max-Forwards: 70\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Accept: application/sdp; level=1, application/x-private; level=2; g=1, text/html\r\n"
			"Accept-Encoding: gzip;q=1.0, identity; q=0.5, *;q=0\r\n"
			"Accept-Language: da, en-gb;q=0.8, en;q=0.7\r\n"
			"Expires: 7200\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Max-Forwards: 70\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Content-Length: 0\r\n"
			"Accept: application/sdp;level=1, application/x-private;level=2;g=1, text/html\r\n"
			"Accept-Encoding: gzip;q=1.0, identity;q=0.5, *;q=0\r\n"
			"Accept-Language: da, en-gb;q=0.8, en;q=0.7\r\n"
			"Expires: 7200\r\n"
			"\r\n" ) == false ) return false;
	
	// Max-Forwards
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Max-Forwards: 70\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Accept: application/sdp; level=1, application/x-private; level=2; g=1, text/html\r\n"
			"Accept-Encoding: gzip;q=1.0, identity; q=0.5, *;q=0\r\n"
			"Accept-Language: da, en-gb;q=0.8, en;q=0.7\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Max-Forwards: 70\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Content-Length: 0\r\n"
			"Accept: application/sdp;level=1, application/x-private;level=2;g=1, text/html\r\n"
			"Accept-Encoding: gzip;q=1.0, identity;q=0.5, *;q=0\r\n"
			"Accept-Language: da, en-gb;q=0.8, en;q=0.7\r\n"
			"\r\n" ) == false ) return false;
	
	// Accept-Language
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Accept: application/sdp; level=1, application/x-private; level=2; g=1, text/html\r\n"
			"Accept-Encoding: gzip;q=1.0, identity; q=0.5, *;q=0\r\n"
			"Accept-Language: da, en-gb;q=0.8, en;q=0.7\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Content-Length: 0\r\n"
			"Accept: application/sdp;level=1, application/x-private;level=2;g=1, text/html\r\n"
			"Accept-Encoding: gzip;q=1.0, identity;q=0.5, *;q=0\r\n"
			"Accept-Language: da, en-gb;q=0.8, en;q=0.7\r\n"
			"\r\n" ) == false ) return false;
			
	// Accept-Encoding
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Accept: application/sdp; level=1, application/x-private; level=2; g=1, text/html\r\n"
			"Accept-Encoding: gzip;q=1.0, identity; q=0.5, *;q=0\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Content-Length: 0\r\n"
			"Accept: application/sdp;level=1, application/x-private;level=2;g=1, text/html\r\n"
			"Accept-Encoding: gzip;q=1.0, identity;q=0.5, *;q=0\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Accept: application/sdp; level=1, application/x-private; level=2; g=1, text/html\r\n"
			"Accept-Encoding: gzip\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Content-Length: 0\r\n"
			"Accept: application/sdp;level=1, application/x-private;level=2;g=1, text/html\r\n"
			"Accept-Encoding: gzip\r\n"
			"\r\n" ) == false ) return false;
	
	// Accept
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Accept: application/sdp; level=1, application/x-private; level=2; g=1, text/html\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Content-Length: 0\r\n"
			"Accept: application/sdp;level=1, application/x-private;level=2;g=1, text/html\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Accept: application/sdp;level=1, application/x-private, text/html\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Content-Length: 0\r\n"
			"Accept: application/sdp;level=1, application/x-private, text/html\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Accept: application/sdp\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Content-Length: 0\r\n"
			"Accept: application/sdp\r\n"
			"\r\n" ) == false ) return false;
	
	// Header
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Ip: 192.168.0.200\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Content-Length: 0\r\n"
			"Ip: 192.168.0.200\r\n"
			"\r\n" ) == false ) return false;
	
	// Content-Type
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Type: application/sdp\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	// Proxy-Authenticate
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"WWW-Authenticate: Digest realm=\"biloxi.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"domain=\"biloxi.com\", opaque=\"5ccc069c403ebaf9f0171e9517f40e41\", qop=auth\r\n"
			"Proxy-Authenticate: Digest realm=\"biloxi1.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"domain=\"biloxi1.com\", opaque=\"5ccc069c403ebaf9f0171e9517f40e41\", qop=auth-int\r\n"
			"Proxy-Authenticate: Digest realm=\"biloxi2.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c094\", "
				"domain=\"biloxi2.com\", opaque=\"5ccc069c403ebaf9f0171e9517f40e43\", qop=auth-int\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"WWW-Authenticate: Digest realm=\"biloxi.com\", domain=\"biloxi.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e41\", qop=auth\r\n"
			"Proxy-Authenticate: Digest realm=\"biloxi1.com\", domain=\"biloxi1.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e41\", qop=auth-int\r\n"
			"Proxy-Authenticate: Digest realm=\"biloxi2.com\", domain=\"biloxi2.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c094\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e43\", qop=auth-int\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"WWW-Authenticate: Digest realm=\"biloxi.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"domain=\"biloxi.com\", opaque=\"5ccc069c403ebaf9f0171e9517f40e41\", qop=auth\r\n"
			"Proxy-Authenticate: Digest realm=\"biloxi1.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"domain=\"biloxi1.com\", opaque=\"5ccc069c403ebaf9f0171e9517f40e41\", qop=auth-int\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"WWW-Authenticate: Digest realm=\"biloxi.com\", domain=\"biloxi.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e41\", qop=auth\r\n"
			"Proxy-Authenticate: Digest realm=\"biloxi1.com\", domain=\"biloxi1.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e41\", qop=auth-int\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;	
	
	// WWW-Authenticate
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"WWW-Authenticate: Digest realm=\"biloxi.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"domain=\"biloxi.com\", opaque=\"5ccc069c403ebaf9f0171e9517f40e41\", qop=auth\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"WWW-Authenticate: Digest realm=\"biloxi.com\", domain=\"biloxi.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e41\", qop=auth\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;	

	// Proxy-Authorization
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>,\r\n"
	    "       <sip:bigbox3.site3.atlanta.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Authorization: Digest username=\"bob\", realm=\"biloxi.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"uri=\"sip:bob@biloxi.com\", qop=auth, nc=00000001, cnonce=\"0a4f113b\", response=\"6629fae49393a05397450978507c4ef1\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e41\"\r\n"
			"Proxy-Authorization: Digest username=\"bob1\", realm=\"biloxi2.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c094\", "
				"uri=\"sip:bob@biloxi2.com\", qop=auth-int, nc=00000002, cnonce=\"0a4f113c\", response=\"6629fae49393a05397450978507c4ef3\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e43\"\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "Route: <sip:bigbox3.site3.atlanta.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Authorization: Digest username=\"bob\", realm=\"biloxi.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"uri=\"sip:bob@biloxi.com\", response=\"6629fae49393a05397450978507c4ef1\", cnonce=\"0a4f113b\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e41\", "
				"qop=auth, nc=00000001\r\n"
			"Proxy-Authorization: Digest username=\"bob1\", realm=\"biloxi2.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c094\", "
				"uri=\"sip:bob@biloxi2.com\", response=\"6629fae49393a05397450978507c4ef3\", cnonce=\"0a4f113c\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e43\", "
				"qop=auth-int, nc=00000002\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>,\r\n"
	    "       <sip:bigbox3.site3.atlanta.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Proxy-Authorization: Digest username=\"bob\", realm=\"biloxi.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"uri=\"sip:bob@biloxi.com\", qop=auth, nc=00000001, cnonce=\"0a4f113b\", response=\"6629fae49393a05397450978507c4ef1\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e41\"\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "Route: <sip:bigbox3.site3.atlanta.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Proxy-Authorization: Digest username=\"bob\", realm=\"biloxi.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"uri=\"sip:bob@biloxi.com\", response=\"6629fae49393a05397450978507c4ef1\", cnonce=\"0a4f113b\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e41\", "
			"qop=auth, nc=00000001\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	// Authorization
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>,\r\n"
	    "       <sip:bigbox3.site3.atlanta.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Authorization: Digest username=\"bob\", realm=\"biloxi.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"uri=\"sip:bob@biloxi.com\", qop=auth, nc=00000001, cnonce=\"0a4f113b\", response=\"6629fae49393a05397450978507c4ef1\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e41\"\r\n"
			"Authorization: Digest username=\"bob1\", realm=\"biloxi2.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c094\", "
				"uri=\"sip:bob@biloxi2.com\", qop=auth-int, nc=00000002, cnonce=\"0a4f113c\", response=\"6629fae49393a05397450978507c4ef3\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e43\"\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "Route: <sip:bigbox3.site3.atlanta.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Authorization: Digest username=\"bob\", realm=\"biloxi.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"uri=\"sip:bob@biloxi.com\", response=\"6629fae49393a05397450978507c4ef1\", cnonce=\"0a4f113b\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e41\", "
				"qop=auth, nc=00000001\r\n"
			"Authorization: Digest username=\"bob1\", realm=\"biloxi2.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c094\", "
				"uri=\"sip:bob@biloxi2.com\", response=\"6629fae49393a05397450978507c4ef3\", cnonce=\"0a4f113c\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e43\", "
				"qop=auth-int, nc=00000002\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>,\r\n"
	    "       <sip:bigbox3.site3.atlanta.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Authorization: Digest username=\"bob\", realm=\"biloxi.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"uri=\"sip:bob@biloxi.com\", qop=auth, nc=00000001, cnonce=\"0a4f113b\", response=\"6629fae49393a05397450978507c4ef1\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e41\"\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "Route: <sip:bigbox3.site3.atlanta.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Authorization: Digest username=\"bob\", realm=\"biloxi.com\", nonce=\"dcd98b7102dd2f0e8b11d0f600bfb0c093\", "
				"uri=\"sip:bob@biloxi.com\", response=\"6629fae49393a05397450978507c4ef1\", cnonce=\"0a4f113b\", "
				"opaque=\"5ccc069c403ebaf9f0171e9517f40e41\", "
			"qop=auth, nc=00000001\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	// Route
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Route: <sip:server10.biloxi.com;lr>,\r\n"
	    "       <sip:bigbox3.site3.atlanta.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Route: <sip:server10.biloxi.com;lr>\r\n"
	    "Route: <sip:bigbox3.site3.atlanta.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	// Record-Route
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Record-Route: <sip:server10.biloxi.com;lr>,\r\n"
	    "                <sip:bigbox3.site3.atlanta.com;lr>\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
			"Record-Route: <sip:server10.biloxi.com;lr>\r\n"
	    "Record-Route: <sip:bigbox3.site3.atlanta.com;lr>\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	// Contact
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>, <sip:alice2@pc34.atlanta.com:5060>,<sip:alice3@pc35.atlanta.com:5080>\r\n"
			"\r\n", 
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Contact: <sip:alice2@pc34.atlanta.com:5060>\r\n"
			"Contact: <sip:alice3@pc35.atlanta.com:5080>\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Contact: <sip:alice2@pc34.atlanta.com:5060>\r\n"
			"Contact: <sip:alice3@pc35.atlanta.com:5080>\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Contact: <sip:alice2@pc34.atlanta.com:5060>\r\n"
			"Contact: <sip:alice3@pc35.atlanta.com:5080>\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"m: <sip:alice2@pc33.atlanta.com:5060>\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Contact: <sip:alice2@pc33.atlanta.com:5060>\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>, <sip:alice2@pc33.atlanta.com:5060>\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Contact: <sip:alice2@pc33.atlanta.com:5060>\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Contact: <sip:alice@pc33.atlanta.com>\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	// Call-ID
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID:       carol@192.168.1.100\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID    :  carol@192.168.1.100\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Call-ID: carol@192.168.1.100\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	// CSeq
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq: 1      OPTIONS\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;

	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"CSeq:              1      OPTIONS\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"CSeq: 1 OPTIONS\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	// Via
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;ttl=20;maddr=1.1.1.1;received=3.3.3.3:5090;branch=123456\r\n"
			"Via: SIP/2.0/UDP 192.168.2.100:5060;ttl=30;maddr=1.2.2.1;received=2.2.2.2:5070;branch=8\r\n"
			"Via: SIP/2.0/UDP 192.168.3.100:5080;ttl=40;maddr=2.2.2.1;received=2.2.2.3:5060;branch=9\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;ttl=20;maddr=1.1.1.1;received=3.3.3.3:5090;branch=123456\r\n"
			"Via: SIP/2.0/UDP 192.168.2.100:5060;ttl=30;maddr=1.2.2.1;received=2.2.2.2:5070;branch=8\r\n"
			"Via: SIP/2.0/UDP 192.168.3.100:5080;ttl=40;maddr=2.2.2.1;received=2.2.2.3:5060;branch=9\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;ttl=20;maddr=1.1.1.1;received=3.3.3.3:5090;branch=123456\r\n"
			"Via: SIP/2.0/UDP 192.168.2.100:5060;ttl=20;maddr=1.2.2.1;received=2.2.2.2:5070;branch=8\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;ttl=20;maddr=1.1.1.1;received=3.3.3.3:5090;branch=123456\r\n"
			"Via: SIP/2.0/UDP 192.168.2.100:5060;ttl=20;maddr=1.2.2.1;received=2.2.2.2:5070;branch=8\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;ttl=20;maddr=1.1.1.1;received=3.3.3.3:5090;branch=123456,"
			" SIP/2.0/UDP 192.168.2.100:5060;ttl=20;maddr=1.2.2.1;received=2.2.2.2:5070;branch=8\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;ttl=20;maddr=1.1.1.1;received=3.3.3.3:5090;branch=123456\r\n"
			"Via: SIP/2.0/UDP 192.168.2.100:5060;ttl=20;maddr=1.2.2.1;received=2.2.2.2:5070;branch=8\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Content-Length: 0\r\n"
			"\r\n") == false ) return false;
	
	if( Test(
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;ttl=20;maddr=1.1.1.1;received=3.3.3.3:5090;branch=123456,"
			" SIP/2.0/UDP 192.168.2.100:5060;ttl=20;maddr=1.2.2.1;received=2.2.2.2:5070;branch=8,"
			"SIP/2.0/UDP 192.168.3.100:5080;ttl=40;maddr=2.2.2.1;received=2.2.2.3:5060;branch=9\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
	    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
			"\r\n",
			"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
			"Via: SIP/2.0/UDP 192.168.1.100:5060;ttl=20;maddr=1.1.1.1;received=3.3.3.3:5090;branch=123456\r\n"
			"Via: SIP/2.0/UDP 192.168.2.100:5060;ttl=20;maddr=1.2.2.1;received=2.2.2.2:5070;branch=8\r\n"
			"Via: SIP/2.0/UDP 192.168.3.100:5080;ttl=40;maddr=2.2.2.1;received=2.2.2.3:5060;branch=9\r\n"
	    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
	    "To: <sip:carol@chicago.com>\r\n"
			"Content-Length: 0\r\n"
			"\r\n" ) == false ) return false;
	
	if( Test(
		"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
		"Via: SIP/2.0/UDP 192.168.1.100:5060;ttl=20;maddr=1.1.1.1;received=3.3.3.3:5090;branch=123456\r\n"
    "To: <sip:carol@chicago.com>\r\n"
    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
		"\r\n",
		"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
		"Via: SIP/2.0/UDP 192.168.1.100:5060;ttl=20;maddr=1.1.1.1;received=3.3.3.3:5090;branch=123456\r\n"
    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
    "To: <sip:carol@chicago.com>\r\n"
		"Content-Length: 0\r\n"
		"\r\n" ) == false ) return false;

	if( Test(
		"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
		"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
    "To: <sip:carol@chicago.com>\r\n"
    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
		"\r\n",
		"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
		"Via: SIP/2.0/UDP 192.168.1.100:5060;branch=123456\r\n"
    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
    "To: <sip:carol@chicago.com>\r\n"
		"Content-Length: 0\r\n"
		"\r\n") == false ) return false;

	if( Test(
		"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
		"Via: SIP/2.0/UDP 192.168.1.100\r\n"
    "To: <sip:carol@chicago.com>\r\n"
    "From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
		"\r\n",
		"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
		"Via: SIP/2.0/UDP 192.168.1.100\r\n"
    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
    "To: <sip:carol@chicago.com>\r\n"
		"Content-Length: 0\r\n"
		"\r\n" ) == false ) return false;

	// From/To
	if( Test( 
		"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
		"To: <sip:carol@chicago.com>\r\n"
  	"From: Alice <sip:alice@atlanta.com>;tag=1928301774\r\n"
		"\r\n",
		"OPTIONS sip:carol@chicago.com SIP/2.0\r\n"
    "From: \"Alice\" <sip:alice@atlanta.com>;tag=1928301774\r\n"
    "To: <sip:carol@chicago.com>\r\n"
		"Content-Length: 0\r\n"
		"\r\n" ) == false ) return false;

	return true;
}
