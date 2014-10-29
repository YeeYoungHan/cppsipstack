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
#include "ServerUtility.h"
#include "Log.h"
#include "MemoryDebug.h"

/**
 * @ingroup SipStack
 * @brief SIP �޽����� �Ľ��Ͽ��� SIP stack �� �Է��Ѵ�.
 * @param pclsSipStack SIP stack
 * @param iThreadId		UDP ������ ��ȣ
 * @param pszBuf			��Ʈ��ũ���� ���ŵ� SIP �޽���
 * @param iBufLen			��Ʈ��ũ���� ���ŵ� SIP �޽����� ����
 * @param pszIp				IP �ּ�
 * @param iPort				��Ʈ ��ȣ
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
static bool SipMessageProcess( CSipStack * pclsSipStack, int iThreadId, const char * pszBuf, int iBufLen, const char * pszIp, unsigned short iPort )
{
	CLog::Print( LOG_NETWORK, "UdpRecv(%s:%d) [%s]", pszIp, iPort, pszBuf );

	return pclsSipStack->RecvSipMessage( iThreadId, pszBuf, iBufLen, pszIp, iPort, E_SIP_UDP );
}

/** 
 * @ingroup SipStack
 * @brief UDP �������ݷ� SIP �޽��� ���� �� SIP ���� �̺�Ʈ�� ó���ϴ� ������ �Լ�
 * @param lpParameter SIP stack ������
 * @returns 0 �� �����Ѵ�.
 */
THREAD_API SipUdpThread( LPVOID lpParameter )
{
	CSipStack * pclsSipStack = (CSipStack *)lpParameter;
	struct pollfd arrPoll[1];
	int		iThreadId, n, iPacketSize;
	char	szPacket[SIP_PACKET_MAX_SIZE], szIp[INET6_ADDRSTRLEN];
	unsigned short sPort;
	bool	bRes;

	pclsSipStack->IncreateUdpThreadCount( iThreadId );
	TcpSetPollIn( arrPoll[0], pclsSipStack->m_hUdpSocket );

	while( pclsSipStack->m_bStopEvent == false )
	{
		pclsSipStack->m_clsUdpRecvMutex.acquire();
		n = poll( arrPoll, 1, 1000 );
		if( n > 0 )
		{
			iPacketSize = sizeof(szPacket);
			bRes = UdpRecv( pclsSipStack->m_hUdpSocket, szPacket, &iPacketSize, szIp, sizeof(szIp), &sPort, pclsSipStack->m_clsSetup.m_bIpv6 );
			pclsSipStack->m_clsUdpRecvMutex.release();

			if( bRes )
			{
				if( iPacketSize < SIP_PACKET_MIN_SIZE || szPacket[0] == '\0' || szPacket[0] == '\r' || szPacket[0] == '\n' ) continue;

				SipMessageProcess( pclsSipStack, iThreadId, szPacket, iPacketSize, szIp, sPort );
			}
		}
		else
		{
			pclsSipStack->m_clsUdpRecvMutex.release();
		}
	}

	pclsSipStack->ThreadEnd( iThreadId );
	pclsSipStack->DecreateUdpThreadCount();

	return 0;
}

/**
 * @ingroup SipStack
 * @brief UDP �������ݷ� SIP �޽��� ���� �� SIP ���� �̺�Ʈ�� ó���ϴ� Thread Pool �� �����Ѵ�.
 * @param pclsSipStack SIP stack ������
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool StartSipUdpThread( CSipStack * pclsSipStack )
{
	for( int i = 0; i < pclsSipStack->m_clsSetup.m_iUdpThreadCount; ++i )
	{
		char	szMsg[101];

		snprintf( szMsg, sizeof(szMsg), "SipUdpThread(%d)", i + 1 );
		if( StartThread( szMsg, SipUdpThread, pclsSipStack ) == false )
		{
			return false;
		}
	}

	return true;
}
