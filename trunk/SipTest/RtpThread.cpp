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

static bool gbStopRtpThread = false;
static bool gbRtpThreadRun = false;

/**
 * @brief RTP 쓰레드
 * @param lpParameter 
 * @returns 0 을 리턴한다.
 */
DWORD WINAPI RtpThread( LPVOID lpParameter )
{

	return 0;
}

/**
 * @brief RTP 쓰레드를 시작한다.
 * @param hWnd 테스트 결과를 전달할 윈도우 핸들
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool StartRtpThread( )
{
	if( gbRtpThreadRun ) return false;

	gbStopRtpThread = false;

	if( StartThread( "RtpThread", RtpThread, NULL ) == false ) return false;

	return true;
}

/**
 * @brief RTP 쓰레드 실행 중지 요청한다.
 */
void StopRtpThread()
{
	gbStopRtpThread = true;
}

/**
 * @brief RTP 쓰레드가 실행되고 있는가?
 * @returns RTP 쓰레드가 실행되고 있으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsRtpThreadRun()
{
	return gbRtpThreadRun;
}
