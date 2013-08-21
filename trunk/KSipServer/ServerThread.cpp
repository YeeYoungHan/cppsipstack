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

#include "KSipServer.h"
#include "SipTcp.h"
#include "SipStackThread.h"
#include "SipServerSetup.h"
#include "Log.h"
#include "ServerThread.h"
#include "ServerService.h"

/**
 * @ingroup KSipServer
 * @brief KSipServer 관리를 위한 네트워크 서버 쓰레드 함수
 * @param lpParameter 의미없음
 * @returns 0 을 리턴한다.
 */
#ifdef WIN32
DWORD WINAPI ServerThread( LPVOID lpParameter )
#else
void * ServerThread( void * lpParameter )
#endif
{
	Socket hMonitorSocket = INVALID_SOCKET;
	pollfd sttPoll[1];
	char	szIp[16];
	int		iPort, n;

	hMonitorSocket = TcpListen( gclsSetup.m_iMonitorPort, 255 );
	if( hMonitorSocket == INVALID_SOCKET )
	{
		CLog::Print( LOG_ERROR, "Monitor Tcp Server Port(%d) open error(%d)", gclsSetup.m_iMonitorPort, GetError() );
	}
	else
	{
		sttPoll[0].fd = hMonitorSocket;
		sttPoll[0].events = POLLIN;
		sttPoll[0].revents = 0;
	}

	CLog::Print( LOG_INFO, "ServerThread is started" );

	while( gbStop == false )
	{
		n = poll( sttPoll, 1, 1000 );
		if( n <= 0 )
		{
			continue;
		}

		Socket hClientSocket = TcpAccept( hMonitorSocket, szIp, sizeof(szIp), &iPort );
		if( hClientSocket == INVALID_SOCKET )
		{

		}
		else if( gclsSetup.IsMonitorIp( szIp ) )
		{
			StartMonitorThread( hClientSocket, szIp, iPort );
		}
		else
		{
			CLog::Print( LOG_ERROR, "monitor client(%s:%d) is disconnected because it's ip is not monitor ip", szIp, iPort );
			closesocket( hClientSocket );
		}
	}

	CLog::Print( LOG_INFO, "ServerThread is terminated" );

	return 0;
}

/**
 * @ingroup KSipServer
 * @brief 모니터링 서버 쓰레드를 시작한다.
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool StartServerThread( )
{
	if( StartThread( "ServerThread", ServerThread, NULL ) == false ) return false;

	return true;
}
