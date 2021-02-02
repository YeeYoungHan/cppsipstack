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
#include "HttpCallBack.h"
#include "RtpThread.h"
#include "UserMap.h"
#include "srtp.h"
#include "Server.h"
#include "MemoryDebug.h"

#include "RtpThreadArg.hpp"
#include "RtpThreadDtls.hpp"

/**
 * @ingroup TestWebRtc
 * @brief SDP ���� ICE ����� ���̵� �� ICE ��й�ȣ�� �����´�.
 * @param pszSdp			SDP
 * @param strIceUser	ICE ����� ���̵�
 * @param strIcePwd		ICE ��й�ȣ
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool GetIceUserPwd( const char * pszSdp, std::string & strIceUser, std::string & strIcePwd )
{
	CSdpMessage clsSdp;
	SDP_MEDIA_LIST::iterator itML;
	SDP_ATTRIBUTE_LIST::iterator itAL;

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

/**
 * @ingroup TestWebRtc
 * @brief RTP ����/���� ������
 * @param lpParameter 
 * @returns 0 �� �����Ѵ�.
 */
THREAD_API RtpThread( LPVOID lpParameter )
{
	CRtpThreadArg * pclsArg = (CRtpThreadArg *)lpParameter;
	CUserInfo clsUserInfo;

	char szSdp[8192], szPacket[1024], szWebRTCIp[21], szPbxIp[21];
	unsigned short iWebRTCPort = 0, iPbxPort = 0;
	int iPacketLen, n;
	CStunMessage clsStunRequest;
	std::string strIceUser, strIcePwd;
	pollfd sttPoll[2];
	srtp_t psttSrtpTx = NULL, psttSrtpRx = NULL;
	bool bDtls = false, bSendRtpToWebRTC = false, bEdge = false;

	szWebRTCIp[0] = '\0';
	szPbxIp[0] = '\0';

	if( gclsUserMap.Select( pclsArg->m_strUserId.c_str(), clsUserInfo ) == false )
	{
		goto FUNC_END;
	}

	if( strstr( clsUserInfo.m_strUserAgent.c_str(), "Edge" ) )
	{
		bEdge = true;
	}

	GetIceUserPwd( pclsArg->m_strSdp.c_str(), strIceUser, strIcePwd );
	strIceUser.append( ":lMRb" );

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
		"a=fingerprint:sha-256 %s\r\n"
		"a=candidate:1 1 udp 2130706431 %s %d typ host\r\n"
		"a=ssrc:100 msid:1234 1234\r\n"
		"a=rtcp-mux\r\n"
		, gstrLocalIp.c_str(), pclsArg->m_iWebRtcUdpPort, gstrLocalIp.c_str(), pclsArg->m_strIcePwd.c_str(), gclsKeyCert.m_strFingerPrint.c_str(), gstrLocalIp.c_str(), pclsArg->m_iWebRtcUdpPort );
	gclsHttpCallBack.Send( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort, "res|invite|180|%s", szSdp );

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

			if( iPacketLen >= 20 && szPacket[0] == 0x00 && szPacket[1] == 0x01 )
			{
				if( clsStunRequest.Parse( szPacket, iPacketLen ) == -1 )
				{
					continue;
				}

				// STUN ���� �޽��� ���� �� ����
				CStunMessage * pclsStunResponse = clsStunRequest.CreateResponse( true );
				if( pclsStunResponse == NULL )
				{
					continue;
				}

				pclsStunResponse->m_strPassword = pclsArg->m_strIcePwd;
				pclsStunResponse->AddXorMappedAddress( szWebRTCIp, iWebRTCPort );
				pclsStunResponse->AddMessageIntegrity();
				pclsStunResponse->AddFingerPrint();
				iPacketLen = pclsStunResponse->ToString( szPacket, sizeof(szPacket) );
				delete pclsStunResponse;

				UdpSend( pclsArg->m_hWebRtcUdp, szPacket, iPacketLen, szWebRTCIp, iWebRTCPort );

				// STUN ��û �޽��� ���� �� ����
				clsStunRequest.m_clsAttributeList.clear();
				clsStunRequest.m_strPassword = strIcePwd;
				clsStunRequest.AddUserName( strIceUser.c_str() );
				clsStunRequest.AddMessageIntegrity();
				clsStunRequest.AddFingerPrint();

				iPacketLen = clsStunRequest.ToString( szPacket, sizeof(szPacket) );

				UdpSend( pclsArg->m_hWebRtcUdp, szPacket, iPacketLen, szWebRTCIp, iWebRTCPort );
			}
			else if( iPacketLen >= 20 && szPacket[0] == 0x01 && szPacket[1] == 0x01 )
			{
				if( bDtls == false )
				{
					// DTLS ����
					SSL * psttSsl;
					struct	sockaddr_in	addr;

					addr.sin_family = AF_INET;
					addr.sin_port   = htons(iWebRTCPort);

					inet_pton( AF_INET, szWebRTCIp, &addr.sin_addr.s_addr );
					if( connect( pclsArg->m_hWebRtcUdp, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR )
					{
						printf( "connect error\n" );
					}
					else if( (psttSsl = SSL_new( gpsttClientCtx ) ) == NULL )
					{
						printf( "ssl_new error\n" );
					}
					else
					{
						// QQQ: ���⼭ 12, 400 byte �޸� ������ �߻��ϴ� �� ����.
						SSL_set_fd( psttSsl, (int)pclsArg->m_hWebRtcUdp );
						SSL_set_tlsext_use_srtp( psttSsl, "SRTP_AES128_CM_SHA1_80" );

						if( bEdge )
						{
							// MS Edge �������� �����ϱ� ���� �ɼ�
							SSL_set_cipher_list( psttSsl, "ECDHE-RSA-AES256-GCM-SHA384" );
						}

						if( SSL_connect( psttSsl ) == -1 )
						{
							printf( "SSL_connect error\n" );
						}
						else
						{
							uint8_t szMaterial[60];
							uint8_t * pszLocalKey, * pszLocalSalt, * pszRemoteKey, * pszRemoteSalt;

							SSL_export_keying_material( psttSsl, szMaterial, sizeof(szMaterial), "EXTRACTOR-dtls_srtp", 19, NULL, 0, 0 );

							pszLocalKey = szMaterial;
							pszRemoteKey = pszLocalKey + 16;
							pszLocalSalt = pszRemoteKey + 16;
							pszRemoteSalt = pszLocalSalt + 14;

							SrtpCreate( &psttSrtpTx, pszLocalKey, pszLocalSalt, false );
							SrtpCreate( &psttSrtpRx, pszRemoteKey, pszRemoteSalt, true );
						}

						SSL_free( psttSsl );
					}

					bDtls = true;
				}
			}
			else
			{
				if( iPbxPort > 0 )
				{
					if( psttSrtpRx )
					{
						srtp_unprotect( psttSrtpRx, szPacket, &iPacketLen );
					}

					UdpSend( pclsArg->m_hPbxUdp, szPacket, iPacketLen, szPbxIp, iPbxPort );
					bSendRtpToWebRTC = true;
				}
			}
		}

		if( sttPoll[1].revents & POLLIN )
		{
			iPacketLen = sizeof(szPacket);
			UdpRecv( pclsArg->m_hPbxUdp, szPacket, &iPacketLen, szPbxIp, sizeof(szPbxIp), &iPbxPort );

			if( bSendRtpToWebRTC )
			{
				int32_t iSsrc = htonl( 100 );
				memcpy( szPacket + 8, &iSsrc, 4 );

				if( psttSrtpTx )
				{
					srtp_protect( psttSrtpTx, szPacket, &iPacketLen );
				}

				UdpSend( pclsArg->m_hWebRtcUdp, szPacket, iPacketLen, szWebRTCIp, iWebRTCPort );
			}
		}
	}

FUNC_END:
	if( psttSrtpTx ) srtp_dealloc( psttSrtpTx );
	if( psttSrtpRx ) srtp_dealloc( psttSrtpRx );

	gclsUserMap.Update( pclsArg->m_strUserId.c_str(), pclsArg, true );
	delete pclsArg;

#if OPENSSL_VERSION_NUMBER < 0x10100000L
	ERR_remove_thread_state( NULL );
#endif

	return 0;
}

/**
 * @ingroup TestWebRtc
 * @brief RTP �����带 �����Ѵ�.
 * @param pclsArg RTP ������ ���� ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool StartRtpThread( CRtpThreadArg * pclsArg )
{
	return StartThread( "RtpThread", RtpThread, pclsArg );
}
