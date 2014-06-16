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

#define CALL_ROUTE_MILI_SECOND	500

static bool gbStopTestThread = false;
static bool gbTestThreadRun = false;
static HWND ghWnd = NULL;

/**
 * @ingroup SipTest
 * @brief �α׸� ����Ѵ�.
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
 * @brief ��� ��ȭ�� ����� ������ ����Ѵ�.
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
 * @brief ��ȭ ���� ��, ��ȭ ������ ������ ����Ѵ�.
 * @param pszTestName �׽�Ʈ �̸�
 * @param clsRtp			RTP ����
 * @param clsRoute		SIP ���� ����
 * @returns �����ϸ� ��ȭ ���� ��, ����Ǳ������ �ð��� mili second ������ �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
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
 * @brief �׽�Ʈ ������
 * @param lpParameter 
 * @returns 0 �� �����Ѵ�.
 */
THREAD_API SipTestThread( LPVOID lpParameter )
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

#ifdef USE_MEDIA_LIST
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
#endif

	clsRoute.m_strDestIp = gclsSetup.m_strSipServerIp;
	clsRoute.m_iDestPort = gclsSetup.m_iSipServerPort;

	// ��ȭ ���� �׽�Ʈ
	gclsTestInfo.m_eTestType = E_TEST_ESTABLISHED;
	iMiliSecond = StartCall( "Call Established Test", clsRtp, clsRoute );
	if( iMiliSecond == -1 ) goto FUNC_END;

	if( gclsTestInfo.m_bResult == false )
	{
		SendLog( "Call Established Test : ERROR" );
		goto FUNC_END;
	}

	SendLog( "Call Established Test : OK" );

	// ��ȭ ��� �׽�Ʈ
	gclsTestInfo.m_eTestType = E_TEST_CANCEL;
	iMiliSecond = StartCall( "Call Cancel Test", clsRtp, clsRoute );
	if( iMiliSecond == -1 ) goto FUNC_END;
	if( iMiliSecond <= CALL_ROUTE_MILI_SECOND )
	{
		SendLog( "Call Cancel Test : OK" );
	}
	else
	{
		SendLog( "Call Cancel Test : ERROR" );
	}

	// ��ȭ ���� �׽�Ʈ
	gclsTestInfo.m_eTestType = E_TEST_DECLINE;
	iMiliSecond = StartCall( "Call Decline Test", clsRtp, clsRoute );
	if( iMiliSecond == -1 ) goto FUNC_END;
	if( iMiliSecond <= CALL_ROUTE_MILI_SECOND )
	{
		SendLog( "Call Decline Test : OK" );
	}
	else
	{
		SendLog( "Call Decline Test : ERROR" );
	}

	if( gclsSetup.m_strCalleeId2.empty() == false )
	{
		// ��ȭ ���� �׽�Ʈ
		SendLog( "Call Blind Transfer Test : Start" );

		gclsTestInfo.m_eTestType = E_TEST_TRANSFER;
		gclsTestInfo.m_bRtpThreadEnd = false;
		if( gclsSipUserAgent.StartCall( gclsSetup.m_strCallerId.c_str(), gclsSetup.m_strCalleeId.c_str(), &clsRtp, &clsRoute, strCallId ) == false )
		{
			SendLog( "gclsSipUserAgent.StartCall error" );
			return -1;
		}

		gclsTestInfo.m_strCallerCallId = strCallId;
		
		while( gclsTestInfo.m_bRtpThreadEnd == false )
		{
			if( gbStopTestThread ) break;
			Sleep(20);
		}

		if( gclsTestInfo.m_bResult == false )
		{
			SendLog( "Call Blind Transfer Test : ERROR" );
			goto FUNC_END;
		}

		SendLog( "Call Blind Transfer Test : Call success => Refer test" );

		gclsTestInfo.m_eTestType = E_TEST_TRANSFER_CALL;
		gclsTestInfo.m_eTransferResult = E_TR_NULL;
		gclsSipUserAgent.TransferCallBlind( gclsTestInfo.m_strCallerCallId.c_str(), gclsSetup.m_strCalleeId2.c_str() );

		while( gclsTestInfo.m_eTransferResult == E_TR_NULL )
		{
			if( gbStopTestThread ) break;
			Sleep(20);
		}

		if( gclsTestInfo.m_eTransferResult == E_TR_ERROR )
		{
			SendLog( "Call Blind Transfer Test : ERROR" );
			goto FUNC_END;
		}

		WaitUntilAllCallStop();

		SendLog( "Call Blind Transfer Test : OK" );

		// Screened Transfer Test
		SendLog( "Call Screened Transfer Test : Start" );

		std::string strFirstCallId, strSecondCall;

		gclsTestInfo.m_eTestType = E_TEST_TRANSFER;
		gclsTestInfo.m_bRtpThreadEnd = false;
		if( gclsSipUserAgent.StartCall( gclsSetup.m_strCallerId.c_str(), gclsSetup.m_strCalleeId.c_str(), &clsRtp, &clsRoute, strCallId ) == false )
		{
			SendLog( "gclsSipUserAgent.StartCall error" );
			return -1;
		}

		strFirstCallId = strCallId;
		gclsTestInfo.m_strCallerCallId = strCallId;
		
		while( gclsTestInfo.m_bRtpThreadEnd == false )
		{
			if( gbStopTestThread ) break;
			Sleep(20);
		}

		if( gclsTestInfo.m_bResult == false )
		{
			SendLog( "Call Screened Transfer Test : ERROR" );
			goto FUNC_END;
		}

		SendLog( "Call Screened Transfer Test : second call start" );

		gclsTestInfo.m_eTestType = E_TEST_TRANSFER_CALL;
		gclsTestInfo.m_bRtpThreadEnd = false;
		if( gclsSipUserAgent.StartCall( gclsSetup.m_strCallerId.c_str(), gclsSetup.m_strCalleeId2.c_str(), &clsRtp, &clsRoute, strSecondCall ) == false )
		{
			gclsSipUserAgent.StopCall( gclsTestInfo.m_strCallerCallId.c_str() );
			SendLog( "gclsSipUserAgent.StartCall error" );
			return -1;
		}

		while( gclsTestInfo.m_bRtpThreadEnd == false )
		{
			if( gbStopTestThread ) break;
			Sleep(20);
		}

		gclsTestInfo.m_eTransferResult = E_TR_NULL;
		gclsTestInfo.m_eTestType = E_TEST_SCREENED_TRANSFER_CALL;

		SendLog( "Call Screened Transfer Test : send refer" );

		if( gclsSipUserAgent.TransferCall( strFirstCallId.c_str(), strSecondCall.c_str() ) == false )
		{
			SendLog( "gclsSipUserAgent.TransferCall error" );
			return -1;
		}

		WaitUntilAllCallStop();

		SendLog( "Call Screened Transfer Test : OK" );
	}

FUNC_END:
	gclsSipUserAgent.StopCallAll();
	gclsTestInfo.CloseRtp();
	gbTestThreadRun = false;

	SendMessage( ghWnd, SIP_TEST_ID, WM_TEST_END, 0 );

	return 0;
}

/**
 * @ingroup SipTest
 * @brief �׽�Ʈ �����带 �����Ѵ�.
 * @param hWnd �׽�Ʈ ����� ������ ������ �ڵ�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief �׽�Ʈ ������ ���� ���� ��û�Ѵ�.
 */
void StopTestThread()
{
	gbStopTestThread = true;
}

/**
 * @ingroup SipTest
 * @brief �׽�Ʈ �����尡 ����ǰ� �ִ°�?
 * @returns �׽�Ʈ �����尡 ����ǰ� ������ true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool IsTestThreadRun()
{
	return gbTestThreadRun;
}
