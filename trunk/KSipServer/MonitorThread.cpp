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

static int giMonitorThreadCount = 0;
static CSipMutex gclsMutex;

class CMonitorSocket
{
public:
	Socket			hSocket;
	std::string	m_strIp;
	int					m_iPort;
};

/**
 * @brief KSipServer �� ���� �ڷᱸ���� ����͸��ϱ� ���� ������ �Լ�
 * @param lpParameter �ǹ̾���
 * @returns 0 �� �����Ѵ�.
 */
#ifdef WIN32
DWORD WINAPI MonitorThread( LPVOID lpParameter )
#else
void * MonitorThread( void * lpParameter )
#endif
{
	CMonitorSocket * pclsArg = ( CMonitorSocket * )lpParameter;
	char	szPacket[1024];
	int		n;

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
			continue;
		}

		n = TcpRecv( pclsArg->hSocket, szPacket, sizeof(szPacket), 1 );
		if( n <= 0 )
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
 * @brief ����͸� �����带 �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief ����͸� ������ ���� ������ �˻��Ѵ�.
 * @returns ����͸� �����尡 ���� ���̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool IsMonitorThreadRun()
{
	if( giMonitorThreadCount > 0 ) return true;

	return false;
}

