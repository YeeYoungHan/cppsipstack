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
#include <time.h>

std::string	gstrInviteId;

/**
 * @ingroup SipClient
 * @brief SIP REGISTER 응답 메시지 수신 이벤트 핸들러
 * @param pclsInfo	SIP REGISTER 응답 메시지를 전송한 IP-PBX 정보 저장 객체
 * @param iStatus		SIP REGISTER 응답 코드
 */
void CSipClient::EventRegister( CSipServerInfo * pclsInfo, int iStatus )
{
	printf( "EventRegister(%s) : %d\n", pclsInfo->m_strUserId.c_str(), iStatus );
}

/**
 * @ingroup SipClient
 * @brief SIP 통화 요청 수신에 대한 인증 확인 이벤트 핸들러
 * @param pclsMessage	SIP INVITE 요청 메시지
 * @return 인증에 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipClient::EventIncomingCallAuth( CSipMessage * pclsMessage )
{
	return true;
}

/**
 * @ingroup SipClient
 * @brief SIP 통화 요청 수신 이벤트 핸들러
 * @param	pszCallId	SIP Call-ID
 * @param pszFrom		SIP From 사용자 아이디
 * @param pszTo			SIP To 사용자 아이디
 * @param pclsRtp		RTP 정보 저장 객체
 */
void CSipClient::EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
{
	printf( "EventIncomingCall(%s,%s)\n", pszCallId, pszFrom );

	gstrInviteId = pszCallId;

	if( pclsRtp )
	{
		printf( "=> RTP(%s:%d) codec(%d)\n", pclsRtp->m_strIp.c_str(), pclsRtp->m_iPort, pclsRtp->m_iCodec );
	}
}

/**
 * @ingroup SipClient
 * @brief SIP Ring / Session Progress 수신 이벤트 핸들러
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP 응답 코드
 * @param pclsRtp			RTP 정보 저장 객체
 */
void CSipClient::EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{
	printf( "EventCallRing(%s,%d)\n", pszCallId, iSipStatus );

	if( pclsRtp )
	{
		printf( "=> RTP(%s:%d) codec(%d)\n", pclsRtp->m_strIp.c_str(), pclsRtp->m_iPort, pclsRtp->m_iCodec );
	}
}

/**
 * @ingroup SipClient
 * @brief SIP 통화 연결 이벤트 핸들러
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP 정보 저장 객체
 */
void CSipClient::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	printf( "EventCallStart(%s)\n", pszCallId );

	if( pclsRtp )
	{
		printf( "=> RTP(%s:%d) codec(%d)\n", pclsRtp->m_strIp.c_str(), pclsRtp->m_iPort, pclsRtp->m_iCodec );
	}
}

/**
 * @ingroup SipClient
 * @brief SIP 통화 종료 이벤트 핸들러
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP 응답 코드. INVITE 에 대한 오류 응답으로 전화가 종료된 경우, INVITE 의 응답 코드를 저장한다.
 */
void CSipClient::EventCallEnd( const char * pszCallId, int iSipStatus )
{
	printf( "EventCallEnd(%s,%d)\n", pszCallId, iSipStatus );
}

/**
 * @ingroup SipClient
 * @brief SIP ReINVITE 수신 이벤트 핸들러
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP 정보 저장 객체
 */
void CSipClient::EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	printf( "EventReInvite(%s)\n", pszCallId );

	if( pclsRtp )
	{
		printf( "=> RTP(%s:%d) codec(%d)\n", pclsRtp->m_strIp.c_str(), pclsRtp->m_iPort, pclsRtp->m_iCodec );
	}
}

void CSipClient::SipLog( bool bSend, const char * pszPacket, const char * pszIp, int iPort )
{
	struct timeval sttTime;
	struct tm	sttTm;

	gettimeofday( &sttTime, NULL );

#ifdef WIN32
	_localtime32_s( &sttTm, &sttTime.tv_sec );
#else
	localtime_r( &sttTime.tv_sec, &sttTm );	
#endif

	printf( "[%02d:%02d:%02d.%03d] %s [%s]\n", sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec, (int)(sttTime.tv_usec / 1000), bSend ? "Send" : "Recv", pszPacket );
}

/**
 * @ingroup SipClient
 * @brief 
 * @param argc 
 * @param argv 
 * @returns 
 */
int main( int argc, char * argv[] )
{
	if( argc != 4 )
	{
		printf( "[Usage] %s {sip server ip} {user id} {password}\n", argv[0] );
		return 0;
	}

	char * pszServerIp = argv[1];
	char * pszUserId = argv[2];
	char * pszPassWord = argv[3];
	
	CSipUserAgent clsUserAgent;
	CSipServerInfo clsServerInfo;
	CSipStackSetup clsSetup;
	CSipClient clsSipClient;

	clsServerInfo.m_strIp = pszServerIp;
	clsServerInfo.m_strUserId = pszUserId;
	clsServerInfo.m_strPassWord = pszPassWord;

	// N개의 IP주소를 사용하는 호스트에서는 SIP 프로토콜로 사용할 IP주소를 직접 입력해 주세요.
	// Vmware 등을 사용하는 경우 N개의 IP주소가 호스트에 존재합니다.
	GetLocalIp( clsSetup.m_strLocalIp );

	// 클라이언트가 SIP 통신에 사용할 포트 번호를 넣어 주세요.
	clsSetup.m_iLocalUdpPort = 10000;

	// UDP 수신 쓰레드의 기본 개수는 1개이다. 이를 수정하려면 CSipStackSetup.m_iUdpThreadCount 를 수정하면 된다.

	clsUserAgent.m_pclsCallBack = &clsSipClient;
	clsUserAgent.AddRegisterInfo( clsServerInfo );
	gclsSipStack.AddNetworkLog( &clsSipClient );

	clsUserAgent.Start( clsSetup );

	char	szCommand[1024];
	int		iLen;

	memset( szCommand, 0, sizeof(szCommand) );
	while( fgets( szCommand, sizeof(szCommand), stdin ) )
	{
		if( szCommand[0] == 'Q' || szCommand[0] == 'q' ) break;

		iLen = strlen(szCommand);
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
			clsRtp.m_iPort = 2000;
			clsRtp.m_iCodec = 0;

			clsRoute.m_strDestIp = pszServerIp;
			clsRoute.m_iDestPort = 5060;

			clsUserAgent.StartCall( pszUserId, szCommand + 2, &clsRtp, &clsRoute, gstrInviteId );
		}
		else if( szCommand[0] == 'e' || szCommand[0] == 's' )
		{
			clsUserAgent.StopCall( gstrInviteId.c_str() );
			gstrInviteId.clear();
		}
		else if( szCommand[0] == 'a' )
		{
			CSipCallRtp clsRtp;

			// QQQ: RTP 수신 IP/Port/Codec 를 넣어 주세요.
			clsRtp.m_strIp = clsSetup.m_strLocalIp;
			clsRtp.m_iPort = 2000;
			clsRtp.m_iCodec = 0;

			clsUserAgent.AcceptCall( gstrInviteId.c_str(), &clsRtp );
		}
	
		memset( szCommand, 0, sizeof(szCommand) );
	}

	if( gstrInviteId.empty() == false )
	{
		clsUserAgent.StopCall( gstrInviteId.c_str() );
	}

	return 0;
}
