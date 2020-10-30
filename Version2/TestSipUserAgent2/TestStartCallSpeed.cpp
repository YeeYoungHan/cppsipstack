#include "TestSipUserAgent.h"

void TestStartCallSpeed( int argc, char * argv[] )
{
	if( argc != 3 )
	{
		printf( "[Usage] %s speed {count}\n", argv[0] );
		return;
	}

	int iLoopCount = atoi( argv[2] );

	CSipUserAgent clsUserAgent;
	CSipStackSetup clsSetup;
	CSipServerInfo clsServerInfo;

	GetLocalIp( clsSetup.m_strLocalIp );

	clsServerInfo.m_strUserId = "1000";
	clsServerInfo.m_strPassWord = "1234";
	clsServerInfo.m_strIp = "192.168.150.10";

	clsUserAgent.InsertRegisterInfo( clsServerInfo );

	if( clsUserAgent.Start( clsSetup, NULL ) == false )
	{
		printf( "clsUserAgent.Start() error\n" );
		return;
	}

	CSipCallRtp clsRtp;
	CSipCallRoute clsRoute;
	std::string strCallId;

	clsRtp.m_iCodec = 0;
	clsRtp.m_strIp = clsSetup.m_strLocalIp;
	clsRtp.m_iPort = 6000;

	clsRoute.m_strDestIp = "192.168.0.1";
	clsRoute.m_iDestPort = 5060;

	DWORD dwStart = GetTickCount();

	for( int i = 0; i < iLoopCount; ++i )
	{
		if( clsUserAgent.StartCall( "1000", "1001", &clsRtp, &clsRoute, strCallId ) == false )
		{
			printf( "clsUserAgent.StartCall() error\n" );
			return;
		}
	}

	DWORD dwEnd = GetTickCount();

	printf( "%d ms\n", dwEnd - dwStart );
}