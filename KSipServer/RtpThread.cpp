
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

static int giRtpThreadCount = 0;
extern CSipMutex gclsCountMutex;

/**
 * @ingroup KSipServer
 * @brief RTP relay 쓰레드 함수
 * @param lpParameter 의미없음
 * @returns 0 을 리턴한다.
 */
#ifdef WIN32
DWORD WINAPI RtpThread( LPVOID lpParameter )
#else
void * RtpThread( void * lpParameter )
#endif
{
	int				iPort = (int)lpParameter;
	CRtpInfo	* pclsRtpInfo;
	pollfd		sttPoll[4];
	int				n, iPacketLen;
	char			szPacket[1500];
	uint32_t	iIp;
	uint16_t	sPort;

	gclsCountMutex.acquire();
	++giRtpThreadCount;
	gclsCountMutex.release();

	CLog::Print( LOG_INFO, "RtpThread is started - port(%d)", iPort );

	if( gclsRtpMap.Select( iPort, &pclsRtpInfo ) == false )
	{
		CLog::Print( LOG_ERROR, "rtp port(%d) is not found in RtpMap", iPort );
		goto FUNC_END;
	}

	for( int i = 0; i < RTP_INFO_SOCKET_COUNT; ++i )
	{
		sttPoll[i].fd = pclsRtpInfo->m_arrSocket[i];
		sttPoll[i].events = POLLIN;
		sttPoll[i].revents = 0;
	}

	while( gbStop == false && pclsRtpInfo->m_bStop == false )
	{
		n = poll( sttPoll, RTP_INFO_SOCKET_COUNT, 1000 );
		if( n <= 0 ) continue;

		if( sttPoll[0].revents & POLLIN )
		{
			iPacketLen = sizeof(szPacket);
			if( UdpRecv( pclsRtpInfo->m_arrSocket[0], szPacket, &iPacketLen, &iIp, &sPort ) )
			{
				if( pclsRtpInfo->m_arrIp[0] == 0 )
				{
					pclsRtpInfo->SetIpPort( 0, iIp, sPort );
				}

				if( pclsRtpInfo->m_arrIp[2] )
				{
					pclsRtpInfo->Send( 2, szPacket, iPacketLen );
				}
			}
		}

		if( sttPoll[1].revents & POLLIN )
		{
			iPacketLen = sizeof(szPacket);
			if( UdpRecv( pclsRtpInfo->m_arrSocket[1], szPacket, &iPacketLen, &iIp, &sPort ) )
			{
				if( pclsRtpInfo->m_arrIp[1] == 0 )
				{
					pclsRtpInfo->SetIpPort( 1, iIp, sPort );
				}

				if( pclsRtpInfo->m_arrIp[3] )
				{
					pclsRtpInfo->Send( 3, szPacket, iPacketLen );
				}
			}
		}

		if( sttPoll[2].revents & POLLIN )
		{
			iPacketLen = sizeof(szPacket);
			if( UdpRecv( pclsRtpInfo->m_arrSocket[2], szPacket, &iPacketLen, &iIp, &sPort ) )
			{
				if( pclsRtpInfo->m_arrIp[2] == 0 )
				{
					pclsRtpInfo->SetIpPort( 2, iIp, sPort );
				}

				if( pclsRtpInfo->m_arrIp[0] )
				{
					pclsRtpInfo->Send( 0, szPacket, iPacketLen );
				}
			}
		}

		if( sttPoll[3].revents & POLLIN )
		{
			iPacketLen = sizeof(szPacket);
			if( UdpRecv( pclsRtpInfo->m_arrSocket[3], szPacket, &iPacketLen, &iIp, &sPort ) )
			{
				if( pclsRtpInfo->m_arrIp[3] == 0 )
				{
					pclsRtpInfo->SetIpPort( 3, iIp, sPort );
				}

				if( pclsRtpInfo->m_arrIp[1] )
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
 * @brief RTP relay 쓰레드를 시작한다.
 * @param	iPort	첫번째 RTP 포트 번호
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool StartRtpThread( int iPort )
{
	if( StartThread( "RtpThread", RtpThread, (void *)iPort ) == false ) return false;

	return true;
}

/**
 * @ingroup KSipServer
 * @brief RTP relay 쓰레드 실행 유무를 검사한다.
 * @returns RTP relay 쓰레드가 실행 중이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsRtpThreadRun()
{
	if( giRtpThreadCount > 0 ) return true;

	return false;
}
