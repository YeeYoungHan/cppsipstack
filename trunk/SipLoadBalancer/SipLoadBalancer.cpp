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
#include "ServerUtility.h"
#include "Log.h"
#include "ServerService.h"
#include "Monitor.h"
#include "MemoryDebug.h"

/**
 * @ingroup SipLoadBalancer
 * @brief C++ SIP stack �� �̿��� �ѱ��� IP-PBX
 * @returns ���� �����ϸ� 0 �� �����ϰ� ������ �߻��ϸ� -1 �� �����Ѵ�.
 */
int ServiceMain( )
{
	if( gclsSetup.Read( GetConfigFileName() ) == false )
	{
		printf( "config filename(%s) read error\n", GetConfigFileName() );
		return -1;
	}

	CLog::SetDirectory( gclsSetup.m_strLogFolder.c_str() );
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

	Fork( true );
	SetCoreDumpEnable();
	ServerSignal();

	if( gclsSipServer.Start( clsSetup ) == false )
	{
		CLog::Print( LOG_ERROR, "SipServer start error\n" );
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

		if( iSecond == 3600 )
		{
			iSecond = 0;
		}

		if( gclsSetup.IsChange() )
		{
			gclsSetup.Read();
		}
	}

	gclsSipStack.Final();

	CLog::Print( LOG_SYSTEM, "SipLoadBalancer is terminated" );
	CLog::Release();

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
	CServerService clsService;

	clsService.m_strName = SERVICE_NAME;
	clsService.m_strDisplayName = SERVICE_DISPLAY_NAME;
	clsService.m_strDescription = SERVICE_DESCRIPTION_STRING;
	clsService.m_strConfigFileName = CONFIG_FILENAME;
	clsService.m_strVersion = SIP_LOAD_BALANCER_VERSION;
	clsService.SetBuildDate( __DATE__, __TIME__ );

	ServerMain( argc, argv, clsService, ServiceMain );

	return 0;
}
