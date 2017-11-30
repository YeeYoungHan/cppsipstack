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
#include "TcpSessionList.h"
#include "TlsFunction.h"
#include "TimeUtility.h"
#include "Log.h"
#include "MemoryDebug.h"

static void DeleteSession( CTcpSessionList * pclsSessionList, CTcpStack * pclsStack, int iIndex )
{
	pclsStack->m_clsClientMap.Delete( pclsSessionList->m_pclsSession[iIndex].m_strIp.c_str(), pclsSessionList->m_pclsSession[iIndex].m_iPort );
	pclsStack->m_pclsCallBack->SessionClosed( &pclsSessionList->m_pclsSession[iIndex] );
	pclsSessionList->Delete( iIndex );
}

static void BeforeDelete( CTcpSessionList * pclsSessionList, int iIndex, void * pclsArg )
{
	CTcpStack * pclsStack = (CTcpStack *)pclsArg;
	pclsStack->m_clsClientMap.Delete( pclsSessionList->m_pclsSession[iIndex].m_strIp.c_str(), pclsSessionList->m_pclsSession[iIndex].m_iPort );
	pclsStack->m_pclsCallBack->SessionClosed( &pclsSessionList->m_pclsSession[iIndex] );
}

/**
 * @ingroup TcpStack
 * @brief TCP 세션을 위한 쓰레드 함수
 * @param lpParameter CThreadListEntry 객체의 포인터
 * @returns 0 을 리턴한다.
 */
