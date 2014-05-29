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

/**
 * @ingroup SipTest
 * @brief RTP 헤더를 초기화시킨다.
 * @param pclsRtpHeader RTP 헤더
 */
void InitRtpHeader( CRtpHeader * pclsRtpHeader )
{
	pclsRtpHeader->SetVersion(2);
	pclsRtpHeader->SetPadding(0);
	pclsRtpHeader->SetExtension(0);
	pclsRtpHeader->SetCC(0);
	pclsRtpHeader->SetMarker(0);
	pclsRtpHeader->SetSsrc( 100 );
}

/**
 * @ingroup SipTest
 * @brief RTP 쓰레드
 * @param lpParameter 
 * @returns 0 을 리턴한다.
 */
DWORD WINAPI RtpThread( LPVOID lpParameter )
{
	struct pollfd arrPoll[4];
	char	szSendPacket[1500], szRecvPacket[1500], szIp[41];
	CRtpHeader * pclsRtpHeader = (CRtpHeader *)szSendPacket;
	int iRtpCount = 0;
	int iEvent, iRecvLen, iPollCount = 2;
	uint16_t	sPort;

	gclsTestInfo.m_bResult = false;
	gclsTestInfo.ClearRtp();

	InitRtpHeader( pclsRtpHeader );

	TcpSetPollIn( arrPoll[0], gclsTestInfo.m_clsCallerRtp.m_hSocket );
	TcpSetPollIn( arrPoll[1], gclsTestInfo.m_clsCalleeRtp.m_hSocket );
	
	if( gclsSetup.m_bUseTwoMedia )
	{
		iPollCount = 4;

		TcpSetPollIn( arrPoll[2], gclsTestInfo.m_clsCallerVideoRtp.m_hSocket );
		TcpSetPollIn( arrPoll[3], gclsTestInfo.m_clsCalleeVideoRtp.m_hSocket );
	}

	while( gbStopRtpThread == false )
	{
		++iRtpCount;
		snprintf( szSendPacket + sizeof(CRtpHeader), sizeof(szSendPacket) - sizeof(CRtpHeader), "caller%d", iRtpCount );
		UdpSend( gclsTestInfo.m_clsCallerRtp.m_hSocket, szSendPacket, sizeof(CRtpHeader) + 160
			, gclsTestInfo.m_clsCallerPeerRtp.m_strIp.c_str(), gclsTestInfo.m_clsCallerPeerRtp.m_iPort );

		snprintf( szSendPacket + sizeof(CRtpHeader), sizeof(szSendPacket) - sizeof(CRtpHeader), "callee%d", iRtpCount );
		UdpSend( gclsTestInfo.m_clsCalleeRtp.m_hSocket, szSendPacket, sizeof(CRtpHeader) + 160
			, gclsTestInfo.m_clsCalleePeerRtp.m_strIp.c_str(), gclsTestInfo.m_clsCalleePeerRtp.m_iPort );

		if( gclsSetup.m_bUseTwoMedia )
		{
			snprintf( szSendPacket + sizeof(CRtpHeader), sizeof(szSendPacket) - sizeof(CRtpHeader), "caller video%d", iRtpCount );
			UdpSend( gclsTestInfo.m_clsCallerVideoRtp.m_hSocket, szSendPacket, sizeof(CRtpHeader) + 160
				, gclsTestInfo.m_clsCallerPeerRtp.m_strIp.c_str(), gclsTestInfo.m_clsCallerPeerRtp.GetVideoPort() );

			snprintf( szSendPacket + sizeof(CRtpHeader), sizeof(szSendPacket) - sizeof(CRtpHeader), "callee video%d", iRtpCount );
			UdpSend( gclsTestInfo.m_clsCalleeVideoRtp.m_hSocket, szSendPacket, sizeof(CRtpHeader) + 160
				, gclsTestInfo.m_clsCalleePeerRtp.m_strIp.c_str(), gclsTestInfo.m_clsCalleePeerRtp.GetVideoPort() );
		}

		iEvent = poll( arrPoll, iPollCount, 20 );
		if( iEvent > 0 )
		{
			if( arrPoll[0].revents & POLLIN )
			{
				iRecvLen = sizeof(szRecvPacket);
				if( UdpRecv( gclsTestInfo.m_clsCallerRtp.m_hSocket, szRecvPacket, &iRecvLen, szIp, sizeof(szIp), &sPort ) == false )
				{
					SendLog( "[ERROR] caller RTP socket recv error(%d)", GetError() );
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
					SendLog( "[ERROR] callee RTP socket recv error(%d)", GetError() );
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

			if( gclsSetup.m_bUseTwoMedia )
			{
				if( arrPoll[2].revents & POLLIN )
				{
					iRecvLen = sizeof(szRecvPacket);
					if( UdpRecv( gclsTestInfo.m_clsCallerVideoRtp.m_hSocket, szRecvPacket, &iRecvLen, szIp, sizeof(szIp), &sPort ) == false )
					{
						SendLog( "[ERROR] caller video RTP socket recv error(%d)", GetError() );
						break;
					}
					else
					{
						if( strncmp( szRecvPacket + sizeof(CRtpHeader), "callee video", 12 ) )
						{
							SendLog( "[ERROR] caller vidoe RTP socket recv error [%s]", szRecvPacket + sizeof(CRtpHeader) );
							break;
						}
						else
						{
							++gclsTestInfo.m_clsCallerVideoRtp.m_iRecvCount;
						}
					}
				}

				if( arrPoll[3].revents & POLLIN )
				{
					iRecvLen = sizeof(szRecvPacket);
					if( UdpRecv( gclsTestInfo.m_clsCalleeVideoRtp.m_hSocket, szRecvPacket, &iRecvLen, szIp, sizeof(szIp), &sPort ) == false )
					{
						SendLog( "[ERROR] callee video RTP socket recv error(%d)", GetError() );
						break;
					}
					else
					{
						if( strncmp( szRecvPacket + sizeof(CRtpHeader), "caller video", 12 ) )
						{
							SendLog( "[ERROR] callee video RTP socket recv error [%s]", szRecvPacket + sizeof(CRtpHeader) );
							break;
						}
						else
						{
							++gclsTestInfo.m_clsCalleeVideoRtp.m_iRecvCount;
						}
					}
				}
			}

			Sleep(20);
		}

		if( iRtpCount >= 100 ) break;
	}

	if( gclsTestInfo.m_eTestType != E_TEST_TRANSFER ) 
	{
		// blind transfer 테스트에서는 최초 통화를 종료하지 않는다.
		// blind transfer 가 진행될 때에 서버에서 통화를 종료한다.
		gclsSipUserAgent.StopCall( gclsTestInfo.m_strCallerCallId.c_str() );
	}

	if( iRtpCount >= 100 && gclsTestInfo.m_clsCallerRtp.m_iRecvCount >= 95 && gclsTestInfo.m_clsCalleeRtp.m_iRecvCount >= 95 )
	{
		if( gclsSetup.m_bUseTwoMedia )
		{
			if( gclsTestInfo.m_clsCallerVideoRtp.m_iRecvCount >= 95 && gclsTestInfo.m_clsCalleeVideoRtp.m_iRecvCount >= 95 )
			{
				SendLog( "RTP audio caller recv count(%d) callee recv count(%d)"
					, gclsTestInfo.m_clsCallerRtp.m_iRecvCount, gclsTestInfo.m_clsCalleeRtp.m_iRecvCount );

				SendLog( "RTP video caller recv count(%d) callee recv count(%d)"
					, gclsTestInfo.m_clsCallerVideoRtp.m_iRecvCount, gclsTestInfo.m_clsCalleeVideoRtp.m_iRecvCount );

				gclsTestInfo.m_bResult = true;
			}
		}
		else
		{
			SendLog( "RTP caller recv count(%d) callee recv count(%d)"
				, gclsTestInfo.m_clsCallerRtp.m_iRecvCount, gclsTestInfo.m_clsCalleeRtp.m_iRecvCount );

			gclsTestInfo.m_bResult = true;
		}
	}
	
	if( gclsTestInfo.m_bResult == false )
	{
		if( gclsSetup.m_bUseTwoMedia )
		{
			SendLog( "[ERROR] RTP audio caller recv count(%d) callee recv count(%d)"
				, gclsTestInfo.m_clsCallerRtp.m_iRecvCount, gclsTestInfo.m_clsCalleeRtp.m_iRecvCount );

			SendLog( "[ERROR] RTP video caller recv count(%d) callee recv count(%d)"
				, gclsTestInfo.m_clsCallerVideoRtp.m_iRecvCount, gclsTestInfo.m_clsCalleeVideoRtp.m_iRecvCount );
		}
		else
		{
			SendLog( "[ERROR] RTP caller recv count(%d) callee recv count(%d)"
				, gclsTestInfo.m_clsCallerRtp.m_iRecvCount, gclsTestInfo.m_clsCalleeRtp.m_iRecvCount );
		}
	}

	gbRtpThreadRun = false;
	gclsTestInfo.m_bRtpThreadEnd = true;

	return 0;
}

/**
 * @ingroup SipTest
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
 * @ingroup SipTest
 * @brief RTP 쓰레드 실행 중지 요청한다.
 */
void StopRtpThread()
{
	gbStopRtpThread = true;

	for( int i = 0; i < 10; ++i )
	{
		if( gbRtpThreadRun == false ) break;

		Sleep(20);
	}
}

/**
 * @ingroup SipTest
 * @brief RTP 쓰레드가 실행되고 있는가?
 * @returns RTP 쓰레드가 실행되고 있으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool IsRtpThreadRun()
{
	return gbRtpThreadRun;
}
