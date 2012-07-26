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

#include "SdpMessage.h"

static bool Test( const char * pszText, const char * pszResult )
{
	CSdpMessage	clsSdp;
	int		iPos, iLen, iTextLen = strlen(pszText);
	char	szResult[512];

	memset( szResult, 0, sizeof(szResult) );

	iPos = clsSdp.Parse( pszText, iTextLen );
	if( iPos == -1 )
	{
		printf( "sdp(%s) parse error\n", pszText );
		return false;
	}

	if( iPos != iTextLen ) 
	{
		printf( "sdp(%s) parse len(%d) != real len(%d)\n", pszText, iPos, iTextLen );
		return false;
	}

	iLen = clsSdp.ToString( szResult, sizeof(szResult) );
	if( strcmp( pszResult, szResult ) )
	{
		printf( "sdp(%s) != result(%s)\n", pszResult, szResult );
		return false;
	}

	return true;
}

bool TestSdp()
{
		if( Test( 
			"v=0\r\n"
			"o=jdoe 2890844526 2890842807 IN IP4 10.47.16.5\r\n"
			"s=SDP Seminar\r\n"
			"i=A Seminar on the session description protocol\r\n"
			"u=http://www.example.com/seminars/sdp.pdf\r\n"
			"e=j.doe@example.com (Jane Doe)\r\n"
			"c=IN IP4 224.2.17.12/127\r\n"
			"t=2873397496 2873404696\r\n"
			"a=recvonly\r\n"
			"m=audio 49170 RTP/AVP 0\r\n"
			"m=video 51372 RTP/AVP 99\r\n"
			"a=rtpmap:99 h263-1998/90000\r\n",
			"v=0\r\n"
			"o=jdoe 2890844526 2890842807 IN IP4 10.47.16.5\r\n"
			"s=SDP Seminar\r\n"
			"i=A Seminar on the session description protocol\r\n"
			"u=http://www.example.com/seminars/sdp.pdf\r\n"
			"e=j.doe@example.com (Jane Doe)\r\n"
			"c=IN IP4 224.2.17.12/127\r\n"
			"t=2873397496 2873404696\r\n"
			"a=recvonly\r\n"
			"m=audio 49170 RTP/AVP 0\r\n"
			"m=video 51372 RTP/AVP 99\r\n"
			"a=rtpmap:99 h263-1998/90000\r\n" ) == false ) return false;
		if( Test( 
			"v=0\r\n"
			"o=jdoe 2890844526 2890842807 IN IP4 10.47.16.5\r\n"
			"s=SDP Seminar\r\n"
			"i=A Seminar on the session description protocol\r\n"
			"u=http://www.example.com/seminars/sdp.pdf\r\n"
			"e=j.doe@example.com (Jane Doe)\r\n"
			"c=IN IP4 224.2.17.12/127\r\n"
			"b=X-YZ:128\r\n",
			"v=0\r\n"
			"o=jdoe 2890844526 2890842807 IN IP4 10.47.16.5\r\n"
			"s=SDP Seminar\r\n"
			"i=A Seminar on the session description protocol\r\n"
			"u=http://www.example.com/seminars/sdp.pdf\r\n"
			"e=j.doe@example.com (Jane Doe)\r\n"
			"c=IN IP4 224.2.17.12/127\r\n"
			"b=X-YZ:128\r\n" ) == false ) return false;


	return true;
}
