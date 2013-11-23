
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

#include "SipStackDefine.h"
#include "KSipServer.h"
#include "Log.h"
#include "RtpMap.h"
#include "ServerService.h"
#include "ServerUtility.h"

static int giRtpThreadCount = 0;
extern CSipMutex gclsCountMutex;

class CRtpThreadInfo
{
public:
	int	m_iPort;
};

/**
 * @ingroup KSipServer
 * @brief RTP relay ������ �Լ�
 * @param lpParameter �ǹ̾���
 * @returns 0 �� �����Ѵ�.
 */
#ifdef WIN32
DWORD WINAPI RtpThread( LPVOID lpParameter )
#else
void * RtpThread( void * lpParameter )
#endif
{
	CRtpThreadInfo * pclsThreadInfo = (CRtpThreadInfo *)lpParameter;
	int				iPort = pclsThreadInfo->m_iPort;
	CRtpInfo	* pclsRtpInfo;
	pollfd		sttPoll[4];
	int				n, iPacketLen;
	char			szPacket[1500];
	uint32_t	iIp;
	uint16_t	sPort;

	delete pclsThreadInfo;

	gclsCountMutex.acquire();
	++giRtpThreadCount;
	gclsCountMutex.release();

	CLog::Print( LOG_INFO, "RtpThread is started - port(%d)", iPort );

	if( gclsRtpMap.Select( iPort, &pclsRtpInfo ) == false )
	{
		CLog::Print( LOG_ERROR, "rtp port(%d) is not found in RtpMap", iPort );
		goto FUNC_END;
	}

	for( uint8_t i = 0; i < pclsRtpInfo->m_iSocketCount; ++i )
	{
		sttPoll[i].fd = pclsRtpInfo->m_phSocket[i];
		sttPoll[i].events = POLLIN;
		sttPoll[i].revents = 0;
	}

	while( gbStop == false && pclsRtpInfo->m_bStop == false )
	{
		n = poll( sttPoll, pclsRtpInfo->m_iSocketCount, 1000 );
		if( n <= 0 ) continue;

		if( sttPoll[0].revents & POLLIN )
		{
			iPacketLen = sizeof(szPacket);
			if( UdpRecv( pclsRtpInfo->m_phSocket[0], szPacket, &iPacketLen, &iIp, &sPort ) )
			{
				if( pclsRtpInfo->m_piIp[0] == 0 )
				{
					pclsRtpInfo->SetIpPort( 0, iIp, sPort );
				}

				if( pclsRtpInfo->m_piIp[2] )
				{
					pclsRtpInfo->Send( 2, szPacket, iPacketLen );
				}
			}
		}

		if( sttPoll[1].revents & POLLIN )
		{
			iPacketLen = sizeof(szPacket);
			if( UdpRecv( pclsRtpInfo->m_phSocket[1], szPacket, &iPacketLen, &iIp, &sPort ) )
			{
				if( pclsRtpInfo->m_piIp[1] == 0 )
				{
					pclsRtpInfo->SetIpPort( 1, iIp, sPort );
				}

				if( pclsRtpInfo->m_piIp[3] )
				{
					pclsRtpInfo->Send( 3, szPacket, iPacketLen );
				}
			}
		}

		if( sttPoll[2].revents & POLLIN )
		{
			iPacketLen = sizeof(szPacket);
			if( UdpRecv( pclsRtpInfo->m_phSocket[2], szPacket, &iPacketLen, &iIp, &sPort ) )
			{
				if( pclsRtpInfo->m_piIp[2] == 0 )
				{
					pclsRtpInfo->SetIpPort( 2, iIp, sPort );
				}

				if( pclsRtpInfo->m_piIp[0] )
				{
					pclsRtpInfo->Send( 0, szPacket, iPacketLen );
				}
			}
		}

		if( sttPoll[3].revents & POLLIN )
		{
			iPacketLen = sizeof(szPacket);
			if( UdpRecv( pclsRtpInfo->m_phSocket[3], szPacket, &iPacketLen, &iIp, &sPort ) )
			{
				if( pclsRtpInfo->m_piIp[3] == 0 )
				{
					pclsRtpInfo->SetIpPort( 3, iIp, sPort );
				}

				if( pclsRtpInfo->m_piIp[1] )
				{
					pclsRtpInfo->Send( 1, szPacket, iPacketLen );
				}
			}
		}

	}

	gclsRtpMap.Delete( iPort );

FUNC_END:
	CLog::Print( LOG_INFO, "RtpThread is terminated - port(%d)", iPort );
	gclsCountMutex.acquire();
	--giRtpThreadCount;
	gclsCountMutex.release();

	return 0;
}

/**
 * @ingroup KSipServer
 * @brief RTP relay �����带 �����Ѵ�.
 * @param	iPort	ù��° RTP ��Ʈ ��ȣ
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool StartRtpThread( int iPort )
{
	CRtpThreadInfo * pclsInfo = new CRtpThreadInfo();
	if( pclsInfo == NULL ) return false;

	pclsInfo->m_iPort = iPort;

	if( StartThread( "RtpThread", RtpThread, pclsInfo ) == false ) return false;

	return true;
}

/**
 * @ingroup KSipServer
 * @brief RTP relay ������ ���� ������ �˻��Ѵ�.
 * @returns RTP relay �����尡 ���� ���̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool IsRtpThreadRun()
{
	if( giRtpThreadCount > 0 ) return true;

	return false;
}
