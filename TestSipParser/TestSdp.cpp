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
#include "SdpAttributeCrypto.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static bool Test( const char * pszText, const char * pszResult )
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

	if( iPos != iTextLen ) 
	{
		printf( "sdp(%s) parse len(%d) != real len(%d)\n", pszText, iPos, iTextLen );
		return false;
	}

	clsSdp.ToString( szResult, sizeof(szResult) );
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
	const char * arrEN[] = { "", "speex", "speex", "speex", "iLBC", "", "GSM", "PCMU", "PCMA", "G722", "", "telephone-event", "" };
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

static bool Test3( const char * pszText )
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
	SDP_ATTRIBUTE_LIST::iterator itAttr;
	const char * arrKey[] = { "profile-level-id", "max-br", "max-mbps", "max-fs", "max-smbps", "max-fps", "max-rcmd-nalu-size", NULL };
	const char * arrValue[] = { "428016", "5000", "490000", "9000", "490000", "6000", "3456000", NULL };

	for( itAttr = clsNewMedia.m_clsAttributeList.begin(); itAttr != clsNewMedia.m_clsAttributeList.end(); ++itAttr )
	{
		if( !strcmp( itAttr->m_strName.c_str(), "fmtp" ) )
		{
			/*
			CSipParameterList clsParamList;

			if( itAttr->GetParameterList( clsParamList ) == false ) return false;

			for( int i = 0; arrKey[i]; ++i )
			{
				const char * pszValue = clsParamList.SelectParamValue( arrKey[i] );
				if( pszValue == NULL || strcmp( pszValue, arrValue[i] ) ) return false;
			}
			*/
		}
	}

	clsSdp.Clear();

	return true;
}

static bool Test4( const char * pszText, const char * pszResult )
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

	if( iPos != iTextLen )
	{
		printf( "sdp(%s) parse len(%d) != real len(%d)\n", pszText, iPos, iTextLen );
		return false;
	}

	SDP_MEDIA_LIST::iterator itML;

	for( itML = clsSdp.m_clsMediaList.begin(); itML != clsSdp.m_clsMediaList.end(); ++itML )
	{
		if( !strcmp( itML->m_strMedia.c_str(), "audio" ) )
		{
			SDP_FMT_LIST clsFmtList = itML->m_clsFmtList;
			SDP_FMT_LIST::iterator itFL;

			for( itFL = clsFmtList.begin(); itFL != clsFmtList.end(); ++itFL )
			{
				int iCodec = atoi( itFL->c_str() );

				if( iCodec != 8 && iCodec != 101 )
				{
					itML->DeleteFmtAttribute( iCodec );
				}
			}
		}
	}

	clsSdp.ToString( szResult, sizeof(szResult) );
	if( strcmp( pszResult, szResult ) )
	{
		printf( "sdp(%s) != result(%s)\n", pszResult, szResult );
		return false;
	}

	return true;
}

static bool TestTopCodec( const char * pszText, const char * pszResult, int iCodec )
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

	if( iPos != iTextLen )
	{
		printf( "sdp(%s) parse len(%d) != real len(%d)\n", pszText, iPos, iTextLen );
		return false;
	}

	SDP_MEDIA_LIST::iterator itML;

	for( itML = clsSdp.m_clsMediaList.begin(); itML != clsSdp.m_clsMediaList.end(); ++itML )
	{
		if( !strcmp( itML->m_strMedia.c_str(), "audio" ) )
		{
			itML->MakeTopFmtAttribute( iCodec );
		}
	}

	clsSdp.ToString( szResult, sizeof(szResult) );
	if( strcmp( pszResult, szResult ) )
	{
		printf( "sdp(%s) != result(%s)\n", pszResult, szResult );
		return false;
	}

	return true;
}

