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
#include "TcpSocket.h"
#include "ServerUtility.h"

static bool gbRun = false;
static bool gbStop = false;

/**
 * @brief 모니터링 결과 패킷을 수신하는 쓰레드
 * @param lpParameter 
 * @returns 0 을 리턴한다.
 */
DWORD WINAPI MonitorThread( LPVOID lpParameter )
{
	gbRun = true;

	while( gbStop == false )
	{
		if( gclsSocket.Poll() > 0 )
		{
			if( gclsSocket.Receive() == false )
			{
				gclsSocket.Close();
				if( gclsSocket.Connect() == false )
				{
					break;
				}
			}
		}
	}

	gbRun = false;

	return 0;
}

/**
 * @brief 모니터링 결과 패킷을 수신하는 쓰레드를 시작한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool StartMonitorThread( )
{
	if( gbRun ) return false;

	gbStop = false;

	if( StartThread( "MonitorThread", MonitorThread, NULL ) == false ) return false;

	return true;
}

/**
 * @brief 모니터링 결과 패킷을 수신하는 쓰레드를 종료시킨다.
 */
void StopMonitorThread( )
{
	gbStop = true;
}

/**
 * @brief 모니터링 결과 패킷을 수신하는 쓰레드가 종료할 때까지 대기한다.
 */
void WaitStopMonitorThread( )
{
	while( gbRun )
	{
		Sleep(100);
	}
}
