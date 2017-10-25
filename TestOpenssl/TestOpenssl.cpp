#include "TestOpenssl.h"


int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {pem filename}\n", argv[0] );
		return 0;
	}

	const char * pszCertFile = argv[1];
	char szIp[51];
	int iPort;

	InitNetwork();

	if( SSLServerStart( pszCertFile, "" ) == false )
	{
		printf( "SSLServerStart() error\n" );
		return 0;
	}

	Socket hListen = TcpListen( 3333, 255 );
	if( hListen == INVALID_SOCKET )
	{
		printf( "TcpListen() error\n" );
		return 0;
	}

	StartClientThread();

	Socket hConn = TcpAccept( hListen, szIp, sizeof(szIp), &iPort );
	if( hConn == INVALID_SOCKET )
	{
		printf( "TcpAccept() error\n" );
		return 0;
	}

	SSL * psttSsl;
	char	szPacket[8192];
	int n, iSize = 0, iCount = 0;

	if( SSLAccept( hConn, &psttSsl, false, 0, 5000 ) == false )
	{
		printf( "SSLAccept() error\n" );
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
