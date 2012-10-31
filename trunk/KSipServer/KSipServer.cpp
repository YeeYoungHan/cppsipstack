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

#include "KSipServer.h"
#include "SipServer.h"
#include "SipServerSetup.h"
#include "Log.h"
#include "NonceMap.h"
#include "UserMap.h"
#include "SipServerMap.h"
#include "CallMap.h"
#include "ServerUtility.h"
#include "DbMySQL.h"
#include "Directory.h"
#include "KSipServerVersion.h"
#include "SipUserAgentVersion.h"
#include "ServerThread.h"
#include <signal.h>

bool gbStop = false;
static std::string gstrConfigFileName;

/**
 * @ingroup KSipServer
 * @brief signal function
 * @param sig 신호 번호
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
 * @ingroup KSipServer
 * @brief C++ SIP stack 을 이용한 한국형 IP-PBX
 * @returns 정상 종료하면 0 을 리턴하고 오류가 발생하면 -1 를 리턴한다.
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

	CLog::Print( LOG_SYSTEM, "KSipServer is started ( version-%s %s %s )", KSIP_SERVER_VERSION, __DATE__, __TIME__ );

	if( gclsSetup.m_strCdrFolder.empty() == false )
	{
		CDirectory::Create( gclsSetup.m_strCdrFolder.c_str() );
	}

	CSipStackSetup clsSetup;

	if( gclsSetup.m_strLocalIp.empty() )
	{
		// N개의 IP주소를 사용하는 호스트에서는 SIP 프로토콜로 사용할 IP주소를 직접 입력해 주세요.
		// Vmware 등을 사용하는 경우 N개의 IP주소가 호스트에 존재합니다.
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
	
	clsSetup.m_strUserAgent = "KSipServer_";
	clsSetup.m_strUserAgent.append( KSIP_SERVER_VERSION );
	clsSetup.m_strUserAgent.append( "_" );
	clsSetup.m_strUserAgent.append( SIP_USER_AGENT_VERSION );

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

#ifdef USE_MYSQL
	if( gclsSetup.m_eType == E_DT_MYSQL )
	{
		gclsReadDB.Connect( gclsSetup.m_strDbHost.c_str(), gclsSetup.m_strDbUserId.c_str(), gclsSetup.m_strDbPassWord.c_str(), gclsSetup.m_strDbName.c_str(), gclsSetup.m_iDbPort );
		gclsWriteDB.Connect( gclsSetup.m_strDbHost.c_str(), gclsSetup.m_strDbUserId.c_str(), gclsSetup.m_strDbPassWord.c_str(), gclsSetup.m_strDbName.c_str(), gclsSetup.m_iDbPort );

		StartDbInsertThread();
	}
#endif

	gclsSipServerMap.Load();

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

		if( iSecond % 10 == 0 )
		{
			gclsNonceMap.DeleteTimeout( 10 );
			gclsUserMap.DeleteTimeout( 10 );
			gclsUserMap.SendOptions( );
		}

		if( iSecond % 60 == 0 )
		{
			gclsSipServerMap.Load();
			gclsSipServerMap.SetSipUserAgentRegisterInfo();
		}

		if( iSecond == 3600 )
		{
			iSecond = 0;
		}

#ifdef USE_MYSQL
		if( gclsSetup.m_eType == E_DT_MYSQL )
		{
			if( gclsReadDB.IsConnected() == false )
			{
				gclsReadDB.Connect( gclsSetup.m_strDbHost.c_str(), gclsSetup.m_strDbUserId.c_str(), gclsSetup.m_strDbPassWord.c_str(), gclsSetup.m_strDbName.c_str(), gclsSetup.m_iDbPort );
			}

			if( gclsWriteDB.IsConnected() == false )
			{
				gclsWriteDB.Connect( gclsSetup.m_strDbHost.c_str(), gclsSetup.m_strDbUserId.c_str(), gclsSetup.m_strDbPassWord.c_str(), gclsSetup.m_strDbName.c_str(), gclsSetup.m_iDbPort );
			}
		}
#endif

		// QQQ: 설정 파일 수정 여부를 검사한다.
	}

	gclsCallMap.StopCallAll();
	gclsTransCallMap.StopCallAll();

	for( int i = 0; i < 20; ++i )
	{
		if( gclsCallMap.GetCount() == 0 && gclsTransCallMap.GetCount() == 0 )
		{
			break;
		}

		sleep( 1 );
	}

	DbSignal();

	for( int i = 0; i < 20; ++i )
	{
		if( IsDbInsertThreadRun() == false )
		{
			break;
		}

		sleep( 1 );
	}

	CLog::Print( LOG_SYSTEM, "KSipServer is terminated" );

	return 0;
}


/**
 * @ingroup KSipServer
 * @brief C++ SIP stack 을 이용한 한국형 IP-PBX
 * @param argc 
 * @param argv 
 * @returns 정상 종료하면 0 을 리턴하고 오류가 발생하면 -1 를 리턴한다.
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

		KSipServiceStart();
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
		printf( "%s version-%s ( build %s %s )\n", argv[0], KSIP_SERVER_VERSION, __DATE__, __TIME__ );
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
