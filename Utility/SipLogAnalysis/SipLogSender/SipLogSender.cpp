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

#include "SipLogSender.h"

Socket ghSocket;
const char * gpszSipIp;
int giSipPort;

void PrintHelp( int argc, char * argv[] )
{
	printf( "[Usage] %s {listen udp port} {sip ip} {sip port} {log file path}\n", argv[0] );
}

int main( int argc, char * argv[] )
{
	if( argc != 5 )
	{
		PrintHelp( argc, argv );
		return 0;
	}

	int iListenUdpPort = atoi( argv[1] );
	gpszSipIp = argv[2];
	giSipPort = atoi( argv[3] );
	const char * pszLogFileName = argv[4];

	InitNetwork();

	ghSocket = UdpListen( iListenUdpPort, NULL );
	if( ghSocket == INVALID_SOCKET )
	{
		printf( "UdpListen(%d) error(%d)\n", iListenUdpPort, GetError() );
		return 0;
	}

	ReadLogFile( pszLogFileName );

	return 0;
}
