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
#include "TimeUtility.h"

void TcpServerClient( const char * pszCertFile )
{
	char szIp[51];
	int iPort;

	if( SSLServerStart( pszCertFile, "" ) == false )
	{
		printf( "SSLServerStart() error\n" );
		return;
	}

	Socket hListen = TcpListen( 3333, 255 );
	if( hListen == INVALID_SOCKET )
	{
		printf( "TcpListen() error\n" );
		return;
	}

	for( int c = 0; c < giTcpServerConnectCount; ++c )
	{
		StartTcpClientThread();

		Socket hConn = TcpAccept( hListen, szIp, sizeof(szIp), &iPort );
		if( hConn == INVALID_SOCKET )
		{
			printf( "TcpAccept() error\n" );
			return;
		}

		SSL * psttSsl;
		char	szPacket[8192];
		int n, iSize = 0, iCount = 0;

		if( SSLAccept( hConn, &psttSsl, false, 0, 0 ) == false )
		{
			printf( "SSLAccept() error\n" );
			return;
		}

		StartTcpSendThread( psttSsl );

		for( int i = 0; i < giTcpServerLoopCount; ++i )
		{
			n = SSLRecv( psttSsl, szPacket, sizeof(szPacket) );
			if( n <= 0 )
			{
				printf( "SSLRecv error\n" );
				break;
			}

			iSize += n;
			++iCount;

			if( iSize >= 1000000000 )
			{
				printf( "%s size(%d) count(%d)\n", __FUNCTION__, iSize, iCount );
				iSize = 0;
				iCount = 0;
			}
		}

		SSLClose( psttSsl );
		closesocket( hConn );

		MiliSleep( 20 );
	}

	MiliSleep( 100 );

	SSLServerStop();
	SSLFinal();
}
