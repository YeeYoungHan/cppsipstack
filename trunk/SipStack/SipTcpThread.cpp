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

/** 
 * @ingroup SipStack
 * @brief TCP �������ݷ� SIP �޽��� ���� �� SIP ���� �̺�Ʈ�� ó���ϴ� ������ �Լ�
 * @param lpParameter SIP stack ������
 * @returns 0 �� �����Ѵ�.
 */
#ifdef WIN32
DWORD WINAPI SipTcpListenThread( LPVOID lpParameter )
#else
void * SipTcpListenThread( void * lpParameter )
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
					bRes = pclsSipStack->m_clsTcpThreadList.SendCommand( (char *)&clsTcpComm, sizeof(clsTcpComm) );
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
 * @brief TCP �������ݷ� SIP �޽��� ���� �� SIP ���� �̺�Ʈ�� ó���ϴ� Thread Pool �� �����Ѵ�.
 * @param pclsSipStack SIP stack ������
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool StartSipTcpThread( CSipStack * pclsSipStack )
{
	return StartThread( "SipTcpListenThread", SipTcpListenThread, pclsSipStack );
}
