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

#include "SipPlatformDefine.h"
#include "SipStatusCode.h"
#include "RtspServer.h"
#include "SipTcp.h"
#include "Log.h"
#include "RtspMessage.h"
#include "MemoryDebug.h"

int main( int argc, char * argv[] )
{
	InitNetwork();

	CLog::SetLevel( LOG_NETWORK | LOG_DEBUG | LOG_INFO );

	Socket hSocket = TcpListen( 554, 255 );
	if( hSocket == INVALID_SOCKET )
	{
		return 0;
	}

	char szIp[20], szPacket[8192];
	int iPort, n;
	CRtspMessage clsMessage, * pclsResponse;

	Socket hConn = TcpAccept( hSocket, szIp, sizeof(szIp), &iPort );
	
	while( 1 )
	{
		memset( szPacket, 0, sizeof(szPacket) );
		n = TcpRecv( hConn, szPacket, sizeof(szPacket), 5 );
		if( n <= 0 ) 
		{
			CLog::Print( LOG_DEBUG, "TcpRecv error n(%d)", n );
			break;
		}

		CLog::Print( LOG_NETWORK, "TcpRecv[%s]", szPacket );

		if( clsMessage.Parse( szPacket, n ) == -1 )
		{
			CLog::Print( LOG_ERROR, "clsMessage.Parse error" );
			break;
		}

		if( !strcasecmp( clsMessage.m_strRtspMethod.c_str(), "DESCRIBE" ) )
		{
			pclsResponse = clsMessage.CreateResponse( SIP_OK );
			if( pclsResponse == NULL ) break;

			char	szSdp[4096];
			int		iLen = 0;

			iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "v=0\r\n"
							"o=CSS 4 2 IN IP4 %s\r\n"
							"s=CSS\r\n", "127.0.0.1" );

			iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "c=IN IP4 %s\r\n", "127.0.0.1" );
			iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "t=0 0\r\n" );
			iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "m=audio 0 RTP/AVP 0" );

			pclsResponse->m_clsContentType.Set( "application", "sdp" );
			pclsResponse->m_iContentLength = iLen;
			pclsResponse->m_strBody = szSdp;
			n = pclsResponse->ToString( szPacket, sizeof(szPacket) );

			TcpSend( hConn, szPacket, n );
			CLog::Print( LOG_NETWORK, "TcpSend[%s]", szPacket );
		}
	}

	return 0;
}
