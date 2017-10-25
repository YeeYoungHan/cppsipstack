#include "TestOpenssl.h"

THREAD_API ClientThread( LPVOID lpParameter )
{
	Socket hSocket = TcpConnect( "127.0.0.1", 3333 );
	if( hSocket == INVALID_SOCKET )
	{
		printf( "TcpConnect() error\n" );
		return 0;
	}

	SSL * psttSsl;
	char	szPacket[8192];
	int n, iSize = 0, iCount = 0;

	if( SSLConnect( hSocket, &psttSsl ) == false )
	{
		printf( "SSLConnect() error\n" );
		return 0;
	}

	StartSendThread( psttSsl );

	while( 1 )
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

	return 0;
}

bool StartClientThread()
{
	return StartThread( "ClientThread", ClientThread, NULL );
}
