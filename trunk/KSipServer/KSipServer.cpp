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
#include "SipServerSetup.h"
#include "Log.h"

/**
 * @ingroup SimpleSipServer
 * @brief C++ SIP stack �� �̿��� �ʰ��� SIP ����
 * @param argc 
 * @param argv 
 * @returns ���� �����ϸ� 0 �� �����ϰ� ������ �߻��ϸ� -1 �� �����Ѵ�.
 */
int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		printf( "[Usage] %s {config filename}\n", argv[0] );
		return -1;
	}

	const char * pszConfigFileName = argv[1];

	if( gclsSetup.Read( pszConfigFileName ) == false )
	{
		printf( "config filename(%s) read error\n", pszConfigFileName );
		return -1;
	}

	CLog::SetDirectory( gclsSetup.m_strLogFolder.c_str() );
	CLog::SetLevel( gclsSetup.m_iLogLevel );

	CLog::Print( LOG_SYSTEM, "KSipServer is started" );

	CSipStackSetup clsSetup;

	if( gclsSetup.m_strLocalIp.empty() )
	{
		// N���� IP�ּҸ� ����ϴ� ȣ��Ʈ������ SIP �������ݷ� ����� IP�ּҸ� ���� �Է��� �ּ���.
		// Vmware ���� ����ϴ� ��� N���� IP�ּҰ� ȣ��Ʈ�� �����մϴ�.
		GetLocalIp( clsSetup.m_strLocalIp );
		gclsSetup.m_strLocalIp = clsSetup.m_strLocalIp;
	}

	clsSetup.m_iLocalUdpPort = gclsSetup.m_iUdpPort;
	clsSetup.m_iUdpThreadCount = gclsSetup.m_iUdpThreadCount;

	if( gclsSipServer.Start( clsSetup ) == false )
	{
		CLog::Print( LOG_ERROR, "SipServer start error\n" );
		return -1;
	}

	while( 1 )
	{
		sleep(1);
	}

	return 0;
}