static bool TestAudioMedia( const char * pszText )
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

	CSdpMedia * pclsMedia;

	if( clsSdp.SelectMedia( "audio", &pclsMedia ) == false )
	{
		printf( "sdp(%s) no audio media\n", pszText );
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

	// number of ports Å×½ºÆ®
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
		"m=audio 49170/2 RTP/AVP 0\r\n"
		"m=video 51372/3 RTP/AVP 99\r\n"
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
		"m=audio 49170/2 RTP/AVP 0\r\n"
		"m=video 51372/3 RTP/AVP 99\r\n"
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

	if( Test( 
		"v=0\r\n"
		"o=CiscoSystemsSIP-GW-UserAgent 7393 3874 IN IP4 192.10.228.41\r\n"
		"s=SIP Call\r\n"
		"c=IN IP4 192.10.228.41\r\n"
		"t=0 0\r\n"
		"m=audio 24864 RTP/AVP 0 101\r\n"
		"c=IN IP4 192.10.228.41\r\n"
		"a=rtpmap:0 PCMU/8000\r\n"
		"a=rtpmap:101 telephone-event/8000\r\n"
		"a=fmtp:101 0-16\r\n"
		"a=ptime:20\r\n"
		"\r\n"
		"--uniqueBoundary\r\n"
		"Content-Type: application/gtd\r\n"
		"Content-Disposition: signal;handling=optional\r\n"
		"\r\n"
		"IAM,\r\n"
		"PRN,isdn*,,NET5*,\r\n"
		"USI,rate,c,s,c,1\r\n"
		"USI,lay1,alaw\r\n"
		"TMR,00\r\n"
		"CPN,00,,1,2936\r\n"
		"CGN,04,,1,y,4,01075066103\r\n"
		"CPC,09\r\n"
		"FCI,,,,,,,y,\r\n"
		"GCI,5c2f3c7d861811e789221005ca2bd880\r\n"
		"--uniqueBoundary\r\n",
		"v=0\r\n"
		"o=CiscoSystemsSIP-GW-UserAgent 7393 3874 IN IP4 192.10.228.41\r\n"
		"s=SIP Call\r\n"
		"c=IN IP4 192.10.228.41\r\n"
		"t=0 0\r\n"
		"m=audio 24864 RTP/AVP 0 101\r\n"
		"c=IN IP4 192.10.228.41\r\n"
		"a=rtpmap:0 PCMU/8000\r\n"
		"a=rtpmap:101 telephone-event/8000\r\n"
		"a=fmtp:101 0-16\r\n"
		"a=ptime:20\r\n" ) == false ) return false;		

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

	Test3( "v=0\r\n"
					"o=- 3593775777 3593775777 IN IP4 192.168.184.129\r\n"
					"s=media\r\n"
					"c=IN IP4 192.168.184.129\r\n"
					"t=0 0\r\n"
					"a=X-nat:0\r\n"
					"m=video 11263 RTP/AVP 97\r\n"
					"a=rtpmap:97 H264/90000\r\n"
					"a=fmtp:97 profile-level-id=428016;max-br=5000;max-mbps=490000;max-fs=9000;max-smbps=490000;max-fps=6000;max-rcmd-nalu-size=3456000\r\n" );

	CSdpAttributeCrypto clsCrypto;
	const char * pszCrypto = "1 AES_CM_128_HMAC_SHA1_80 inline:7s65riA38OlE4U5OepU5zgaoMvzOL19nSQqeursI";

	if( clsCrypto.Parse( pszCrypto, (int)strlen(pszCrypto) ) == -1 )
	{
		printf( "crypto[%s] parser error\n", pszCrypto );
		return false;
	}

	if( strcmp( clsCrypto.m_strTag.c_str(), "1" ) || 
			strcmp( clsCrypto.m_strCryptoSuite.c_str(), "AES_CM_128_HMAC_SHA1_80" ) ||
			strcmp( clsCrypto.m_strKey.c_str(), "7s65riA38OlE4U5OepU5zgaoMvzOL19nSQqeursI" ) )
	{
		printf( "crypto[%s] parser result error\n", pszCrypto );
		return false;
	}

	if( TestAudioMedia( "v=0\r\n"
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

	if( Test4( "v=0\r\n"
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
					"a=fmtp:101 0-15\r\n"
				, "v=0\r\n"
					"o=- 3593775777 3593775777 IN IP4 192.168.184.129\r\n"
					"s=media\r\n"
					"c=IN IP4 192.168.184.129\r\n"
					"t=0 0\r\n"
					"a=X-nat:0\r\n"
					"m=audio 11263 RTP/AVP 8 101\r\n"
					"a=rtcp:11264 IN IP4 192.168.184.129\r\n"
					"a=rtpmap:8 PCMA/8000\r\n"
					"a=sendrecv\r\n"
					"a=rtpmap:101 telephone-event/8000\r\n"
					"a=fmtp:101 0-15\r\n") == false ) return false;

	if( TestTopCodec( "v=0\r\n"
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
					"a=fmtp:101 0-15\r\n"
				, "v=0\r\n"
					"o=- 3593775777 3593775777 IN IP4 192.168.184.129\r\n"
					"s=media\r\n"
					"c=IN IP4 192.168.184.129\r\n"
					"t=0 0\r\n"
					"a=X-nat:0\r\n"
					"m=audio 11263 RTP/AVP 8 103 102 104 117 3 0 9 101\r\n"
					"a=rtcp:11264 IN IP4 192.168.184.129\r\n"
					"a=rtpmap:8 PCMA/8000\r\n"
					"a=rtpmap:103 speex/16000\r\n"
					"a=rtpmap:102 speex/8000\r\n"
					"a=rtpmap:104 speex/32000\r\n"
					"a=rtpmap:117 iLBC/8000\r\n"
					"a=fmtp:117 mode=30\r\n"
					"a=rtpmap:3 GSM/8000\r\n"
					"a=rtpmap:0 PCMU/8000\r\n"
					"a=rtpmap:9 G722/8000\r\n"
					"a=sendrecv\r\n"
					"a=rtpmap:101 telephone-event/8000\r\n"
					"a=fmtp:101 0-15\r\n", 8 ) == false ) return false;

	if( TestTopCodec( "v=0\r\n"
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
					"a=fmtp:101 0-15\r\n"
				, "v=0\r\n"
					"o=- 3593775777 3593775777 IN IP4 192.168.184.129\r\n"
					"s=media\r\n"
					"c=IN IP4 192.168.184.129\r\n"
					"t=0 0\r\n"
					"a=X-nat:0\r\n"
					"m=audio 11263 RTP/AVP 117 103 102 104 3 0 8 9 101\r\n"
					"a=rtcp:11264 IN IP4 192.168.184.129\r\n"
					"a=rtpmap:117 iLBC/8000\r\n"
					"a=fmtp:117 mode=30\r\n"
					"a=rtpmap:103 speex/16000\r\n"
					"a=rtpmap:102 speex/8000\r\n"
					"a=rtpmap:104 speex/32000\r\n"
					"a=rtpmap:3 GSM/8000\r\n"
					"a=rtpmap:0 PCMU/8000\r\n"
					"a=rtpmap:8 PCMA/8000\r\n"
					"a=rtpmap:9 G722/8000\r\n"
					"a=sendrecv\r\n"
					"a=rtpmap:101 telephone-event/8000\r\n"
					"a=fmtp:101 0-15\r\n", 117 ) == false ) return false;

	if( TestTopCodec( "v=0\r\n"
					"o=- 3593775777 3593775777 IN IP4 192.168.184.129\r\n"
					"s=media\r\n"
					"c=IN IP4 192.168.184.129\r\n"
					"t=0 0\r\n"
					"a=X-nat:0\r\n"
					"m=audio 11263 RTP/AVP 8 101\r\n"
					"a=rtpmap:8 PCMA/8000\r\n"
					"a=sendrecv\r\n"
					"a=rtpmap:101 telephone-event/8000\r\n"
					"a=fmtp:101 0-15\r\n"
				, "v=0\r\n"
					"o=- 3593775777 3593775777 IN IP4 192.168.184.129\r\n"
					"s=media\r\n"
					"c=IN IP4 192.168.184.129\r\n"
					"t=0 0\r\n"
					"a=X-nat:0\r\n"
					"m=audio 11263 RTP/AVP 8 101\r\n"
					"a=rtpmap:8 PCMA/8000\r\n"
					"a=sendrecv\r\n"
					"a=rtpmap:101 telephone-event/8000\r\n"
					"a=fmtp:101 0-15\r\n", 8 ) == false ) return false;

	if( TestTopCodec( "v=0\r\n"
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
					"a=fmtp:101 0-15\r\n"
				, "v=0\r\n"
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
					"a=fmtp:101 0-15\r\n", 300 ) == false ) return false;

	CSdpMessage clsSdp;
	const char * pszT38 = "v=0\r\n"
		"o=iPECSCM 27404766 27404766 IN IP4 192.168.0.1\r\n"
		"s=iPECSCM Call\r\n"
		"c=IN IP4 192.168.0.1\r\n"
		"t=0 0\r\n"
		"m=image 8042 udptl t38\r\n"
		"a=T38FaxVersion:0\r\n"
		"a=T38MaxBitRate:14400\r\n"
		"a=T38FaxFillBitRemoval:0\r\n"
		"a=T38FaxTranscodingMMR:0\r\n"
		"a=T38FaxTranscodingJBIG:0\r\n"
		"a=T38FaxRateManagement:transferredTCF\r\n"
		"a=T38FaxUdpEC:t38UDPRedundancy\r\n";

	clsSdp.Parse( pszT38, (int)strlen(pszT38) );

	const char * pszAbnormalV = "v\r\n"
		"o==0\r\n"
		"o=- 1665110595 2 IN IP4 1.1.1.1\r\n"
		"s=-\r\n";
	
	clsSdp.Parse( pszAbnormalV, (int)strlen(pszAbnormalV) );

	return true;
}
