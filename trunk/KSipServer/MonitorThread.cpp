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
#include "Log.h"
#include "MonitorDefine.h"
#include "SipUserAgent.h"
#include "CallMap.h"

static int giMonitorThreadCount = 0;
static CSipMutex gclsMutex;

class CMonitorSocket
{
public:
	Socket			hSocket;
	std::string	m_strIp;
	int					m_iPort;
};

static bool MonitorCommand( CMonitorSocket * pclsArg, const char * pszPacket )
{
	std::string	strBuf;
	int	iPacketLen, n;

	if( !strcmp( pszPacket, MC_CALL_MAP_LIST ) )
	{
		
	}
	else if( !strcmp( pszPacket, MC_SIP_STACK_COUNT_LIST ) )
	{
		gclsSipStack.GetString( strBuf );
	}
	else
	{
		return false;
	}

	iPacketLen = htonl( strBuf.length() );

	n = TcpSend( pclsArg->hSocket, (char *)&iPacketLen, sizeof(iPacketLen) );
	if( n != sizeof(iPacketLen) )
	{
		return false;
	}

	n = TcpSend( pclsArg->hSocket, strBuf.c_str(), strBuf.length() );
	if( n != strBuf.length() )
	{
		return false;
	}

	return true;
}

/**
 * @brief KSipServer 의 내부 자료구조를 모니터링하기 위한 쓰레드 함수
 * @param lpParameter 의미없음
 * @returns 0 을 리턴한다.
 */
#ifdef WIN32
DWORD WINAPI MonitorThread( LPVOID lpParameter )
#else
void * MonitorThread( void * lpParameter )
#endif
{
	CMonitorSocket * pclsArg = ( CMonitorSocket * )lpParameter;
	char	szPacket[1024];
	int		iPacketLen, n, iNoCommandSecond = 0;

	pollfd sttPoll[1];

	sttPoll[0].fd = pclsArg->hSocket;
	sttPoll[0].events = POLLIN;
	sttPoll[0].revents = 0;

	gclsMutex.acquire();
	++giMonitorThreadCount;
	gclsMutex.release();

	CLog::Print( LOG_INFO, "MonitorThread is started" );

	while( gbStop == false )
	{
		n = poll( sttPoll, 1, 1000 );
		if( n < 0 )
		{
			break;
		}
		else if( n == 0 )
		{
			++iNoCommandSecond;
			if( iNoCommandSecond == 600 ) break;
			continue;
		}

		iNoCommandSecond = 0;

		// 명령 길이를 수신한다.
		n = recv( pclsArg->hSocket, (char *)&iPacketLen, sizeof(iPacketLen), 0 );
		if( n <= 0 )
		{
			break;
		}

		iPacketLen = ntohl( iPacketLen );
		if( iPacketLen <= 0 || iPacketLen >= sizeof(szPacket) )
		{
			break;
		}

		// 명령을 수신한다.
		n = TcpRecv( pclsArg->hSocket, szPacket, sizeof(szPacket), 1 );
		if( n <= 0 )
		{
			break;
		}

		szPacket[n] = '\0';
		if( MonitorCommand( pclsArg, szPacket ) == false )
		{
			break;
		}
	}

	closesocket( pclsArg->hSocket );

	CLog::Print( LOG_INFO, "MonitorThread is terminated" );

	gclsMutex.acquire();
	--giMonitorThreadCount;
	gclsMutex.release();

	return 0;
}

/**
 * @brief 모니터링 쓰레드를 시작한다.
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool StartMonitorThread( Socket hSocket, const char * pszIp, int iPort )
{
	CMonitorSocket * pclsArg = new CMonitorSocket();
	if( pclsArg == NULL ) return false;

	pclsArg->hSocket = hSocket;
	pclsArg->m_strIp = pszIp;
	pclsArg->m_iPort = iPort;

	if( StartThread( "MonitorThread", MonitorThread, pclsArg ) == false ) return false;

	return true;
}

/**
 * @brief 모니터링 쓰레드 실행 유무를 검사한다.
 * @returns 모니터링 쓰레드가 실행 중이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsMonitorThreadRun()
{
	if( giMonitorThreadCount > 0 ) return true;

	return false;
}

