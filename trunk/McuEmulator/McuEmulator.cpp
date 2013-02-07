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
#include "Log.h"
#include <signal.h>

static bool gbStop = false;

/**
 * @ingroup McuEmulator
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
 * @ingroup McuEmulator
 * @brief C++ SIP stack �� �̿��� Radvision MCU Emulator
 * @param argc 
 * @param argv 
 * @returns ���� �����ϸ� 0 �� �����ϰ� ������ �߻��ϸ� -1 �� �����Ѵ�.
 */
int main( int argc, char * argv[] )
{
	CSipStackSetup clsSetup;

	if( argc != 2 )
	{
		printf( "[Usage] %s {sip port}\n", argv[0] );
		return -1;
	}

	int iLocalUdpPort = atoi( argv[1] );

	CLog::SetLevel( LOG_DEBUG | LOG_INFO );

	signal( SIGINT, LastMethod );
	signal( SIGTERM, LastMethod );
	signal( SIGABRT, LastMethod );
#ifndef WIN32
	signal( SIGKILL, LastMethod );
	signal( SIGQUIT, LastMethod );
	signal( SIGPIPE, SIG_IGN );
#endif

	// N���� IP�ּҸ� ����ϴ� ȣ��Ʈ������ SIP �������ݷ� ����� IP�ּҸ� ���� �Է��� �ּ���.
	// Vmware ���� ����ϴ� ��� N���� IP�ּҰ� ȣ��Ʈ�� �����մϴ�.
	GetLocalIp( clsSetup.m_strLocalIp );

	// SIP ������ listen port �� ����� UDP ��Ʈ ��ȣ�� �־��ּ���.
	clsSetup.m_iLocalUdpPort = iLocalUdpPort;

	// UDP ���� �������� �⺻ ������ 1���̴�. �̸� �����Ϸ��� CSipStackSetup.m_iUdpThreadCount �� �����ϸ� �ȴ�.

	if( gclsSipServer.Start( clsSetup ) == false )
	{
		printf( "SipServer start error\n" );
		return -1;
	}

	printf( "McuEmulator is started\n" );

	while( gbStop == false )
	{
		sleep(1);
	}

	printf( "McuEmulator is terminated\n" );

	return 0;
}
