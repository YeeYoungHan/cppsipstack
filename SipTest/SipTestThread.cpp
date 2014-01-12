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

#include "stdafx.h"
#include "SipStackDefine.h"
#include "SipUserAgent.h"
#include "Setup.h"
#include "TimeUtility.h"
#include "ServerUtility.h"
#include "TestInfo.h"

static bool gbStopTestThread = false;
static bool gbTestThreadRun = false;
static HWND ghWnd = NULL;

/**
 * @ingroup SipTest
 * @brief 로그를 출력한다.
 */
void SendLog( const char * fmt, ... )
{
	va_list		ap;
	char			szBuf[8192];

	va_start( ap, fmt );
	vsnprintf( szBuf, sizeof(szBuf)-1, fmt, ap );
	va_end( ap );

	SendMessage( ghWnd, SIP_TEST_ID, WM_TEST_MSG, (LPARAM)szBuf );
}

/**
 * @ingroup SipTest
 * @brief 모든 통화가 종료될 때까지 대기한다.
 */
int WaitUntilAllCallStop()
{
	int iMiliSecond = 0;

	while( gclsSipUserAgent.GetCallCount() > 0 )
	{
		Sleep(20);
		iMiliSecond += 20;

		if( gbStopTestThread )
		{
			gclsSipUserAgent.StopCallAll();
		}
	}

	return iMiliSecond;
}

/**
 * @ingroup SipTest
 * @brief 통화 연결 후, 통화 종료할 때까지 대기한다.
 * @param pszTestName 테스트 이름
 * @param clsRtp			RTP 정보
 * @param clsRoute		SIP 서버 정보
 * @returns 성공하면 통화 연결 후, 종료되기까지의 시간을 mili second 단위로 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int StartCall( const char * pszTestName, CSipCallRtp & clsRtp, CSipCallRoute & clsRoute )
{
	std::string	strCallId;

	SendLog( "%s : Start", pszTestName );

	if( gclsSipUserAgent.StartCall( gclsSetup.m_strCallerId.c_str(), gclsSetup.m_strCalleeId.c_str(), &clsRtp, &clsRoute, strCallId ) == false )
	{
		SendLog( "gclsSipUserAgent.StartCall error" );
		return -1;
	}

	gclsTestInfo.m_strCallerCallId = strCallId;

	int iMiliSecond = WaitUntilAllCallStop();

	SendLog( "%s : Stop", pszTestName );

	return iMiliSecond;
}

/**
 * @ingroup SipTest
 * @brief 테스트 쓰레드
 * @param lpParameter 
 * @returns 0 을 리턴한다.
 */
DWORD WINAPI SipTestThread( LPVOID lpParameter )
{
	CSipCallRtp clsRtp;
	CSipCallRoute clsRoute;
	std::string strCallId;
	int iMiliSecond;

	gbTestThreadRun = true;

	if( gclsTestInfo.CreateRtp() == false ) 
	{
		SendLog( "gclsTestInfo.CreateRtp error" );
		goto FUNC_END;
	}

	clsRtp.m_strIp = gclsSipUserAgent.m_clsSipStack.m_clsSetup.m_strLocalIp;
	clsRtp.m_iPort = gclsTestInfo.m_clsCallerRtp.m_iPort;
	clsRtp.m_iCodec = 0;

	if( gclsSetup.m_bUseTwoMedia )
	{
		CSdpMedia sdpAudio( "audio", gclsTestInfo.m_clsCallerRtp.m_iPort, "RTP/AVP" );
		CSdpMedia sdpVideo( "video", gclsTestInfo.m_clsCallerVideoRtp.m_iPort, "RTP/AVP" );

		sdpAudio.AddAttribute( "rtpmap", "0 PCMU/8000" );
		
		sdpVideo.AddAttribute( "rtpmap", "97 H264/90000" );
		sdpVideo.AddAttribute( "fmtp", "97 profile-level-id=42" );

		clsRtp.m_clsMediaList.push_back( sdpAudio );
		clsRtp.m_clsMediaList.push_back( sdpVideo );
	}

	clsRoute.m_strDestIp = gclsSetup.m_strSipServerIp;
	clsRoute.m_iDestPort = gclsSetup.m_iSipServerPort;

	// 통화 연결 테스트
	gclsTestInfo.m_eTestType = E_TEST_ESTABLISHED;
	iMiliSecond = StartCall( "Call Established Test", clsRtp, clsRoute );
	if( iMiliSecond == -1 ) goto FUNC_END;

	if( gclsTestInfo.m_bResult == false )
	{
		SendLog( "Call Established Test : ERROR" );
		goto FUNC_END;
	}

	SendLog( "Call Established Test : OK" );

	// 통화 취소 테스트
	gclsTestInfo.m_eTestType = E_TEST_CANCEL;
	iMiliSecond = StartCall( "Call Cancel Test", clsRtp, clsRoute );
	if( iMiliSecond == -1 ) goto FUNC_END;
	if( iMiliSecond <= 200 )
	{
		SendLog( "Call Cancel Test : OK" );
	}
	else
	{
		SendLog( "Call Cancel Test : ERROR" );
	}

	// 통화 거절 테스트
	gclsTestInfo.m_eTestType = E_TEST_DECLINE;
	iMiliSecond = StartCall( "Call Decline Test", clsRtp, clsRoute );
	if( iMiliSecond == -1 ) goto FUNC_END;
	if( iMiliSecond <= 200 )
	{
		SendLog( "Call Decline Test : OK" );
	}
	else
	{
		SendLog( "Call Decline Test : ERROR" );
	}

FUNC_END:
	gclsTestInfo.CloseRtp();
	gbTestThreadRun = false;

	SendMessage( ghWnd, SIP_TEST_ID, WM_TEST_END, 0 );

	return 0;
}

/**
 * @ingroup SipTest
 * @brief 테스트 쓰레드를 시작한다.
 * @param hWnd 테스트 결과를 전달할 윈도우 핸들
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool StartTestThread( HWND hWnd )
{
	if( gbTestThreadRun ) return false;

	gbStopTestThread = false;
	ghWnd = hWnd;

	if( StartThread( "SipTestThread", SipTestThread, NULL ) == false ) return false;

	return true;
}

/**
 * @ingroup SipTest
 * @brief 테스트 쓰레드 실행 중지 요청한다.
 */
void StopTestThread()
{
	gbStopTestThread = true;
}

/**
 * @ingroup SipTest
 * @brief 테스트 쓰레드가 실행되고 있는가?
 * @returns 테스트 쓰레드가 실행되고 있으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsTestThreadRun()
{
	return gbTestThreadRun;
}
