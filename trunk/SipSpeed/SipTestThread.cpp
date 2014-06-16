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
#include "SipUtility.h"
#include "Setup.h"
#include "TimeUtility.h"
#include "ServerUtility.h"
#include "CallIdMap.h"

static bool gbStopTestThread = false;
static bool gbTestThreadRun = false;
static HWND ghWnd = NULL;
extern CSipUserAgent		 gclsSipUserAgent;

#include "SipTestOneToOneCall.hpp"
#include "SipTestSipOptions.hpp"

/**
 * @ingroup SipSpeed
 * @brief �׽�Ʈ ������
 * @param lpParameter 
 * @returns 0 �� �����Ѵ�.
 */
THREAD_API SipTestThread( LPVOID lpParameter )
{
	struct timeval sttStart, sttEnd;

	gbTestThreadRun = true;

	gettimeofday( &sttStart, NULL );

	switch( gclsSetup.m_iTestType )
	{
	case 0:
		SipTestOneToOneCall( );
		break;
	case 1:
		SipTestSipOptions( );
		break;
	}

	gettimeofday( &sttEnd, NULL );

	int iDiff = DiffTimeval( &sttStart, &sttEnd );

	gbTestThreadRun = false;

	SendMessage( ghWnd, SIP_TEST_ID, WM_TEST_END, iDiff );

	return 0;
}

/**
 * @ingroup SipSpeed
 * @brief �׽�Ʈ �����带 �����Ѵ�.
 * @param hWnd �׽�Ʈ ����� ������ ������ �ڵ�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief �׽�Ʈ ������ ���� ���� ��û�Ѵ�.
 */
void StopTestThread()
{
	gbStopTestThread = true;
}

/**
 * @ingroup SipSpeed
 * @brief �׽�Ʈ �����尡 ����ǰ� �ִ°�?
 * @returns �׽�Ʈ �����尡 ����ǰ� ������ true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool IsTestThreadRun()
{
	return gbTestThreadRun;
}
