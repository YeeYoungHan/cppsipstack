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
#include "Directory.h"
#include "KSipServerVersion.h"
#include "SipUserAgentVersion.h"
#include "ServerService.h"
#include "Monitor.h"
#include "MemoryDebug.h"

bool gbFork = true;

/**
 * @ingroup KSipServer
 * @brief C++ SIP stack 을 이용한 한국형 IP-PBX
 * @returns 정상 종료하면 0 을 리턴하고 오류가 발생하면 -1 를 리턴한다.
 */
int ServiceMain( )
{
#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
#endif

	if( gclsSetup.Read( GetConfigFileName() ) == false && gclsSetup.Read( CONFIG_FILENAME ) == false )
	{
		printf( "config filename(%s) read error\n", GetConfigFileName() );
		return -1;
	}

	CLog::SetDirectory( gclsSetup.m_strLogFolder.c_str() );
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

	clsSetup.m_iLocalTlsPort = gclsSetup.m_iTlsPort;
	clsSetup.m_iTlsAcceptTimeout = gclsSetup.m_iTlsAcceptTimeout;
	clsSetup.m_strCertFile = gclsSetup.m_strCertFile;
	
	clsSetup.m_strUserAgent = "KSipServer_";
	clsSetup.m_strUserAgent.append( KSIP_SERVER_VERSION );
	clsSetup.m_strUserAgent.append( "_" );
	clsSetup.m_strUserAgent.append( SIP_USER_AGENT_VERSION );

	Fork( gbFork );
	SetCoreDumpEnable();
	ServerSignal();

	gclsSipServerMap.Load();

	if( gclsSipServer.Start( clsSetup ) == false )
	{
		CLog::Print( LOG_ERROR, "SipServer start error" );
		return -1;
	}

	if( gclsSetup.m_iMonitorPort > 0 )
	{
		gclsMonitor.m_iMonitorPort = gclsSetup.m_iMonitorPort;
		StartMonitorServerThread( &gclsMonitor );
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

		if( gclsSetup.IsChange() )
		{
			gclsSetup.Read();
		}
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

	gclsUserAgent.Stop();
	gclsUserAgent.Final();

	CLog::Print( LOG_SYSTEM, "KSipServer is terminated" );
	CLog::Release();

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
	CServerService clsService;

	clsService.m_strName = SERVICE_NAME;
	clsService.m_strDisplayName = SERVICE_DISPLAY_NAME;
	clsService.m_strDescription = SERVICE_DESCRIPTION_STRING;
	clsService.m_strConfigFileName = CONFIG_FILENAME;
	clsService.m_strVersion = KSIP_SERVER_VERSION;

	if( argc == 3 && !strcmp( argv[2], "-n" ) )
	{
		gbFork = false;
	}

	ServerMain( argc, argv, clsService, ServiceMain );

	return 0;
}
