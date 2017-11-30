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

#include "SipPlatformDefine.h"
#include "TcpStack.h"
#include "TcpThread.h"
#include "ServerUtility.h"
#include "Log.h"
#include "MemoryDebug.h"

static bool gbTcpListenThreadRun = false;

/**
 * @ingroup TcpStack
 * @brief TCP 서버 쓰레드
 * @param lpParameter CTcpStack 객체
 * @returns 0 을 리턴한다.
 */
THREAD_API TcpListenThread( LPVOID lpParameter )
{
	CTcpStack * pclsStack = (CTcpStack *)lpParameter;
	CTcpComm clsTcpComm;
	pollfd sttPoll[1];
	int		n, iSleepSecond = 0;

	CLog::Print( LOG_INFO, "TcpListenThread started(port=%d)", pclsStack->m_clsSetup.m_iListenPort );
	gbTcpListenThreadRun = true;

	TcpSetPollIn( sttPoll[0], pclsStack->m_hTcpListenFd );

	while( pclsStack->m_bStop == false )
	{
		n = poll( sttPoll, 1, 1000 );
		if( n <= 0 )
		{
			++iSleepSecond;

			if( iSleepSecond == 60 )
			{
				iSleepSecond = 0;

				if( pclsStack->m_clsSetup.m_bUseThreadPipe )
				{
					pclsStack->m_clsThreadList.DeleteNoUseThread();
				}
			}
			continue;
		}

		iSleepSecond = 0;

		clsTcpComm.m_hSocket = TcpAccept( pclsStack->m_hTcpListenFd, clsTcpComm.m_szIp, sizeof(clsTcpComm.m_szIp), &clsTcpComm.m_iPort );
		if( clsTcpComm.m_hSocket != INVALID_SOCKET )
		{
			if( pclsStack->m_clsSetup.m_bUseThreadPipe )
			{
				if( pclsStack->m_clsThreadList.SendCommand( (char *)&clsTcpComm, sizeof(clsTcpComm) ) == false )
				{
					CLog::Print( LOG_ERROR, "%s m_clsThreadList.SendCommand error", __FUNCTION__ );
					closesocket( clsTcpComm.m_hSocket );
				}
			}
			else
			{
				CTcpNoPipeThreadArg * pclsArg = new CTcpNoPipeThreadArg();
				if( pclsArg == NULL )
				{
					CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
					closesocket( clsTcpComm.m_hSocket );
				}
				else
				{
					pclsArg->m_strIp = clsTcpComm.m_szIp;
					pclsArg->m_iPort = clsTcpComm.m_iPort;
					pclsArg->m_hSocket = clsTcpComm.m_hSocket;
					pclsArg->m_pclsStack = pclsStack;

					if( StartThread( "TcpNoPipeThread", TcpNoPipeThread, pclsArg ) == false )
					{
						CLog::Print( LOG_ERROR, "%s StartThread error", __FUNCTION__ );
						closesocket( clsTcpComm.m_hSocket );
					}
				}
			}
		}
	}

	CLog::Print( LOG_INFO, "TcpListenThread terminated(port=%d)", pclsStack->m_clsSetup.m_iListenPort );
	gbTcpListenThreadRun = false;

	return 0;
}

/**
 * @ingroup TcpStack
 * @brief TCP listen 쓰레드가 실행중인가?
 * @returns TCP listen 쓰레드가 실행중이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsTcpListenThreadRun()
{
	return gbTcpListenThreadRun;
}
