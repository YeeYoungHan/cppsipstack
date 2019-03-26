#include "TestNetwork.h"

#define LISTEN_PORT	4000

class CTcpClientThreadArg
{
public:
	bool	m_bRun;
};

THREAD_API TcpClientThread( LPVOID lpParameter )
{
	CTcpClientThreadArg * pclsArg = (CTcpClientThreadArg *)lpParameter;

	Socket hConn = TcpConnect( "127.0.0.1", LISTEN_PORT );
	if( hConn == INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "TcpConnect error" );
	}
	else
	{
		char szBuf[8192];
		int n = 0, iCount = 0;

#ifdef WIN32
		int iTimeout = 10000;

		n = setsockopt( hConn, SOL_SOCKET, SO_SNDTIMEO, (const char*)&iTimeout, sizeof(iTimeout) );
#else
		struct timeval sttTime;

		sttTime.tv_sec = 10;
		sttTime.tv_usec = 0;

		n = setsockopt( hConn, SOL_SOCKET, SO_SNDTIMEO, (const char*)&sttTime, sizeof(sttTime) );
#endif

		CLog::Print( LOG_DEBUG, "setsockopt n(%d)", n );

		while( 1 )
		{
			++iCount;

			CLog::Print( LOG_DEBUG, "before send" );
			n = send( hConn, szBuf, sizeof(szBuf), 0 );
			CLog::Print( LOG_DEBUG, "after send n(%d) count(%d)", n, iCount );

			if( n <= 0 ) break;
		}

		closesocket( hConn );
	}

	pclsArg->m_bRun = false;

	return 0;
}

bool TestTcpSendBlocking( )
{
	Socket hListen = TcpListen( LISTEN_PORT, 255, "127.0.0.1" );
	if( hListen == INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "TcpListen() error" );
		return false;
	}

	CTcpClientThreadArg * pclsArg = new CTcpClientThreadArg();
	pclsArg->m_bRun = true;

	StartThread( "TcpClientThread", TcpClientThread, pclsArg );

	char szIp[52];
	int iPort;

	Socket hConn = TcpAccept( hListen, szIp, sizeof(szIp), &iPort );

	while( pclsArg->m_bRun )
	{
		sleep(1);
	}

	closesocket( hConn );

	delete pclsArg;

	return true;
}