THREAD_API TcpPipeThread( LPVOID lpParameter )
{
	CTcpThreadInfo * pclsThreadInfo = (CTcpThreadInfo *)lpParameter;
	CTcpSessionList * pclsSessionList = &pclsThreadInfo->m_clsSessionList;
	CTcpStack * pclsStack = pclsThreadInfo->m_pclsStack;
	CTcpComm			clsTcpComm;
	int		n, iIndex;
	char	szPacket[TCP_INIT_BUF_SIZE];
	time_t	iTime, iDeleteTime;
	bool bAccept, bDeleteThreadInfo = false;

	CLog::Print( LOG_INFO, "TcpPipeThread started (index=%d)", pclsThreadInfo->m_iIndex );

	time( &iDeleteTime );

	while( 1 )
	{
		n = poll( pclsSessionList->m_psttPollFd, pclsSessionList->m_iPoolFdCount, 1000 );
		time( &iTime );
		if( n <= 0 ) goto LOOP_END;

		if( pclsSessionList->m_psttPollFd[0].revents & POLLIN )
		{
			// 새로운 TCP 세션을 자료구조에 추가한다.
			if( CTcpThreadList::RecvCommand( pclsSessionList->m_psttPollFd[0].fd, (char *)&clsTcpComm, sizeof(clsTcpComm) ) == sizeof(clsTcpComm) )
			{
				if( clsTcpComm.m_hSocket == INVALID_SOCKET )
				{
					// 종료 신호 수신
					bDeleteThreadInfo = true;
					break;
				}
				else
				{
					bAccept = true;

#ifdef USE_TLS
					if( pclsStack->m_clsSetup.m_bUseTls && clsTcpComm.m_bClient == false )
					{
						if( SSLAccept( clsTcpComm.m_hSocket, &clsTcpComm.m_psttSsl, false, 0, 10000 ) == false )
						{
							bAccept = false;
						}
					}
#endif

					if( bAccept )
					{
						iIndex = pclsSessionList->Insert( clsTcpComm );
						if( iIndex == -1 )
						{
							// 새로운 TCP 세션을 자료구조에 실패하면 TCP 세션을 종료하고 오류 처리한다.
							clsTcpComm.Close();
							if( clsTcpComm.m_bClient )
							{
								pclsStack->m_clsClientMap.Delete( clsTcpComm.m_szIp, clsTcpComm.m_iPort );
							}
						}
						else if( clsTcpComm.m_bClient )
						{
							// TCP 클라이언트로 서버에 연결된 경우, TCP 클라이언트 정보를 저장하고 버퍼링된 패킷들을 전송한다.
							pclsStack->m_clsClientMap.SetConnected( clsTcpComm.m_szIp, clsTcpComm.m_iPort, pclsThreadInfo->m_iIndex, iIndex );

							SEND_PACKET_LIST clsSendPacketList;
							SEND_PACKET_LIST::iterator itList;

							pclsStack->m_clsClientMap.DeleteSendPacketList( clsTcpComm.m_szIp, clsTcpComm.m_iPort, clsSendPacketList );
							for( itList = clsSendPacketList.begin(); itList != clsSendPacketList.end(); ++itList )
							{
								pclsSessionList->Send( iIndex, (*itList)->c_str(), (*itList)->length() );
								delete *itList;
							}
						}
						else
						{
							// TCP 서버로써 클라이언트가 연결되면 클라이언트 연결시 최초 이벤트 처리를 위한 callback 을 호출한다.
							if( pclsStack->m_pclsCallBack->InComingConnected( &pclsSessionList->m_pclsSession[iIndex] ) == false )
							{
								// 허용되지 않는 TCP 연결이면 TCP 연결 종료시킨다.
								CLog::Print( LOG_INFO, "not allowed TCP session(%s:%d)", clsTcpComm.m_szIp, clsTcpComm.m_iPort );
								DeleteSession( pclsSessionList, pclsStack, iIndex );
							}
							else
							{
								pclsStack->m_clsClientMap.Insert( clsTcpComm.m_szIp, clsTcpComm.m_iPort, pclsThreadInfo->m_iIndex, iIndex );
							}
						}
					}
				}
			}
			--n;
		}

		if( n == 0 ) goto LOOP_END;

		for( iIndex = 1; iIndex < pclsSessionList->m_iPoolFdCount; ++iIndex )
		{
			if( !(pclsSessionList->m_psttPollFd[iIndex].revents & POLLIN) ) continue;

#ifdef USE_TLS
			if( pclsSessionList->m_pclsSession[iIndex].m_psttSsl )
			{
				pclsSessionList->m_pclsSession[iIndex].m_clsMutex.acquire();
				n = SSLRecv( pclsSessionList->m_pclsSession[iIndex].m_psttSsl, szPacket, sizeof(szPacket) );
				pclsSessionList->m_pclsSession[iIndex].m_clsMutex.release();
			}
			else
#endif
			{
				n = recv( pclsSessionList->m_psttPollFd[iIndex].fd, szPacket, sizeof(szPacket), 0 );
			}

			if( n <= 0 )
			{
				DeleteSession( pclsSessionList, pclsStack, iIndex );
				continue;
			}

			if( CLog::GetLevel() & LOG_NETWORK )
			{
				pclsSessionList->m_pclsSession[iIndex].Log( szPacket, n, false );
			}

			pclsSessionList->m_pclsSession[iIndex].m_iRecvTime = iTime;

			if( pclsStack->m_pclsCallBack->RecvPacket( szPacket, n, &pclsSessionList->m_pclsSession[iIndex] ) == false )
			{
				DeleteSession( pclsSessionList, pclsStack, iIndex );
				continue;
			}
		}

LOOP_END:
		if( ( iDeleteTime + 60 ) < iTime )
		{
			pclsSessionList->DeleteTimeout( pclsStack->m_clsSetup.m_iTcpRecvTimeout, pclsStack->m_clsSetup.m_iTcpNoPacketTimeout
				, BeforeDelete, pclsStack );
			iDeleteTime = iTime;
		}
	}

	pclsSessionList->DeleteAll();

	CLog::Print( LOG_INFO, "TcpPipeThread terminated (index=%d)", pclsThreadInfo->m_iIndex );

	if( bDeleteThreadInfo )
	{
		pclsThreadInfo->Close();
		delete pclsThreadInfo;
	}

	return 0;
}
