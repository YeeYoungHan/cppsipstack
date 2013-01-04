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

#include "SipStackThread.h"
#include "TcpSessionList.h"
#include "Log.h"
#include <time.h>

/**
 * @ingroup SipStack
 * @brief SIP 메시지를 파싱하여서 SIP stack 에 입력한다.
 * @param pclsSipStack SIP stack
 * @param iThreadId		UDP 쓰레드 번호
 * @param pszBuf			네트워크에서 수신된 SIP 메시지
 * @param iBufLen			네트워크에서 수신된 SIP 메시지의 길이
 * @param pszIp				IP 주소
 * @param iPort				포트 번호
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
static bool SipMessageProcess( CSipStack * pclsSipStack, int iThreadId, const char * pszBuf, int iBufLen, const char * pszIp, unsigned short iPort )
{
	CLog::Print( LOG_NETWORK, "TlsRecv(%s:%d) [%.*s]", pszIp, iPort, iBufLen, pszBuf );

	CSipMessage	* pclsMessage = new CSipMessage();
	if( pclsMessage == NULL ) return false;

	if( pclsMessage->Parse( pszBuf, iBufLen ) == -1 )
	{
		delete pclsMessage;
		return false;
	}

	if( pclsMessage->IsRequest() )
	{
		pclsMessage->AddIpPortToTopVia( pszIp, iPort );
	}

	pclsMessage->m_eTransport = E_SIP_TLS;

	if( pclsSipStack->RecvSipMessage( iThreadId, pclsMessage ) == false )
	{
		delete pclsMessage;
	}

	return true;
}

/**
 * @brief TLS 세션을 위한 쓰레드 함수
 * @param lpParameter CThreadListEntry 객체의 포인터
 * @returns 0 을 리턴한다.
 */
#ifdef WIN32
DWORD WINAPI SipTlsThread( LPVOID lpParameter )
#else
void * SipTlsThread( void * lpParameter )
#endif
{
	CThreadListEntry * pclsEntry = (CThreadListEntry *)lpParameter;
	CSipStack * pclsSipStack = (CSipStack *)pclsEntry->m_pUser;
	CTcpSessionList	clsSessionList;
	CTcpComm			clsTcpComm;
	int		n, i, iBufLen, iThreadId;
	char	szBuf[2048], *pszBuf;
	time_t	iTime, iDeleteTime;

	pclsSipStack->IncreateTcpThreadCount( iThreadId );

	if( clsSessionList.Init( pclsSipStack->m_clsSetup.m_iTcpMaxSocketPerThread + 1 ) == false ) goto FUNC_END;
	if( clsSessionList.Insert( pclsEntry->m_hRecv ) == false ) goto FUNC_END;

	time( &iDeleteTime );
	while( pclsSipStack->m_bStopEvent == false )
	{
		n = poll( clsSessionList.m_psttPollFd, clsSessionList.m_iPoolFdCount, 1000 );
		time( &iTime );
		if( n <= 0 ) goto LOOP_END;

		if( clsSessionList.m_psttPollFd[0].revents & POLLIN )
		{
			if( CThreadList::RecvCommand( clsSessionList.m_psttPollFd[0].fd, (char *)&clsTcpComm, sizeof(clsTcpComm) ) == sizeof(clsTcpComm) )
			{
				SSL	* psttSsl;
				bool	bRes = false;

				if( SSLAccept( clsTcpComm.m_hSocket, &psttSsl, false, 0, pclsSipStack->m_clsSetup.m_iTlsAcceptTimeout ) )
				{
					if( clsSessionList.Insert( clsTcpComm, psttSsl ) )
					{
						pclsSipStack->m_clsTlsSocketMap.Insert( clsTcpComm.m_szIp, clsTcpComm.m_iPort, clsTcpComm.m_hSocket, psttSsl );
						bRes = true;
					}
					else
					{
						SSLClose( psttSsl );
					}
				}

				if( bRes == false )
				{
					closesocket( clsTcpComm.m_hSocket );
					pclsEntry->DecreaseSocketCount();
				}
			}
			--n;
		}

		if( n == 0 ) goto LOOP_END;

		for( i = 1; i < clsSessionList.m_iPoolFdCount; ++i )
		{
			if( !(clsSessionList.m_psttPollFd[i].revents & POLLIN) ) continue;

			n = SSLRecv( clsSessionList.m_clsList[i].m_psttSsl, szBuf, sizeof(szBuf) );
			if( n <= 0 )
			{
CLOSE_SESSION:
				pclsSipStack->m_clsTlsSocketMap.Delete( clsSessionList.m_clsList[i].m_strIp.c_str(), clsSessionList.m_clsList[i].m_iPort );
				clsSessionList.Delete( i, pclsEntry );
				continue;
			}

			clsSessionList.m_clsList[i].m_iRecvTime = iTime;

			if( clsSessionList.m_clsList[i].m_clsSipBuf.AddBuf( szBuf, n ) == false ) goto CLOSE_SESSION;
			while( clsSessionList.m_clsList[i].m_clsSipBuf.GetSipMessage( &pszBuf, &iBufLen ) )
			{
				SipMessageProcess( pclsSipStack, iThreadId, pszBuf, iBufLen, clsSessionList.m_clsList[i].m_strIp.c_str(), clsSessionList.m_clsList[i].m_iPort );
				clsSessionList.m_clsList[i].m_clsSipBuf.ShiftBuf( iBufLen );
			}
		}

LOOP_END:
		if( ( iDeleteTime + 5 ) < iTime )
		{
			clsSessionList.DeleteTimeout( pclsSipStack->m_clsSetup.m_iTcpRecvTimeout, pclsEntry );
			iDeleteTime = iTime;
		}
	}

	clsSessionList.DeleteAll( pclsEntry );

FUNC_END:
	pclsSipStack->DecreateTcpThreadCount();

	return 0;
}

