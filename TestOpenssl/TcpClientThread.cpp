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

THREAD_API TcpClientThread( LPVOID lpParameter )
{
	Socket hSocket = TcpConnect( gpszServerIp, 3333 );
	if( hSocket == INVALID_SOCKET )
	{
		printf( "TcpConnect() error\n" );
		return 0;
	}

	SSL * psttSsl;
	char	szPacket[8192];
	int n, iSize = 0, iCount = 0, iErrorCount = 0;

	if( SSLConnect( hSocket, &psttSsl ) == false )
	{
		printf( "SSLConnect() error\n" );
		return 0;
	}

	StartTcpSendThread( psttSsl );

	while( 1 )
	{
		n = SSLRecv( psttSsl, szPacket, sizeof(szPacket) );
		if( n <= 0 )
		{
			printf( "SSLRecv error\n" );
			if( gbTcpServerRenegotiate )
			{
				++iErrorCount;
				if( iErrorCount == 2 )
				{
					break;
				}
				else
				{
					continue;
				}
			}
			else
			{
				break;
			}
		}

		if( gbTcpClientRenegotiate )
		{
			if( SSL_renegotiate(psttSsl) <= 0 )
			{
				printf( "SSL_renegotiate error\n" );
				break;
			}

			if( SSL_do_handshake(psttSsl) <= 0 )
			{
				printf( "SSL_do_handshake error\n" );
				break;
			}

			// 2번째 SSL_do_handshake() 호출시 에러가 발생하여서 일단 1번만 SSL_renegotiate() 호출하도록 수정함
			gbTcpClientRenegotiate = false;
		}

		iErrorCount = 0;
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
	closesocket( hSocket );

	ERR_remove_thread_state( NULL );

	return 0;
}

bool StartTcpClientThread()
{
	return StartThread( "TcpClientThread", TcpClientThread, NULL );
}

void TcpClient( const char * pszCertFile )
{
	SSLServerStart( pszCertFile, "" );
	TcpClientThread( 0 );
	SSLServerStop();
	SSLFinal();
}
