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
#include "SdpMessage.h"
#include "SipCallBack.h"
#include "HttpCallBack.h"
#include "RtpThread.h"
#include "UserMap.h"
#include "CallMap.h"

#include "RtpThreadArg.hpp"

bool GetIceUserPwd( const char * pszSdp, std::string & strIceUser, std::string & strIcePwd )
{
	CSdpMessage clsSdp;
	SDP_MEDIA_LIST::iterator itML;
	SDP_ATTRIBUTE_LIST::iterator itAL;
	bool bFound = false;

	if( clsSdp.Parse( pszSdp, strlen(pszSdp) ) == -1 ) return false;

	for( itML = clsSdp.m_clsMediaList.begin(); itML != clsSdp.m_clsMediaList.end(); ++itML )
	{
		if( !strcmp( itML->m_strMedia.c_str(), "audio" ) )
		{
			for( itAL = itML->m_clsAttributeList.begin(); itAL != itML->m_clsAttributeList.end(); ++itAL )
			{
				if( !strcmp( itAL->m_strName.c_str(), "ice-ufrag" ) )
				{
					strIceUser = itAL->m_strValue;
				}
				else if( !strcmp( itAL->m_strName.c_str(), "ice-pwd" ) )
				{
					strIcePwd = itAL->m_strValue;
					return true;
				}
			}
		}
	}

	return false;
}

THREAD_API RtpThread( LPVOID lpParameter )
{
	CRtpThreadArg * pclsArg = (CRtpThreadArg *)lpParameter;
	CUserInfo clsUserInfo;

	char szSdp[8192], szPacket[1024], szWebRTCIp[21], szPbxIp[21];
	const char * pszIcePwd = "FNPRfT4qUaVOKa0ivkn64mMY";
	unsigned short iWebRTCPort = 0, iPbxPort = 0;
	int iPacketLen, n;
	CStunMessage clsStunRequest;
	std::string strCallId, strIceUser, strIcePwd;
	CSipCallRtp clsRtp;
	CSipCallRoute clsRoute;
	pollfd sttPoll[2];

	szWebRTCIp[0] = '\0';
	szPbxIp[0] = '\0';

	if( gclsUserMap.Select( pclsArg->m_strUserId.c_str(), clsUserInfo ) == false )
	{
		goto FUNC_END;
	}

	GetIceUserPwd( pclsArg->m_strSdp.c_str(), strIceUser, strIcePwd );
	strIceUser.append( ":lMRb" );

	snprintf( szSdp, sizeof(szSdp), "v=0\r\n"
		"o=- 4532014611503881976 0 IN IP4 %s\r\n"
		"s=-\r\n"
		"t=0 0\r\n"
		"m=audio %d RTP/AVP 0\r\n"
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
	UdpRecv( pclsArg->m_hWebRtcUdp, szPacket, &iPacketLen, szWebRTCIp, sizeof(szWebRTCIp), &iWebRTCPort );

	clsRtp.m_iCodec = 0;
	clsRtp.m_iPort = pclsArg->m_iPbxUdpPort;
	clsRtp.m_strIp = gstrLocalIp;

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

	TcpSetPollIn( sttPoll[0], pclsArg->m_hWebRtcUdp );
	TcpSetPollIn( sttPoll[1], pclsArg->m_hPbxUdp );

	while( pclsArg->m_bStop == false )
	{
		n = poll( sttPoll, 2, 1000 );
		if( n <= 0 ) continue;
		
		if( sttPoll[0].revents & POLLIN )
		{
			iPacketLen = sizeof(szPacket);
			UdpRecv( pclsArg->m_hWebRtcUdp, szPacket, &iPacketLen, szWebRTCIp, sizeof(szWebRTCIp), &iWebRTCPort );

			if( (uint8_t)szPacket[0] == 0x80 || (uint8_t)szPacket[0] == 0x81 )
			{
				if( iPbxPort > 0 )
				{
					UdpSend( pclsArg->m_hPbxUdp, szPacket, iPacketLen, szPbxIp, iPbxPort );
				}
			}
			else if( iPacketLen >= 20 && szPacket[0] == 0x00 && szPacket[1] == 0x01 )
			{
				if( clsStunRequest.Parse( szPacket, iPacketLen ) == -1 )
				{
					continue;
				}

				// STUN 응답 메시지 생성 및 전송
				CStunMessage * pclsStunResponse = clsStunRequest.CreateResponse( true );
				if( pclsStunResponse == NULL )
				{
					continue;
				}

				pclsStunResponse->m_strPassword = pszIcePwd;
				pclsStunResponse->AddXorMappedAddress( szWebRTCIp, iWebRTCPort );
				//pclsStunResponse->AddInt( 0x8070, 3 );
				pclsStunResponse->AddMessageIntegrity();
				pclsStunResponse->AddFingerPrint();
				iPacketLen = pclsStunResponse->ToString( szPacket, sizeof(szPacket) );
				delete pclsStunResponse;

				UdpSend( pclsArg->m_hWebRtcUdp, szPacket, iPacketLen, szWebRTCIp, iWebRTCPort );

				// STUN 응답 메시지 생성 및 전송
				clsStunRequest.m_clsAttributeList.clear();
				clsStunRequest.m_strPassword = strIcePwd;
				clsStunRequest.AddUserName( strIceUser.c_str() );
				clsStunRequest.AddMessageIntegrity();
				clsStunRequest.AddFingerPrint();

				iPacketLen = clsStunRequest.ToString( szPacket, sizeof(szPacket) );

				UdpSend( pclsArg->m_hWebRtcUdp, szPacket, iPacketLen, szWebRTCIp, iWebRTCPort );
			}
		}

		if( sttPoll[1].revents & POLLIN )
		{
			iPacketLen = sizeof(szPacket);
			UdpRecv( pclsArg->m_hPbxUdp, szPacket, &iPacketLen, szPbxIp, sizeof(szPbxIp), &iPbxPort );
			UdpSend( pclsArg->m_hPbxUdp, szPacket, iPacketLen, szWebRTCIp, iWebRTCPort );
		}
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
