
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
#include "SipServerSetup.h"
#include "MemoryDebug.h"

static int giRtpThreadCount = 0;
extern CSipMutex gclsCountMutex;

class CRtpThreadInfo
{
public:
	CRtpThreadInfo() : m_iPort(0)
	{
	}

	int	m_iPort;
};

/**
 * @ingroup KSipServer
 * @brief RTP relay ������ �Լ�
 * @param lpParameter �ǹ̾���
 * @returns 0 �� �����Ѵ�.
 */
THREAD_API RtpThread( LPVOID lpParameter )
{
	CRtpThreadInfo * pclsThreadInfo = (CRtpThreadInfo *)lpParameter;
	int				iPort = pclsThreadInfo->m_iPort;
	CRtpInfo	* pclsRtpInfo;
	pollfd		* psttPoll = NULL;
	int				n, iPacketLen;
	char			szPacket[1500];
	uint32_t	iIp;
	IN6_ADDR	sttIp;
	uint16_t	sPort;
	uint8_t		cPos, cNext;
	bool			bRecv;

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

	psttPoll = (pollfd *)malloc( sizeof(pollfd) * pclsRtpInfo->m_iSocketCount );
	if( psttPoll == NULL )
	{
		CLog::Print( LOG_ERROR, "rtp port(%d) memory error", iPort );
		gclsRtpMap.Delete( iPort );
		goto FUNC_END;
	}

	for( cPos = 0; cPos < pclsRtpInfo->m_iSocketCount; ++cPos )
	{
		psttPoll[cPos].fd = pclsRtpInfo->m_phSocket[cPos];
		psttPoll[cPos].events = POLLIN;
		psttPoll[cPos].revents = 0;
	}

	while( gbStop == false && pclsRtpInfo->m_bStop == false )
	{
		n = poll( psttPoll, pclsRtpInfo->m_iSocketCount, 1000 );
		if( n <= 0 ) continue;

		for( cPos = 0, cNext = 0; cPos < pclsRtpInfo->m_iSocketCount; ++cPos )
		{
			if( psttPoll[cPos].revents & POLLIN )
			{
				iPacketLen = sizeof(szPacket);

				if( gclsSetup.m_bIpv6 )
				{
					bRecv = UdpRecv( pclsRtpInfo->m_phSocket[cPos], szPacket, &iPacketLen, &sttIp, &sPort );
					if( bRecv )
					{
						if( pclsRtpInfo->m_piIp[cPos] == 0 )
						{
							pclsRtpInfo->SetIpPort( cPos, &sttIp, sPort );
							pclsRtpInfo->m_piIp[cPos] = 1;
						}
					}
				}
				else
				{
					bRecv = UdpRecv( pclsRtpInfo->m_phSocket[cPos], szPacket, &iPacketLen, &iIp, &sPort );
					if( bRecv )
					{
						if( pclsRtpInfo->m_piIp[cPos] == 0 )
						{
							pclsRtpInfo->SetIpPort( cPos, iIp, sPort );
						}
					}
				}

				if( bRecv )
				{
					if( cNext % 2 == 0 )
					{
						if( pclsRtpInfo->m_piIp[cPos+2] )
						{
							pclsRtpInfo->Send( cPos+2, szPacket, iPacketLen );
						}
					}
					else
					{
						if( pclsRtpInfo->m_piIp[cPos-2] )
						{
							pclsRtpInfo->Send( cPos-2, szPacket, iPacketLen );
						}
					}
				}
			}

			if( cPos % 2 == 1 )
			{
				++cNext;
			}
		}
	}

	if( psttPoll )
	{
		free( psttPoll );
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
