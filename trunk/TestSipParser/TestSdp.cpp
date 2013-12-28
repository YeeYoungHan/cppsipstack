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
#include <string.h>
#include <stdio.h>

static bool Test( const char * pszText, const char * pszResult )
{
	CSdpMessage	clsSdp;
	int		iPos, iLen, iTextLen = (int)strlen(pszText);
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

static bool Test2( const char * pszText )
{
	CSdpMessage	clsSdp;
	int		iPos, iTextLen = (int)strlen(pszText);
	char	szResult[512];

	memset( szResult, 0, sizeof(szResult) );

	iPos = clsSdp.Parse( pszText, iTextLen );
	if( iPos == -1 )
	{
		printf( "sdp(%s) parse error\n", pszText );
		return false;
	}

	CSdpMedia clsNewMedia = *(clsSdp.m_clsMediaList.begin());
	SDP_MEDIA_LIST clsMediaList = clsSdp.m_clsMediaList;
	SDP_ATTRIBUTE_LIST::iterator itAttr;
	int arrPT[] = { -1, 103, 102, 104, 117, 117, 3, 0, 8, 9, -1, 101, 101 };
	char * arrEN[] = { "", "speex", "speex", "speex", "iLBC", "", "GSM", "PCMU", "PCMA", "G722", "", "telephone-event", "" };
	int iIndex = 0;
	std::string strEN;

	for( itAttr = clsNewMedia.m_clsAttributeList.begin(); itAttr != clsNewMedia.m_clsAttributeList.end(); ++itAttr )
	{
		int iPT = itAttr->GetPayLoadType();

		if( iPT != arrPT[iIndex] )
		{
			printf( "iPT(%d) != arrPT[%d](%d)\n", iPT, iIndex, arrPT[iIndex] );
			return false;
		}

		itAttr->GetEncodingName( strEN );
		if( strcmp( strEN.c_str(), arrEN[iIndex] ) )
		{
			printf( "strEN(%s) != arrEN[%d](%s)\n", strEN.c_str(), iIndex, arrEN[iIndex] );
			return false;
		}

		++iIndex;
	}

	clsSdp.Clear();

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

	Test2( "v=0\r\n"
					"o=- 3593775777 3593775777 IN IP4 192.168.184.129\r\n"
					"s=media\r\n"
					"c=IN IP4 192.168.184.129\r\n"
					"t=0 0\r\n"
					"a=X-nat:0\r\n"
					"m=audio 11263 RTP/AVP 103 102 104 117 3 0 8 9 101\r\n"
					"a=rtcp:11264 IN IP4 192.168.184.129\r\n"
					"a=rtpmap:103 speex/16000\r\n"
					"a=rtpmap:102 speex/8000\r\n"
					"a=rtpmap:104 speex/32000\r\n"
					"a=rtpmap:117 iLBC/8000\r\n"
					"a=fmtp:117 mode=30\r\n"
					"a=rtpmap:3 GSM/8000\r\n"
					"a=rtpmap:0 PCMU/8000\r\n"
					"a=rtpmap:8 PCMA/8000\r\n"
					"a=rtpmap:9 G722/8000\r\n"
					"a=sendrecv\r\n"
					"a=rtpmap:101 telephone-event/8000\r\n"
					"a=fmtp:101 0-15\r\n" );

	return true;
}
