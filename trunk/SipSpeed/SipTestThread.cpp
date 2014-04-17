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

static bool gbStopTestThread = false;
static bool gbTestThreadRun = false;
static HWND ghWnd = NULL;
extern CSipUserAgent		 gclsSipUserAgent;

#include "SipTestOneToOneCall.hpp"

/**
 * @ingroup SipSpeed
 * @brief 테스트 쓰레드
 * @param lpParameter 
 * @returns 0 을 리턴한다.
 */
DWORD WINAPI SipTestThread( LPVOID lpParameter )
{
	struct timeval sttStart, sttEnd;

	gbTestThreadRun = true;

	gettimeofday( &sttStart, NULL );

	SipTestOneToOneCall( );

	gettimeofday( &sttEnd, NULL );

	int iDiff = DiffTimeval( &sttStart, &sttEnd );

	gbTestThreadRun = false;

	SendMessage( ghWnd, SIP_TEST_ID, WM_TEST_END, iDiff );

	return 0;
}

/**
 * @ingroup SipSpeed
 * @brief 테스트 쓰레드를 시작한다.
 * @param hWnd 테스트 결과를 전달할 윈도우 핸들
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool StartTestThread( HWND hWnd )
{
	if( gbTestThreadRun ) return false;

	gbStopTestThread = false;
	ghWnd = hWnd;

	if( StartThread( "SipSpeedThread", SipTestThread, NULL ) == false ) return false;

	return true;
}

/**
 * @ingroup SipSpeed
 * @brief 테스트 쓰레드 실행 중지 요청한다.
 */
void StopTestThread()
{
	gbStopTestThread = true;
}

/**
 * @ingroup SipSpeed
 * @brief 테스트 쓰레드가 실행되고 있는가?
 * @returns 테스트 쓰레드가 실행되고 있으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsTestThreadRun()
{
	return gbTestThreadRun;
}
