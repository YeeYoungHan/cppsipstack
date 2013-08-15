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

#include "SipServer.h"
#include "SipServerMap.h"
#include "SipServerSetup.h"
#include "SipLoadBalancer.h"
#include "SipLoadBalancerVersion.h"
#include "ServerThread.h"
#include "ServerUtility.h"
#include "Log.h"
#include <signal.h>

bool gbStop = false;
static std::string gstrConfigFileName;

/**
 * @ingroup SipLoadBalancer
 * @brief signal function
 * @param sig ��ȣ ��ȣ
 */
void LastMethod( int sig )
{
	char	szText[21];

	szText[0] = '\0';
	switch( sig )
	{
	case SIGINT:
		snprintf( szText, sizeof(szText), "SIGINT" );
		break;
	case SIGSEGV:
		snprintf( szText, sizeof(szText), "SIGSEGV" );
		break;
	case SIGTERM:
		snprintf( szText, sizeof(szText), "SIGTERM" );
		break;
#ifndef WIN32
	case SIGQUIT:
		snprintf( szText, sizeof(szText), "SIGQUIT" );
		break;
#endif
	case SIGABRT:
		snprintf( szText, sizeof(szText), "SIGABRT" );
		break;
	}
	CLog::Print( LOG_ERROR, "signal%s%s(%d) is received. terminated", strlen(szText) > 0 ? "-" : "", szText, sig );

	gbStop = true;
}

/**
 * @ingroup SipLoadBalancer
 * @brief C++ SIP stack �� �̿��� �ѱ��� IP-PBX
 * @returns ���� �����ϸ� 0 �� �����ϰ� ������ �߻��ϸ� -1 �� �����Ѵ�.
 */
int ServerMain( )
{
	if( gclsSetup.Read( gstrConfigFileName.c_str() ) == false )
	{
		printf( "config filename(%s) read error\n", gstrConfigFileName.c_str() );
		return -1;
	}

	CLog::SetDirectory( gclsSetup.m_strLogFolder.c_str() );
	CLog::SetLevel( gclsSetup.m_iLogLevel );

	CLog::Print( LOG_SYSTEM, "SipLoadBalancer is started ( version-%s %s %s )", SIP_LOAD_BALANCER_VERSION, __DATE__, __TIME__ );

	CSipStackSetup clsSetup;

	if( gclsSetup.m_strLocalIp.empty() )
	{
		// N���� IP�ּҸ� ����ϴ� ȣ��Ʈ������ SIP �������ݷ� ����� IP�ּҸ� ���� �Է��� �ּ���.
		// Vmware ���� ����ϴ� ��� N���� IP�ּҰ� ȣ��Ʈ�� �����մϴ�.
		GetLocalIp( clsSetup.m_strLocalIp );
		gclsSetup.m_strLocalIp = clsSetup.m_strLocalIp;
	}
	else
	{
		clsSetup.m_strLocalIp = gclsSetup.m_strLocalIp;
	}

	clsSetup.m_iLocalUdpPort = gclsSetup.m_iUdpPort;
	clsSetup.m_iUdpThreadCount = gclsSetup.m_iUdpThreadCount;

	clsSetup.m_iLocalTcpPort = gclsSetup.m_iTcpPort;
	clsSetup.m_iTcpThreadCount = gclsSetup.m_iTcpThreadCount;

	clsSetup.m_iLocalTlsPort = gclsSetup.m_iTlsPort;
	clsSetup.m_iTlsAcceptTimeout = gclsSetup.m_iTlsAcceptTimeout;
	clsSetup.m_strCertFile = gclsSetup.m_strCertFile;
	
	clsSetup.m_strUserAgent = "SipLoadBalancer_";
	clsSetup.m_strUserAgent.append( SIP_LOAD_BALANCER_VERSION );

	clsSetup.m_clsDenySipUserAgentMap = gclsSetup.m_clsDenySipUserAgentMap;

	Fork( true );
	SetCoreDumpEnable();

	signal( SIGINT, LastMethod );
	signal( SIGTERM, LastMethod );
	signal( SIGABRT, LastMethod );
#ifndef WIN32
	signal( SIGKILL, LastMethod );
	signal( SIGQUIT, LastMethod );
	signal( SIGPIPE, SIG_IGN );
#endif

	if( gclsSipServer.Start( clsSetup ) == false )
	{
		CLog::Print( LOG_ERROR, "SipServer start error\n" );
		return -1;
	}

	if( gclsSetup.m_iMonitorPort > 0 )
	{
		StartServerThread();
	}

	int iSecond = 0;

	while( gbStop == false )
	{
		sleep(1);
		++iSecond;

		if( iSecond == 3600 )
		{
			iSecond = 0;
		}

		// QQQ: ���� ���� ���� ���θ� �˻��Ѵ�.
	}

	CLog::Print( LOG_SYSTEM, "SipLoadBalancer is terminated" );

	return 0;
}


/**
 * @ingroup SipLoadBalancer
 * @brief C++ SIP stack �� �̿��� SIP �ε�뷱��
 * @param argc 
 * @param argv 
 * @returns ���� �����ϸ� 0 �� �����ϰ� ������ �߻��ϸ� -1 �� �����Ѵ�.
 */
int main( int argc, char * argv[] )
{
#ifdef WIN32
	if( argc == 1 )
	{
		gstrConfigFileName = GetConfigFileName();
		if( IsExistFile( gstrConfigFileName.c_str() ) == false )
		{
			printf( "setup file(%s) is not exist", gstrConfigFileName.c_str() );
			return -1;
		}

		ServiceStart();
		return 0;
	}
#endif
	 
	if( argc != 2 )
	{
		printf( "[Usage] %s {config filename}\n", argv[0] );
		return -1;
	}

	gstrConfigFileName = argv[1];
	if( !strcmp( gstrConfigFileName.c_str(), "-h" ) || !strcmp( gstrConfigFileName.c_str(), "-v" ) )
	{
		printf( "%s version-%s ( build %s %s )\n", argv[0], SIP_LOAD_BALANCER_VERSION, __DATE__, __TIME__ );
		printf( "[Usage] %s {config filename}\n", argv[0] );
#ifdef WIN32
		printf( "        %s -i : install service\n", argv[0] );
		printf( "        %s -u : uninstall service\n", argv[0] );
#endif
		return 0;
	}
#ifdef WIN32
	if( !strcmp( gstrConfigFileName.c_str(), "-i" ) )
	{
		InstallService();
		return 0;
	}
	else if( !strcmp( gstrConfigFileName.c_str(), "-u" ) )
	{
		InitNetwork();
		UninstallService();
		return 0;
	}
#endif

	ServerMain( );

	return 0;
}
