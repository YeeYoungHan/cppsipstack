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
#include "SipTcp.h"
#include "ServerUtility.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef WIN32

#include <io.h>
#define OPEN_READ_FLAG	( _O_RDONLY | _O_BINARY )
#define OPEN_WRITE_FLAG	( _O_WRONLY | _O_CREAT | _O_BINARY | _O_TRUNC )
#define OPEN_READ_MODE	_S_IREAD
#define OPEN_WRITE_MODE	( _S_IREAD | _S_IWRITE )

#define open		_open
#define close		_close

#else

#include <unistd.h>
#define OPEN_READ_FLAG	O_RDONLY
#define OPEN_WRITE_FLAG	( O_WRONLY | O_CREAT | O_TRUNC )
#define OPEN_READ_MODE	( S_IRUSR | S_IRGRP | S_IROTH )
#define OPEN_WRITE_MODE ( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )

#endif

void TestFileFd()
{
	const char * pszFileName = "/tmp/sipplatform.txt";

	for( int i = 0; i < 3; ++i )
	{
		int iFd = open( pszFileName, OPEN_WRITE_FLAG, OPEN_WRITE_MODE );
		if( iFd == -1 )
		{
			printf( "open(%s) error\n", pszFileName );
			return;
		}

		printf( "fd = %d\n" );
		close( iFd );

		iFd = open( pszFileName, OPEN_READ_FLAG, OPEN_READ_MODE );
		if( iFd == -1 )
		{
			printf( "open(%s) error\n", pszFileName );
			return;
		}

		printf( "fd = %d\n" );
		close( iFd );
	}
}

#define TCP_PORT	12022

THREAD_API ConnectThread( LPVOID lpParameter )
{
	for( int i = 0; i < 10; ++i )
	{
		SOCKET hConn = TcpConnect( "127.0.0.1", TCP_PORT );
		if( hConn == INVALID_SOCKET ) break;

		printf( "connected fd = %d\n", hConn );

		closesocket( hConn );
	}

	return 0;
}

void TestSocketFd()
{
	SOCKET hListen = TcpListen( TCP_PORT, 255 );
	if( hListen == INVALID_SOCKET )
	{
		printf( "TcpListen() error\n" );
		return;
	}

	printf( "listen socket = %d\n", hListen );

	StartThread( "ConnectThread", ConnectThread, NULL );

	char szIp[21];
	int iPort;

	for( int i = 0; i < 10; ++i )
	{
		SOCKET hConn = TcpAccept( hListen, szIp, sizeof(szIp), &iPort );

		printf( "accept fd = %d\n", hConn );

		closesocket( hConn );
	}

	sleep(1);
}
