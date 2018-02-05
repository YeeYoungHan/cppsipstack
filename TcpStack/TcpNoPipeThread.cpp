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
#include "TlsFunction.h"
#include "TimeUtility.h"
#include "Log.h"
#include "MemoryDebug.h"

/**
 * @ingroup TcpStack
 * @brief pipe 를 사용하지 않는 TCP 세션 쓰레드 함수 - m_bUseThreadPipe 가 false 일 때에 사용된다.
 * @param lpParameter CThreadListEntry 객체의 포인터
 * @returns 0 을 리턴한다.
 */
THREAD_API TcpNoPipeThread( LPVOID lpParameter )
{
	CTcpNoPipeThreadArg * pclsArg = (CTcpNoPipeThreadArg *)lpParameter;
	CTcpStack * pclsStack = pclsArg->m_pclsStack;
	CTcpSessionInfo clsSessionInfo;
	int  iDiffTime, n;
	bool bAccept = true;
	char szPacket[TCP_INIT_BUF_SIZE];
	time_t iTime;

	CLog::Print( LOG_INFO, "TcpNoPipeThread started" );

	clsSessionInfo.m_hSocket = pclsArg->m_hSocket;
	clsSessionInfo.m_strIp = pclsArg->m_strIp;
	clsSessionInfo.m_iPort = pclsArg->m_iPort;
	time( &clsSessionInfo.m_iConnectTime );

#ifdef USE_TLS
	if( pclsStack->m_clsSetup.m_bUseTls && pclsArg->m_bClient == false )
	{
		if( SSLAccept( clsSessionInfo.m_hSocket, &clsSessionInfo.m_psttSsl, false, 0, 10000 ) == false )
		{
			bAccept = false;
		}
	}
#endif

	delete pclsArg;

	if( bAccept )
	{
		if( pclsStack->m_pclsCallBack->InComingConnected( &clsSessionInfo ) == false )
		{
			// 허용되지 않는 TCP 연결이면 TCP 연결 종료시킨다.
			CLog::Print( LOG_INFO, "not allowed TCP session(%s:%d)", clsSessionInfo.m_strIp.c_str(), clsSessionInfo.m_iPort );
		}
		else
		{
			if( pclsStack->m_clsSessionMap.Insert( clsSessionInfo.m_strIp.c_str(), clsSessionInfo.m_iPort, &clsSessionInfo ) == false )
			{
				CLog::Print( LOG_ERROR, "%s m_clsSessionMap.Insert(%s:%d) error", __FUNCTION__, clsSessionInfo.m_strIp.c_str(), clsSessionInfo.m_iPort );
			}
			else
			{
				CLog::Print( LOG_INFO, "TcpNoPipeThread (index=%d)", clsSessionInfo.m_iThreadIndex );

				pollfd sttPoll[1];

				TcpSetPollIn( sttPoll[0], clsSessionInfo.m_hSocket );

				while( pclsStack->m_bStop == false )
				{
					n = poll( sttPoll, 1, 1000 );
					if( n <= 0 )
					{
						time( &iTime );

						if( clsSessionInfo.m_iRecvTime != 0 )
						{
							iDiffTime = (int)(iTime - clsSessionInfo.m_iRecvTime);

							if( iDiffTime >= pclsStack->m_clsSetup.m_iTcpRecvTimeout )
							{
								CLog::Print( LOG_ERROR, "%s client(%s:%d) recv timeout", __FUNCTION__, clsSessionInfo.m_strIp.c_str(), clsSessionInfo.m_iPort );
								break;
							}
						}
						else
						{
							iDiffTime = (int)(iTime - clsSessionInfo.m_iConnectTime);

							if( iDiffTime >= pclsStack->m_clsSetup.m_iTcpNoPacketTimeout )
							{
								CLog::Print( LOG_ERROR, "%s client(%s:%d) no packet timeout", __FUNCTION__, clsSessionInfo.m_strIp.c_str(), clsSessionInfo.m_iPort );
								break;
							}
						}

						continue;
					}

#ifdef USE_TLS
					if( clsSessionInfo.m_psttSsl )
					{
						clsSessionInfo.m_clsMutex.acquire();
						n = SSLRecv( clsSessionInfo.m_psttSsl, szPacket, sizeof(szPacket) );
						clsSessionInfo.m_clsMutex.release();
					}
					else
#endif
					{
						n = recv( clsSessionInfo.m_hSocket, szPacket, sizeof(szPacket), 0 );
					}

					if( n <= 0 )
					{
						break;
					}

					if( CLog::GetLevel() & LOG_NETWORK )
					{
						clsSessionInfo.Log( szPacket, n, false );
					}

					time( &clsSessionInfo.m_iRecvTime );

					if( pclsStack->m_pclsCallBack->RecvPacket( szPacket, n, &clsSessionInfo ) == false )
					{
						break;
					}
				}

				pclsStack->m_clsSessionMap.Delete( clsSessionInfo.m_strIp.c_str(), clsSessionInfo.m_iPort );
			}
		}
	}

	closesocket( clsSessionInfo.m_hSocket );

	pclsStack->m_pclsCallBack->SessionClosed( &clsSessionInfo );
	clsSessionInfo.Clear();

	CLog::Print( LOG_INFO, "TcpNoPipeThread terminated (index=%d)", clsSessionInfo.m_iThreadIndex );

#ifdef USE_TLS
	if( pclsStack->m_clsSetup.m_bUseTls )
	{
		ERR_remove_thread_state( NULL );
	}
#endif

	return 0;
}
