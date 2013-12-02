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

#include "stdafx.h"
#include "SipStackDefine.h"
#include "SipUserAgent.h"
#include "Setup.h"
#include "TimeUtility.h"
#include "ServerUtility.h"
#include "TestInfo.h"
#include "RtpHeader.h"

static bool gbStopRtpThread = false;
static bool gbRtpThreadRun = false;

void InitRtpHeader( CRtpHeader * pclsRtpHeader )
{
	pclsRtpHeader->SetVersion(2);
	pclsRtpHeader->SetPadding(0);
	pclsRtpHeader->SetExtension(0);
	pclsRtpHeader->SetCC(0);
	pclsRtpHeader->SetMarker(0);
	pclsRtpHeader->ssrc = htonl( 100 );
}

/**
 * @brief RTP 쓰레드
 * @param lpParameter 
 * @returns 0 을 리턴한다.
 */
DWORD WINAPI RtpThread( LPVOID lpParameter )
{
	struct pollfd arrPoll[2];
	char	szSendPacket[1500], szRecvPacket[1500], szIp[41];
	CRtpHeader * pclsRtpHeader = (CRtpHeader *)szSendPacket;
	int iRtpCount = 0;
	int iEvent, iRecvLen;
	uint16_t	sPort;

	gclsTestInfo.ClearRtp();

	InitRtpHeader( pclsRtpHeader );

	TcpSetPollIn( arrPoll[0], gclsTestInfo.m_clsCallerRtp.m_hSocket );
	TcpSetPollIn( arrPoll[1], gclsTestInfo.m_clsCalleeRtp.m_hSocket );

	while( gbStopRtpThread == false )
	{
		++iRtpCount;
		snprintf( szSendPacket + sizeof(CRtpHeader), sizeof(szSendPacket) - sizeof(CRtpHeader), "caller%d", iRtpCount );
		UdpSend( gclsTestInfo.m_clsCallerRtp.m_hSocket, szSendPacket, sizeof(CRtpHeader) + 160
			, gclsTestInfo.m_clsCallerPeerRtp.m_strIp.c_str(), gclsTestInfo.m_clsCallerPeerRtp.m_iPort );

		snprintf( szSendPacket + sizeof(CRtpHeader), sizeof(szSendPacket) - sizeof(CRtpHeader), "callee%d", iRtpCount );
		UdpSend( gclsTestInfo.m_clsCalleeRtp.m_hSocket, szSendPacket, sizeof(CRtpHeader) + 160
			, gclsTestInfo.m_clsCalleePeerRtp.m_strIp.c_str(), gclsTestInfo.m_clsCalleePeerRtp.m_iPort );

		iEvent = poll( arrPoll, 2, 20 );
		if( iEvent > 0 )
		{
			if( arrPoll[0].revents & POLLIN )
			{
				iRecvLen = sizeof(szRecvPacket);
				if( UdpRecv( gclsTestInfo.m_clsCallerRtp.m_hSocket, szRecvPacket, &iRecvLen, szIp, sizeof(szIp), &sPort ) == false )
				{
					SendLog( "[ERROR] caller RTP socket recv error" );
					break;
				}
				else
				{
					if( strncmp( szRecvPacket + sizeof(CRtpHeader), "callee", 6 ) )
					{
						SendLog( "[ERROR] caller RTP socket recv error [%s]", szRecvPacket + sizeof(CRtpHeader) );
						break;
					}
					else
					{
						++gclsTestInfo.m_clsCallerRtp.m_iRecvCount;
					}
				}
			}

			if( arrPoll[1].revents & POLLIN )
			{
				iRecvLen = sizeof(szRecvPacket);
				if( UdpRecv( gclsTestInfo.m_clsCalleeRtp.m_hSocket, szRecvPacket, &iRecvLen, szIp, sizeof(szIp), &sPort ) == false )
				{
					SendLog( "[ERROR] callee RTP socket recv error" );
					break;
				}
				else
				{
					if( strncmp( szRecvPacket + sizeof(CRtpHeader), "caller", 6 ) )
					{
						SendLog( "[ERROR] callee RTP socket recv error [%s]", szRecvPacket + sizeof(CRtpHeader) );
						break;
					}
					else
					{
						++gclsTestInfo.m_clsCalleeRtp.m_iRecvCount;
					}
				}
			}

			Sleep(20);
		}

		if( iRtpCount >= 100 ) break;
	}

	gclsSipUserAgent.StopCall( gclsTestInfo.m_strCallerCallId.c_str() );

	if( iRtpCount >= 100 && gclsTestInfo.m_clsCallerRtp.m_iRecvCount >= 95 && gclsTestInfo.m_clsCalleeRtp.m_iRecvCount >= 95 )
	{
		SendLog( "RTP caller recv count(%d) callee recv count(%d)"
			, gclsTestInfo.m_clsCallerRtp.m_iRecvCount, gclsTestInfo.m_clsCalleeRtp.m_iRecvCount );
	}
	else
	{
		SendLog( "[ERROR] RTP caller recv count(%d) callee recv count(%d)"
			, gclsTestInfo.m_clsCallerRtp.m_iRecvCount, gclsTestInfo.m_clsCalleeRtp.m_iRecvCount );
	}

	gbRtpThreadRun = false;

	return 0;
}

/**
 * @brief RTP 쓰레드를 시작한다.
 * @param hWnd 테스트 결과를 전달할 윈도우 핸들
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool StartRtpThread( )
{
	if( gbRtpThreadRun ) return false;

	gbStopRtpThread = false;

	if( StartThread( "RtpThread", RtpThread, NULL ) == false ) return false;

	return true;
}

/**
 * @brief RTP 쓰레드 실행 중지 요청한다.
 */
void StopRtpThread()
{
	gbStopRtpThread = true;
}

/**
 * @brief RTP 쓰레드가 실행되고 있는가?
 * @returns RTP 쓰레드가 실행되고 있으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsRtpThreadRun()
{
	return gbRtpThreadRun;
}
