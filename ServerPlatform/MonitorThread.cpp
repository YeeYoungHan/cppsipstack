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

#include "SipTcp.h"
#include "SipStackThread.h"
#include "Log.h"
#include "MonitorCallBack.h"
#include "ServerService.h"
#include "ServerUtility.h"
#include "MemoryDebug.h"

static int giMonitorThreadCount = 0;
CSipMutex gclsCountMutex;

/**
 * @ingroup ServerPlatform
 * @brief 서버 모니터링 쓰레드를 위한 변수
 */
class CMonitorSocket
{
public:
	Socket			hSocket;
	std::string	m_strIp;
	int					m_iPort;
	IMonitorCallBack * m_pclsCallBack;
};

/**
 * @ingroup ServerPlatform
 * @brief 모니터링 클라이언트에서 수신한 명령을 처리한다.
 * @param pclsArg		서버 모니터링 쓰레드 변수
 * @param pszPacket 수신된 패킷
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
static bool MonitorCommand( CMonitorSocket * pclsArg, const char * pszPacket )
{
	std::string	strBuf;
	int	iPacketLen, n;

	if( pclsArg->m_pclsCallBack->RecvRequest( pszPacket, strBuf ) == false )
	{
		CLog::Print( LOG_DEBUG, "MonitorThread(%s:%d) command(%s) is not defined", pclsArg->m_strIp.c_str(), pclsArg->m_iPort, pszPacket );
		return false;
	}

	iPacketLen = htonl( (int)strBuf.length() );

	n = TcpSend( pclsArg->hSocket, (char *)&iPacketLen, sizeof(iPacketLen) );
	if( n != sizeof(iPacketLen) )
	{
		CLog::Print( LOG_DEBUG, "MonitorThread(%s:%d) send header error(%d)", pclsArg->m_strIp.c_str(), pclsArg->m_iPort, GetError() );
		return false;
	}

	n = TcpSend( pclsArg->hSocket, strBuf.c_str(), (int)strBuf.length() );
	if( n != (int)strBuf.length() )
	{
		CLog::Print( LOG_DEBUG, "MonitorThread(%s:%d) send body error(%d)", pclsArg->m_strIp.c_str(), pclsArg->m_iPort, GetError() );
		return false;
	}

	return true;
}

/**
 * @ingroup ServerPlatform
 * @brief SipLoadBalancer 의 내부 자료구조를 모니터링하기 위한 쓰레드 함수
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

	gclsCountMutex.acquire();
	++giMonitorThreadCount;
	gclsCountMutex.release();

	CLog::Print( LOG_INFO, "MonitorThread(%s:%d) is started", pclsArg->m_strIp.c_str(), pclsArg->m_iPort );

	while( gbStop == false )
	{
		n = poll( sttPoll, 1, 1000 );
		if( n < 0 )
		{
			CLog::Print( LOG_ERROR, "MonitorThread(%s:%d) poll error(%d)", pclsArg->m_strIp.c_str(), pclsArg->m_iPort, GetError() );
			break;
		}
		else if( n == 0 )
		{
			++iNoCommandSecond;
			if( iNoCommandSecond == 600 ) 
			{
				CLog::Print( LOG_DEBUG, "MonitorThread(%s:%d) no command received in 600 second", pclsArg->m_strIp.c_str(), pclsArg->m_iPort );
				break;
			}

			continue;
		}

		iNoCommandSecond = 0;

		// 명령 길이를 수신한다.
		n = recv( pclsArg->hSocket, (char *)&iPacketLen, sizeof(iPacketLen), 0 );
		if( n <= 0 )
		{
			CLog::Print( LOG_DEBUG, "MonitorThread(%s:%d) recv header(%d)", pclsArg->m_strIp.c_str(), pclsArg->m_iPort, n );
			break;
		}

		iPacketLen = ntohl( iPacketLen );
		if( iPacketLen <= 0 || iPacketLen >= (int)sizeof(szPacket) )
		{
			CLog::Print( LOG_DEBUG, "MonitorThread(%s:%d) recv command length(%d)", pclsArg->m_strIp.c_str(), pclsArg->m_iPort, iPacketLen );
			break;
		}

		// 명령을 수신한다.
		n = TcpRecv( pclsArg->hSocket, szPacket, iPacketLen, 5 );
		if( n <= 0 )
		{
			CLog::Print( LOG_DEBUG, "MonitorThread(%s:%d) recv command body(%d)", pclsArg->m_strIp.c_str(), pclsArg->m_iPort, n );
			break;
		}

		szPacket[n] = '\0';
		if( MonitorCommand( pclsArg, szPacket ) == false )
		{
			break;
		}
	}

	closesocket( pclsArg->hSocket );

	CLog::Print( LOG_INFO, "MonitorThread(%s:%d) is terminated", pclsArg->m_strIp.c_str(), pclsArg->m_iPort );
	delete pclsArg;

	gclsCountMutex.acquire();
	--giMonitorThreadCount;
	gclsCountMutex.release();

	return 0;
}

/**
 * @ingroup ServerPlatform
 * @brief 모니터링 쓰레드를 시작한다.
 * @param hSocket	연결된 TCP 소켓
 * @param pszIp		클라이언트 IP 주소
 * @param iPort		클라이언트 포트 번호
 * @param pclsCallBack	모니터링 callback 인터페이스
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool StartMonitorThread( Socket hSocket, const char * pszIp, int iPort, IMonitorCallBack * pclsCallBack )
{
	CMonitorSocket * pclsArg = new CMonitorSocket();
	if( pclsArg == NULL ) return false;

	pclsArg->hSocket = hSocket;
	pclsArg->m_strIp = pszIp;
	pclsArg->m_iPort = iPort;
	pclsArg->m_pclsCallBack = pclsCallBack;

	if( StartThread( "MonitorThread", MonitorThread, pclsArg ) == false ) return false;

	return true;
}

/**
 * @ingroup ServerPlatform
 * @brief 모니터링 쓰레드 실행 유무를 검사한다.
 * @returns 모니터링 쓰레드가 실행 중이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsMonitorThreadRun()
{
	if( giMonitorThreadCount > 0 ) return true;

	return false;
}

