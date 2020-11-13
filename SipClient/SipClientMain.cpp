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
#include "Log.h"
#include "SipUtility.h"
#include "RtpThread.h"
#include "MemoryDebug.h"

extern std::string	gstrInviteId;

/**
 * @ingroup SipClient
 * @brief 
 * @param argc 
 * @param argv 
 * @returns 
 */
int main( int argc, char * argv[] )
{
	if( argc < 4 )
	{
		printf( "[Usage] %s {sip server ip} {user id} {password} {local port} {tcp|tls} {domain}\n", argv[0] );
		return 0;
	}

	char * pszServerIp = argv[1];
	char * pszUserId = argv[2];
	char * pszPassWord = argv[3];
	char * pszDomain = NULL;
	int iLocalPort = 10000;
	ESipTransport eTransport = E_SIP_UDP;
	int iServerPort = SIP_UDP_PORT;

	if( argc >= 5 )
	{
		iLocalPort = atoi( argv[4] );
	}

	if( argc >= 6 )
	{
		if( !strcasecmp( argv[5], "tcp" ) )
		{
			eTransport = E_SIP_TCP;
		}
		else if( !strcasecmp( argv[5], "tls" ) )
		{
#ifndef USE_TLS
			printf( "TLS function is not supported. please compile with USE_TLS define\n" );
			return 0;
#endif
			eTransport = E_SIP_TLS;
			iServerPort = SIP_TLS_PORT;
		}
	}

	if( argc >= 7 )
	{
		pszDomain = argv[6];
	}

#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
	CLog::SetDirectory( "c:\\temp\\sipclient" );
	CLog::SetLevel( LOG_NETWORK | LOG_DEBUG | LOG_INFO );
#endif
	
	CSipUserAgent clsUserAgent;
	CSipServerInfo clsServerInfo;
	CSipStackSetup clsSetup;
	CSipClient clsSipClient;

	clsServerInfo.m_strIp = pszServerIp;
	clsServerInfo.m_strUserId = pszUserId;
	clsServerInfo.m_strPassWord = pszPassWord;
	clsServerInfo.m_eTransport = eTransport;
	clsServerInfo.m_iPort = iServerPort;
	clsServerInfo.m_iLoginTimeout = 600;
	//clsServerInfo.m_iNatTimeout = 10;

	if( pszDomain )
	{
		clsServerInfo.m_strDomain = pszDomain;
	}

	// 삼성 070 서비스 로그인시 User-Agent 헤더에 특수한 문자열이 포함되지 않으면 480 응답이 수신된다.
	// 아래와 같이 Acrobits 으로 User-Agent 헤더가 시작하면 정상적으로 401 응답을 수신한다.
	//clsSetup.m_strUserAgent = "Acrobits";

	// Expires 헤더에 300 을 입력하고 싶으면 아래와 같이 설정하면 된다.
	// clsServerInfo.m_iLoginTimeout = 300;

	// N개의 IP주소를 사용하는 호스트에서는 SIP 프로토콜로 사용할 IP주소를 직접 입력해 주세요.
	// Vmware 등을 사용하는 경우 N개의 IP주소가 호스트에 존재합니다.
	GetLocalIp( clsSetup.m_strLocalIp );

	// 클라이언트가 SIP 통신에 사용할 포트 번호를 넣어 주세요.
	clsSetup.m_iLocalUdpPort = iLocalPort;

	if( eTransport == E_SIP_TCP )
	{
		clsSetup.m_iLocalTcpPort = iLocalPort;
		clsSetup.m_iTcpCallBackThreadCount = 2;
	}
	else if( eTransport == E_SIP_TLS )
	{
		clsSetup.m_iLocalTlsPort = iLocalPort;
		clsSetup.m_strCertFile = "C:\\OpenProject\\CppSipStack\\trunk\\SipClient\\SipServer.pem";
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

			// QQQ: RTP 수신 IP/Port/Codec 를 넣어 주세요.
			clsRtp.m_strIp = clsSetup.m_strLocalIp;
			clsRtp.m_iPort = gclsRtpThread.m_iPort;
			clsRtp.m_iCodec = 0;

			clsRoute.m_strDestIp = pszServerIp;
			clsRoute.m_iDestPort = iServerPort;
			clsRoute.m_eTransport = eTransport;

			clsUserAgent.StartCall( pszUserId, szCommand + 2, &clsRtp, &clsRoute, gstrInviteId );
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

			// QQQ: RTP 수신 IP/Port/Codec 를 넣어 주세요.
			clsRtp.m_strIp = clsSetup.m_strLocalIp;
			clsRtp.m_iPort = gclsRtpThread.m_iPort;
			clsRtp.m_iCodec = 0;

			clsUserAgent.AcceptCall( gstrInviteId.c_str(), &clsRtp );
			gclsRtpThread.Start( clsSipClient.m_clsDestRtp.m_strIp.c_str(), clsSipClient.m_clsDestRtp.m_iPort );
		}
		else if( szCommand[0] == 'm' )
		{
			CSipCallRoute	clsRoute;

			clsRoute.m_strDestIp = pszServerIp;
			clsRoute.m_iDestPort = 5060;
			clsRoute.m_eTransport = eTransport;

			clsUserAgent.SendSms( pszUserId, szCommand + 2, "hello", &clsRoute );
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
