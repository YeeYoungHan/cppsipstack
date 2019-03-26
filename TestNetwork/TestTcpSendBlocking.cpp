#include "TestNetwork.h"

#define LISTEN_PORT	4000

THREAD_API TcpClientThread( LPVOID lpParameter )
{
	Socket hConn = TcpConnect( "127.0.0.1", LISTEN_PORT );
	if( hConn == INVALID_SOCKET )
	{
		printf( "TcpConnect error\n" );
		return 0;
	}

	char szBuf[8192];
	int n = 0, iCount = 0;

	while( 1 )
	{
		++iCount;

		printf( "before send\n" );
		n = send( hConn, szBuf, sizeof(szBuf), 0 );
		printf( "after send n(%d) count(%d)\n", n, iCount );
	}

	return 0;
}

bool TestTcpSendBlocking( )
{
	Socket hListen = TcpListen( LISTEN_PORT, 255, "127.0.0.1" );
	if( hListen == INVALID_SOCKET )
	{
		printf( "TcpListen() error\n" );
		return false;
	}

	StartThread( "TcpClientThread", TcpClientThread, NULL );

	char szIp[52];
	int iPort;

	Socket hConn = TcpAccept( hListen, szIp, sizeof(szIp), &iPort );

	while( 1 )
	{
		sleep(1);
	}

	return true;
}
