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
#include "SipUdp.h"
#include "CallMap.h"
#include "RoomMap.h"
#include "Log.h"
#include "ServerUtility.h"

#define MAX_POLL_COUNT	16

static int giRtpThreadCount = 0;
CSipMutex gclsCountMutex;

/**
 * @brief RTP relay ������ ����
 */
class CRtpThreadInfo
{
public:
	std::string	m_strRoomId;
};

/**
 * @ingroup McuEmulator
 * @brief RTP relay ������ �Լ�
 * @param lpParameter �ǹ̾���
 * @returns 0 �� �����Ѵ�.
 */
THREAD_API RtpThread( LPVOID lpParameter )
{
	pollfd		sttPoll[MAX_POLL_COUNT];
	int				n, iPacketLen, iPollCount;
	char			szPacket[1500];
	uint32_t	iIp;
	uint16_t	sPort;
	CRtpThreadInfo * pclsRtpThreadInfo = (CRtpThreadInfo *)lpParameter;
	CRoomInfo	clsRoomInfo;
	CCallInfo	clsCallInfo;
	CALL_ID_LIST::iterator	itCIL;

	gclsCountMutex.acquire();
	++giRtpThreadCount;
	gclsCountMutex.release();

	CLog::Print( LOG_INFO, "RtpThread is started - room_id(%s)", pclsRtpThreadInfo->m_strRoomId.c_str() );

	while( 1 )
	{
		if( gclsRoomMap.Select( pclsRtpThreadInfo->m_strRoomId.c_str(), clsRoomInfo ) == false )
		{
			CLog::Print( LOG_DEBUG, "%s room_id(%s) select error", __FUNCTION__, pclsRtpThreadInfo->m_strRoomId.c_str() );
			break;
		}

		if( clsRoomInfo.m_clsCallIdList.size() == 0 )
		{
			CLog::Print( LOG_DEBUG, "%s room_id(%s) callid count is zero", __FUNCTION__, pclsRtpThreadInfo->m_strRoomId.c_str() );
			break;
		}

		iPollCount = 0;

		// ȸ�Ƿ� �������� RTP ������ �����´�.
		for( itCIL = clsRoomInfo.m_clsCallIdList.begin(); itCIL != clsRoomInfo.m_clsCallIdList.end(); ++itCIL )
		{
			if( gclsCallMap.Select( itCIL->c_str(), clsCallInfo ) )
			{
				if( (iPollCount + 1) >= MAX_POLL_COUNT ) break;

				TcpSetPollIn( sttPoll[iPollCount++], clsCallInfo.m_hAudioRtp );
				TcpSetPollIn( sttPoll[iPollCount++], clsCallInfo.m_hAudioRtcp );
				TcpSetPollIn( sttPoll[iPollCount++], clsCallInfo.m_hVideoRtp );
				TcpSetPollIn( sttPoll[iPollCount++], clsCallInfo.m_hVideoRtcp );
			}
		}

		if( iPollCount == 0 )
		{
			sleep( 1 );
			continue;
		}

		n = poll( sttPoll, iPollCount, 1000 );
		if( n <= 0 ) continue;

		for( int iIndex = 0; iIndex < iPollCount; ++iIndex )
		{
			if( sttPoll[iIndex].revents & POLLIN )
			{
				iPacketLen = sizeof(szPacket);
				if( UdpRecv( sttPoll[iIndex].fd, szPacket, &iPacketLen, &iIp, &sPort ) )
				{
					UdpSend( sttPoll[iIndex].fd, szPacket, iPacketLen, iIp, sPort );
				}
			}
		}
	}

	CLog::Print( LOG_INFO, "RtpThread is terminated - room_id(%s)", pclsRtpThreadInfo->m_strRoomId.c_str() );

	delete pclsRtpThreadInfo;

	gclsCountMutex.acquire();
	--giRtpThreadCount;
	gclsCountMutex.release();

	return 0;
}

/**
 * @ingroup McuEmulator
 * @brief RTP relay �����带 �����Ѵ�.
 * @param	pszRoomId	ȸ�ǹ� ��ȣ
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool StartRtpThread( const char * pszRoomId )
{
	CRtpThreadInfo * pclsInfo = new CRtpThreadInfo();
	if( pclsInfo == NULL ) return false;

	pclsInfo->m_strRoomId = pszRoomId;

	if( StartThread( "RtpThread", RtpThread, pclsInfo ) == false ) return false;

	return true;
}

/**
 * @ingroup McuEmulator
 * @brief RTP relay ������ ���� ������ �˻��Ѵ�.
 * @returns RTP relay �����尡 ���� ���̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool IsRtpThreadRun()
{
	if( giRtpThreadCount > 0 ) return true;

	return false;
}
