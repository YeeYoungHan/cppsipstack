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

#include "SipPlatformDefine.h"
#include "ServerUtility.h"
#include "StunMessage.h"
#include "SipCallBack.h"
#include "HttpCallBack.h"
#include "RtpThread.h"
#include "UserMap.h"
#include "CallMap.h"

static int giUdpPort = 10000;

bool CreateUdpSocket( Socket & hSocket, int & iPort )
{
	int iStartPort = giUdpPort;

	while( giUdpPort < 65535 )
	{
		hSocket = UdpListen( giUdpPort, NULL );
		if( hSocket != INVALID_SOCKET ) 
		{
			iPort = giUdpPort;
			giUdpPort += 2;
			return true;
		}
		giUdpPort += 2;
	}

	for( giUdpPort = 10000; giUdpPort < iStartPort; giUdpPort += 2 )
	{
		hSocket = UdpListen( giUdpPort, NULL );
		if( hSocket != INVALID_SOCKET )
		{
			iPort = giUdpPort;
			giUdpPort += 2;
			return true;
		}
	}

	return false;
}

bool CRtpThreadArg::CreateSocket()
{
	if( CreateUdpSocket( m_hWebRtcUdp, m_iWebRtcUdpPort ) && CreateUdpSocket( m_hPbxUdp, m_iPbxUdpPort ) )
	{
		return true;
	}

	Close();

	return false;
}

void CRtpThreadArg::Close()
{
	if( m_hWebRtcUdp != INVALID_SOCKET )
	{
		closesocket( m_hWebRtcUdp );
		m_hWebRtcUdp = INVALID_SOCKET;
	}

	if( m_hPbxUdp != INVALID_SOCKET )
	{
		closesocket( m_hPbxUdp );
		m_hPbxUdp = INVALID_SOCKET;
	}

	m_iWebRtcUdpPort = 0;
	m_iPbxUdpPort = 0;
}

THREAD_API RtpThread( LPVOID lpParameter )
{
	CRtpThreadArg * pclsArg = (CRtpThreadArg *)lpParameter;
	CUserInfo clsUserInfo;

	char szSdp[8192], szPacket[1024], szIp[21];
	const char * pszIcePwd = "FNPRfT4qUaVOKa0ivkn64mMY";
	unsigned short iPort;
	int iPacketLen;
	CStunMessage clsStunRequest;
	std::string strCallId;
	CSipCallRtp clsRtp;
	CSipCallRoute clsRoute;

	if( gclsUserMap.Select( pclsArg->m_strUserId.c_str(), clsUserInfo ) == false )
	{
		goto FUNC_END;
	}

	snprintf( szSdp, sizeof(szSdp), "v=0\r\n"
		"o=- 4532014611503881976 0 IN IP4 %s\r\n"
		"s=-\r\n"
		"t=0 0\r\n"
		"m=audio %d UDP/TLS/RTP/SAVPF 0\r\n"
		"c=IN IP4 %s\r\n"
		"a=rtpmap:0 PCMU/8000\r\n"
		"a=sendrecv\r\n"
		"a=ice-ufrag:lMRb\r\n"
		"a=ice-pwd:%s\r\n"
		"a=fingerprint:sha-256 0D:F6:43:E7:2D:94:11:47:47:84:A4:E4:AF:42:34:A4:B1:B9:58:AB:A9:BF:82:37:7A:73:C4:04:F0:62:7C:36\r\n"
		"a=candidate:1 1 udp 2130706431 %s %d typ host\r\n"
		, gstrLocalIp.c_str(), pclsArg->m_iWebRtcUdpPort, gstrLocalIp.c_str(), pszIcePwd, gstrLocalIp.c_str(), pclsArg->m_iWebRtcUdpPort );
	gclsHttpCallBack.Send( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort, "res|invite|180|%s", szSdp );

	iPacketLen = sizeof(szPacket);
	UdpRecv( pclsArg->m_hWebRtcUdp, szPacket, &iPacketLen, szIp, sizeof(szIp), &iPort );

	if( iPacketLen >= 20 && szPacket[0] == 0x00 && szPacket[1] == 0x01 )
	{
		if( clsStunRequest.Parse( szPacket, iPacketLen ) == -1 )
		{
			gclsHttpCallBack.Send( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort, "res|invite|500" );
			goto FUNC_END;
		}

		// QQQ: STUN 응답 메시지 생성 및 전송
	}

	// QQQ: DTLS 설정

	clsRoute.m_strDestIp = clsUserInfo.m_strSipServerIp;
	clsRoute.m_iDestPort = 5060;

	if( gclsSipStack.StartCall( pclsArg->m_strUserId.c_str(), pclsArg->m_strToId.c_str(), &clsRtp, &clsRoute, strCallId ) == false )
	{
		gclsHttpCallBack.Send( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort, "res|invite|500" );
		goto FUNC_END;
	}

	if( gclsCallMap.Insert( strCallId.c_str(), pclsArg->m_strUserId.c_str() ) == false )
	{
		gclsSipStack.StopCall( strCallId.c_str() );
		gclsHttpCallBack.Send( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort, "res|invite|500" );
		goto FUNC_END;
	}

	while( pclsArg->m_bStop == false )
	{
		
	}

FUNC_END:
	gclsUserMap.Update( pclsArg->m_strUserId.c_str(), pclsArg, true );
	delete pclsArg;

	return 0;
}

bool StartRtpThread( CRtpThreadArg * pclsArg )
{
	return StartThread( "RtpThread", RtpThread, pclsArg );
}
