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
		printf( "[Usage] %s {sip server ip} {user id} {password} {local port} {tcp}\n", argv[0] );
		return 0;
	}

	char * pszServerIp = argv[1];
	char * pszUserId = argv[2];
	char * pszPassWord = argv[3];
	int iLocalPort = 10000;
	ESipTransport eTransport = E_SIP_UDP;

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
	}
	
	CSipUserAgent clsUserAgent;
	CSipServerInfo clsServerInfo;
	CSipStackSetup clsSetup;
	CSipClient clsSipClient;

	clsServerInfo.m_strIp = pszServerIp;
	clsServerInfo.m_strUserId = pszUserId;
	clsServerInfo.m_strPassWord = pszPassWord;
	clsServerInfo.m_eTransport = eTransport;

	// N���� IP�ּҸ� ����ϴ� ȣ��Ʈ������ SIP �������ݷ� ����� IP�ּҸ� ���� �Է��� �ּ���.
	// Vmware ���� ����ϴ� ��� N���� IP�ּҰ� ȣ��Ʈ�� �����մϴ�.
	GetLocalIp( clsSetup.m_strLocalIp );

	// Ŭ���̾�Ʈ�� SIP ��ſ� ����� ��Ʈ ��ȣ�� �־� �ּ���.
	clsSetup.m_iLocalUdpPort = iLocalPort;

	if( eTransport == E_SIP_TCP )
	{
		clsSetup.m_iLocalTcpPort = SIP_TCP_PORT;
	}

	// UDP ���� �������� �⺻ ������ 1���̴�. �̸� �����Ϸ��� CSipStackSetup.m_iUdpThreadCount �� �����ϸ� �ȴ�.

	clsUserAgent.InsertRegisterInfo( clsServerInfo );

	clsUserAgent.Start( clsSetup, &clsSipClient );

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

			// QQQ: RTP ���� IP/Port/Codec �� �־� �ּ���.
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

			// QQQ: RTP ���� IP/Port/Codec �� �־� �ּ���.
			clsRtp.m_strIp = clsSetup.m_strLocalIp;
			clsRtp.m_iPort = 2000;
			clsRtp.m_iCodec = 0;

			clsUserAgent.AcceptCall( gstrInviteId.c_str(), &clsRtp );
		}
		else if( szCommand[0] == 'm' )
		{
			CSipCallRoute	clsRoute;

			clsRoute.m_strDestIp = pszServerIp;
			clsRoute.m_iDestPort = 5060;

			clsUserAgent.SendSms( pszUserId, szCommand + 2, "hello", &clsRoute );
		}
	
		memset( szCommand, 0, sizeof(szCommand) );
	}

	if( gstrInviteId.empty() == false )
	{
		clsUserAgent.StopCall( gstrInviteId.c_str() );
	}

	sleep(2);

	clsUserAgent.Stop();
	clsUserAgent.Final();

	return 0;
}
