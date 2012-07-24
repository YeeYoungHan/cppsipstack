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

/**
 * @brief SIP �޽����� �Ľ��Ͽ��� SIP stack �� �Է��Ѵ�.
 * @param pclsSipStack	SIP stack
 * @param pszBuf				��Ʈ��ũ���� ���ŵ� SIP �޽���
 * @param iBufLen				��Ʈ��ũ���� ���ŵ� SIP �޽����� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool SipMessageProcess( CSipStack * pclsSipStack, int iThreadId, const char * pszBuf, int iBufLen, const char * pszIp, unsigned short iPort )
{
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

	if( pclsSipStack->RecvSipMessage( iThreadId, pclsMessage ) == false )
	{
		delete pclsMessage;
	}

	return true;
}

/** 
 * @brief UDP �������ݷ� SIP �޽��� ���� �� SIP ���� �̺�Ʈ�� ó���ϴ� ������ �Լ�
 * @param lpParameter SIP stack ������
 * @returns 0 �� �����Ѵ�.
 */
#ifdef WIN32
DWORD WINAPI SipUdpThread( LPVOID lpParameter )
#else
void * SipUdpThread( void * lpParameter )
#endif
{
	CSipStack * pclsSipStack = (CSipStack *)lpParameter;
	struct pollfd arrPoll[1];
	int		iThreadId, n, iPacketSize;
	char	szPacket[SIP_PACKET_MAX_SIZE], szIp[16];
	unsigned short sPort;
	bool	bRes;

	pclsSipStack->IncreateUdpThreadCount( iThreadId );
	TcpSetPollIn( arrPoll[0], pclsSipStack->m_iUdpSocket );

	while( pclsSipStack->m_bStopEvent == false )
	{
		pclsSipStack->m_clsUdpRecvMutex.acquire();
		n = poll( arrPoll, 1, 1000 );
		if( n > 0 )
		{
			iPacketSize = sizeof(szPacket);
			bRes = UdpRecv( pclsSipStack->m_iUdpSocket, szPacket, &iPacketSize, szIp, sizeof(szIp), &sPort );
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

	pclsSipStack->DecreateUdpThreadCount();

	return 0;
}

/**
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
