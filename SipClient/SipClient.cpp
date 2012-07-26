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

void CSipClient::EventRegister( CSipServerInfo clsInfo, int iStatus )
{
	printf( "REGISTER(%s) : %d\n", clsInfo.m_strUserId.c_str(), iStatus );
}

void CSipClient::SipLog( bool bSend, const char * pszPacket )
{
	struct timeval sttTime;
	struct tm	sttTm;

	gettimeofday( &sttTime, NULL );

#ifdef WIN32
	_localtime32_s( &sttTm, &sttTime.tv_sec );
#else
	localtime_r( &sttTime.tv_sec, &sttTm );	
#endif

	printf( "[%02d:%02d:%02d.%03d] %s [%s]\n", sttTm.tm_hour, sttTm.tm_min, sttTm.tm_sec, sttTime.tv_usec / 1000, bSend ? "Send" : "Recv", pszPacket );
}

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

	// QQQ: 클라이언트가 현재 사용하는 IP 주소를 넣어 주세요.
	clsSetup.m_strLocalIp = "127.0.0.1";

	clsUserAgent.m_pclsCallBack = &clsSipClient;
	clsUserAgent.AddRegisterInfo( clsServerInfo );
	gclsSipStack.AddNetworkLog( &clsSipClient );

	clsUserAgent.Start( clsSetup );

	while( 1 )
	{
		sleep(1);
	}

	return 0;
}
