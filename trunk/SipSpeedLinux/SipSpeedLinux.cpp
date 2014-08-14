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

#include "SipSpeedLinux.h"
#include "SipSpeedSetup.h"
#include "SipClient.h"
#include "TimeUtility.h"

CSipUserAgent		 gclsSipUserAgent;

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {setup filename}\n", argv[0] );
		return 0;
	}

	const char * pszSetupFileName = argv[1];

	if( gclsSetup.Read( pszSetupFileName ) == false ) return 0;

	// SipStack 을 시작한다.
	CSipStackSetup clsSetup;
	CSipServerInfo clsInfo;
	CSipClient clsCallBack;

	GetLocalIp( clsSetup.m_strLocalIp );

	clsInfo.m_strIp = gclsSetup.m_strSipServerIp;
	clsInfo.m_iPort = gclsSetup.m_iSipServerPort;
	clsInfo.m_strDomain = gclsSetup.m_strSipDomain;
	clsInfo.m_strUserId = gclsSetup.m_strCallerId;
	clsInfo.m_strPassWord = gclsSetup.m_strCallerPassWord;

	gclsSipUserAgent.InsertRegisterInfo( clsInfo );

	clsInfo.m_strUserId = gclsSetup.m_strCalleeId;
	clsInfo.m_strPassWord = gclsSetup.m_strCalleePassWord;

	gclsSipUserAgent.InsertRegisterInfo( clsInfo );

	bool bSuccess = false;

	clsSetup.m_iUdpThreadCount = 4;
	clsSetup.m_iStackExecutePeriod = 100;
	clsSetup.m_iTimerD = 4000;
	clsSetup.m_iTimerJ = 4000;

	for( int i = 0; i < 100; ++i )
	{
		clsSetup.m_iLocalUdpPort = i + 10000;

		if( gclsSipUserAgent.Start( clsSetup, &clsCallBack ) )
		{
			bSuccess = true;
			break;
		}
	}

	if( bSuccess == false )
	{
		printf( "gclsSipUserAgent.Start() error\n" );
		return 0;
	}

	while( clsCallBack.m_bCallerLogin == false || clsCallBack.m_bCalleeLogin == false )
	{
		MiliSleep(20);
	}

	struct timeval sttStart, sttEnd;

	gettimeofday( &sttStart, NULL );

	CSipCallRtp clsRtp;
	CSipCallRoute clsRoute;
	std::string strCallId;

	clsRtp.m_strIp = gclsSipUserAgent.m_clsSipStack.m_clsSetup.m_strLocalIp;
	clsRtp.m_iPort = 4000;
	clsRtp.m_iCodec = 0;

	clsRoute.m_strDestIp = gclsSetup.m_strSipServerIp;
	clsRoute.m_iDestPort = gclsSetup.m_iSipServerPort;

	// 통화 연결 테스트
	for( int i = 0; i < gclsSetup.m_iCallTotalCount; ++i )
	{
		// 동시 통화 개수가 설정된 개수와 같거나 큰 경우에는 대기한다.
		while( gclsSipUserAgent.GetCallCount() >= gclsSetup.m_iCallConcurrentCount )
		{
			MiliSleep(20);
		}

		if( gclsSipUserAgent.StartCall( gclsSetup.m_strCallerId.c_str(), gclsSetup.m_strCalleeId.c_str(), &clsRtp, &clsRoute, strCallId ) == false )
		{
			break;
		}
	}

	// 모든 통화가 종료될 때까지 대기한다.
	while( gclsSipUserAgent.GetCallCount() > 0 )
	{
		MiliSleep(20);
	}

	gettimeofday( &sttEnd, NULL );

	int iDiff = DiffTimeval( &sttStart, &sttEnd );
	double fCps = 0.0;

	if( iDiff > 0 )
	{
		fCps = (double)( clsCallBack.m_iCallSuccess + clsCallBack.m_iCallError ) / ( (double)iDiff / 1000 );
	}

	printf( "call success(%d) / error(%d) / cps(%.3f) / time(%d ms)", clsCallBack.m_iCallSuccess, clsCallBack.m_iCallError, fCps, iDiff );

	gclsSipUserAgent.Stop();

	return 0;
}
