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

#include "SipClient.h"
#include "SipClientSetup.h"
#include "Log.h"
#include "SipUtility.h"
#include "RtpThread.h"
#include "MemoryDebug.h"

extern std::string	gstrInviteId;

/**
 * @ingroup SipClient
 * @brief SIP 클라이언트
 * @param argc 
 * @param argv 
 * @returns 0 을 리턴한다.
 */
int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {xml file}\n", argv[0] );
		return 0;
	}

	char * pszFileName = argv[1];

	if( gclsSetupFile.Read( pszFileName ) == false )
	{
		printf( "Setup file error\n" );
		return 0;
	}

#ifdef WIN32
#ifdef _DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
	CLog::SetDirectory( "c:\\temp\\sipclient" );
	CLog::SetLevel( LOG_NETWORK | LOG_DEBUG | LOG_INFO );
#endif
#endif
	
	CSipUserAgent clsUserAgent;
	CSipServerInfo clsServerInfo;
	CSipStackSetup clsSetup;
	CSipClient clsSipClient;

	clsServerInfo.m_strIp = gclsSetupFile.m_strSipServerIp;
	clsServerInfo.m_strDomain = gclsSetupFile.m_strSipDomain;
	clsServerInfo.m_strUserId = gclsSetupFile.m_strSipUserId;
	clsServerInfo.m_strPassWord = gclsSetupFile.m_strSipPassWord;
	clsServerInfo.m_eTransport = gclsSetupFile.m_eSipTransport;
	clsServerInfo.m_iPort = gclsSetupFile.m_iSipServerPort;
	clsServerInfo.m_iLoginTimeout = 600;
	//clsServerInfo.m_iNatTimeout = 10;

	// 삼성 070 서비스 로그인시 User-Agent 헤더에 특수한 문자열이 포함되지 않으면 480 응답이 수신된다.
	// 아래와 같이 Acrobits 으로 User-Agent 헤더가 시작하면 정상적으로 401 응답을 수신한다.
	//clsSetup.m_strUserAgent = "Acrobits";

	// Expires 헤더에 300 을 입력하고 싶으면 아래와 같이 설정하면 된다.
	// clsServerInfo.m_iLoginTimeout = 300;

	clsSetup.m_iLocalUdpPort = gclsSetupFile.m_iUdpPort;
	clsSetup.m_strLocalIp = gclsSetupFile.m_strLocalIp;

	if( gclsSetupFile.m_eSipTransport == E_SIP_TCP )
	{
		clsSetup.m_iLocalTcpPort = gclsSetupFile.m_iUdpPort;
		clsSetup.m_iTcpCallBackThreadCount = 1;
	}
	else if( gclsSetupFile.m_eSipTransport == E_SIP_TLS )
	{
		clsSetup.m_iLocalTlsPort = gclsSetupFile.m_iUdpPort;
		clsSetup.m_strCertFile = gclsSetupFile.m_strPemFile;
	}

	// Via 헤더 및 Contact 헤더에 로컬 수신 포트 번호를 설정하고 싶으면 아래와 같이 설정하면 된다.
	// clsSetup.m_bUseContactListenPort = true;

	// UDP 수신 쓰레드의 기본 개수는 1개이다. 이를 수정하려면 CSipStackSetup.m_iUdpThreadCount 를 수정하면 된다.

	clsUserAgent.InsertRegisterInfo( clsServerInfo );

	if( clsUserAgent.Start( clsSetup, &clsSipClient ) == false )
	{
		printf( "sip stack start error\n" );
		return 0;
	}

	if( gclsRtpThread.Create() == false )
	{
		printf( "rtp thread create error\n" );
		return 0;
	}

	char	szCommand[1024];
	int		iLen;

	memset( szCommand, 0, sizeof(szCommand) );
	while( fgets( szCommand, sizeof(szCommand), stdin ) )
	{
		if( szCommand[0] == 'Q' || szCommand[0] == 'q' ) break;

		iLen = (int)strlen(szCommand);
		if( iLen >= 2 && szCommand[iLen-2] == '\r' )
		{
			szCommand[iLen-2] = '\0';
		}
		else if( iLen >= 1 && szCommand[iLen-1] == '\n' )
		{
			szCommand[iLen-1] = '\0';
		}

		if( szCommand[0] == 'C' || szCommand[0] == 'c' )
		{
			CSipCallRtp clsRtp;
			CSipCallRoute	clsRoute;

			clsRtp.m_strIp = clsSetup.m_strLocalIp;
			clsRtp.m_iPort = gclsRtpThread.m_iPort;
			clsRtp.m_iCodec = 0;

			clsRoute.m_strDestIp = gclsSetupFile.m_strSipServerIp;
			clsRoute.m_iDestPort = gclsSetupFile.m_iSipServerPort;
			clsRoute.m_eTransport = gclsSetupFile.m_eSipTransport;

			clsUserAgent.StartCall( gclsSetupFile.m_strSipUserId.c_str(), szCommand + 2, &clsRtp, &clsRoute, gstrInviteId );
		}
		else if( szCommand[0] == 'e' || szCommand[0] == 's' )
		{
			clsUserAgent.StopCall( gstrInviteId.c_str() );
			gclsRtpThread.Stop( );
			gstrInviteId.clear();
		}
		else if( szCommand[0] == 'a' )
		{
			CSipCallRtp clsRtp;

			clsRtp.m_strIp = clsSetup.m_strLocalIp;
			clsRtp.m_iPort = gclsRtpThread.m_iPort;
			clsRtp.m_iCodec = 0;

			clsUserAgent.AcceptCall( gstrInviteId.c_str(), &clsRtp );
			gclsRtpThread.Start( clsSipClient.m_clsDestRtp.m_strIp.c_str(), clsSipClient.m_clsDestRtp.m_iPort );
		}
		else if( szCommand[0] == 'm' )
		{
			CSipCallRoute	clsRoute;

			clsRoute.m_strDestIp = gclsSetupFile.m_strSipServerIp;
			clsRoute.m_iDestPort = gclsSetupFile.m_iSipServerPort;
			clsRoute.m_eTransport = gclsSetupFile.m_eSipTransport;

			clsUserAgent.SendSms( gclsSetupFile.m_strSipUserId.c_str(), szCommand + 2, "hello", &clsRoute );
		}
		else if( szCommand[0] == 'i' )
		{
			CMonitorString strBuf;

			clsUserAgent.m_clsSipStack.GetString( strBuf );

			printf( "%s", strBuf.GetString() );
		}
		else if( szCommand[0] == 't' )
		{
			// OPTION 메시지 전송 예제
			CSipMessage * pclsMessage = new CSipMessage();
			if( pclsMessage )
			{
				char	szTag[SIP_TAG_MAX_SIZE], szCallIdName[SIP_CALL_ID_NAME_MAX_SIZE];

				SipMakeTag( szTag, sizeof(szTag) );
				SipMakeCallIdName( szCallIdName, sizeof(szCallIdName) );

				// Call-ID 를 저장한다.
				pclsMessage->m_clsCallId.m_strName = szCallIdName;
				pclsMessage->m_clsCallId.m_strHost = clsUserAgent.m_clsSipStack.m_clsSetup.m_strLocalIp;

				pclsMessage->m_eTransport = E_SIP_UDP;

				// SIP method 를 저장한다.
				pclsMessage->m_strSipMethod = SIP_METHOD_OPTIONS;

				// Request Uri 를 저장한다.
				pclsMessage->m_clsReqUri.Set( SIP_PROTOCOL, "1000", "192.168.0.1", 5060 );

				// CSeq 를 저장한다.
				pclsMessage->m_clsCSeq.Set( 1, SIP_METHOD_OPTIONS );

				// From 헤더를 저장한다.
				pclsMessage->m_clsFrom.m_clsUri.Set( SIP_PROTOCOL, "2000", "192.168.0.200", 5060 );
				pclsMessage->m_clsFrom.InsertParam( SIP_TAG, szTag );

				// To 헤더를 저장한다.
				pclsMessage->m_clsTo.m_clsUri.Set( SIP_PROTOCOL, "1000", "192.168.0.1", 5060 );

				// SIP 메시지를 전송할 대상 IP 주소와 포트 번호 및 프로토콜을 저장한다.
				pclsMessage->AddRoute( "192.168.0.1", 5060, E_SIP_UDP );

				clsUserAgent.m_clsSipStack.SendSipMessage( pclsMessage );
			}
		}
	
		memset( szCommand, 0, sizeof(szCommand) );
	}

	if( gstrInviteId.empty() == false )
	{
		clsUserAgent.StopCall( gstrInviteId.c_str() );
	}

	gclsRtpThread.Stop( );

	sleep(2);

	clsUserAgent.Stop();
	clsUserAgent.Final();

	return 0;
}
