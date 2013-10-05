#include "stdafx.h"
#include "SipStackDefine.h"
#include "SipUserAgent.h"
#include "Setup.h"
#include "TimeUtility.h"

static bool gbStopTestThread = false;
static bool gbTestThreadRun = false;
static HWND ghWnd = NULL;
extern CSipUserAgent		 gclsSipUserAgent;

DWORD WINAPI SipTestThread( LPVOID lpParameter )
{
	CSipCallRtp clsRtp;
	CSipCallRoute clsRoute;
	std::string strCallId;
	struct timeval sttStart, sttEnd;

	gbTestThreadRun = true;

	clsRtp.m_strIp = gclsSipStack.m_clsSetup.m_strLocalIp;
	clsRtp.m_iPort = 4000;
	clsRtp.m_iCodec = 0;

	clsRoute.m_strDestIp = gclsSetup.m_strSipServerIp;
	clsRoute.m_iDestPort = gclsSetup.m_iSipServerPort;

	gettimeofday( &sttStart, NULL );

	for( int i = 0; i < gclsSetup.m_iCallTotalCount; ++i )
	{
		// ���� ��ȭ ������ ������ ������ ���ų� ū ��쿡�� ����Ѵ�.
		while( gclsSipUserAgent.GetCallCount() >= gclsSetup.m_iCallConcurrentCount )
		{
			Sleep(20);
			if( gbStopTestThread ) break;
		}

		if( gbStopTestThread ) break;

		if( gclsSipUserAgent.StartCall( gclsSetup.m_strCallerId.c_str(), gclsSetup.m_strCalleeId.c_str(), &clsRtp, &clsRoute, strCallId ) == false )
		{
			break;
		}
	}

	while( gclsSipUserAgent.GetCallCount() > 0 )
	{
		Sleep(20);
	}

	gettimeofday( &sttEnd, NULL );

	int iDiff = DiffTimeval( &sttStart, &sttEnd );

	gbTestThreadRun = false;

	SendMessage( ghWnd, SIP_TEST_ID, WM_TEST_END, iDiff );

	return 0;
}

bool StartTestThread( HWND hWnd )
{
	if( gbTestThreadRun ) return false;

	gbStopTestThread = false;
	ghWnd = hWnd;

	if( StartThread( "SipSpeedThread", SipTestThread, NULL ) == false ) return false;

	return true;
}

void StopTestThread()
{
	gbStopTestThread = true;
}

bool IsTestThreadRun()
{
	return gbTestThreadRun;
}
