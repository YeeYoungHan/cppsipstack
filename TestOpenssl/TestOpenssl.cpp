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

#include "TestOpenssl.h"

int giTcpServerConnectCount = 2;
int giTcpServerLoopCount = 1;

int main( int argc, char * argv[] )
{
#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
#endif

	if( argc != 3 )
	{
		printf( "[Usage] %s {pem filename} {tcpserver|tcpclient|tcp|udp}\n", argv[0] );
		return 0;
	}

	InitNetwork();

	if( !strcmp( argv[2], "tcpserver" ) )
	{
		// TLS 서버를 시작한다.
		TcpServer( argv[1] );
	}
	else if( !strcmp( argv[2], "tcpclient" ) )
	{
		// TLS 클라이언트를 시작한다.
		TcpClient( argv[1] );
	}
	else if( !strcmp( argv[2], "tcp" ) )
	{
		// TLS 서버/클라이언트를 시작한다.
		TcpServerClient( argv[1] );
	}
	else
	{
		// DTLS 서버/클라이언트를 시작한다.
		UdpServer( argv[1] );
	}

	return 0;
}