/** 
 * @ingroup SipStack
 * @brief TLS 프로토콜로 SIP 메시지 수신 및 SIP 수신 이벤트를 처리하는 쓰레드 함수
 * @param lpParameter SIP stack 포인터
 * @returns 0 을 리턴한다.
 */
#ifdef WIN32
DWORD WINAPI SipTlsListenThread( LPVOID lpParameter )
#else
void * SipTlsListenThread( void * lpParameter )
#endif
{
	CSipStack * pclsSipStack = (CSipStack *)lpParameter;
	struct pollfd arrPollFd[2];
	int		iSocketCount = 0, n, i;
	bool	bRes;
	Socket	hConnFd;
	struct sockaddr_in sttAddr;
	socklen_t		iAddrLen;
	CTcpComm		clsTcpComm;

	if( pclsSipStack->m_hTcpSocket != INVALID_SOCKET )
	{
		TcpSetPollIn( arrPollFd[iSocketCount], pclsSipStack->m_hTcpSocket );
		++iSocketCount;
	}

	if( iSocketCount == 0 ) goto FUNC_END;

	while( pclsSipStack->m_bStopEvent == false )
	{
		n = poll( arrPollFd, iSocketCount, 1000 );
		if( n > 0 )
		{
			for( i = 0; i < iSocketCount; ++i )
			{
				if( !(arrPollFd[i].revents & POLLIN) ) continue;

				iAddrLen = sizeof(sttAddr);
				hConnFd = accept( arrPollFd[i].fd, (struct sockaddr *)&sttAddr, &iAddrLen );
				if( hConnFd == INVALID_SOCKET )
				{
					continue;
				}

				clsTcpComm.m_hSocket = hConnFd;
				clsTcpComm.m_iPort = ntohs( sttAddr.sin_port );

#ifdef WIN32
				snprintf( clsTcpComm.m_szIp, sizeof(clsTcpComm.m_szIp), "%s", inet_ntoa( sttAddr.sin_addr ) );
#else
				inet_ntop( AF_INET, &sttAddr.sin_addr, clsTcpComm.m_szIp, sizeof(clsTcpComm.m_szIp) );
#endif

				bRes = false;
				if( arrPollFd[i].fd == pclsSipStack->m_hTcpSocket )
				{
					bRes = pclsSipStack->m_clsTlsThreadList.SendCommand( (char *)&clsTcpComm, sizeof(clsTcpComm) );
				}

				if( bRes == false )
				{
					closesocket( hConnFd );
				}
			}
		}
	}

FUNC_END:
	return 0;
}

/**
 * @ingroup SipStack
 * @brief TLS 프로토콜로 SIP 메시지 수신 및 SIP 수신 이벤트를 처리하는 Thread Pool 을 시작한다.
 * @param pclsSipStack SIP stack 포인터
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool StartSipTlsListenThread( CSipStack * pclsSipStack )
{
	return StartThread( "SipTlsListenThread", SipTlsListenThread, pclsSipStack );
}
