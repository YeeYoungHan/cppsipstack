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
int giUdpPort;
int giSipPort;

void PrintHelp( int argc, char * argv[] )
{
	printf( "[Usage] %s {listen udp port} {sip port} {log file path}\n", argv[0] );
}

int main( int argc, char * argv[] )
{
	if( argc != 4 )
	{
		PrintHelp( argc, argv );
		return 0;
	}

	giUdpPort = atoi( argv[1] );
	giSipPort = atoi( argv[2] );
	const char * pszLogFileName = argv[3];

	InitNetwork();

	ghSocket = UdpListen( giUdpPort, NULL );
	if( ghSocket == INVALID_SOCKET )
	{
		printf( "UdpListen(%d) error(%d)\n", giUdpPort, GetError() );
		return 0;
	}

	ReadLogFile( pszLogFileName );

	return 0;
}