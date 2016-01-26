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
#include "UserMap.h"
#include "Directory.h"
#include <signal.h>

static bool gbStop = false;
std::string gstrUserFileName;

/**
 * @ingroup TestSipServer
 * @brief signal function
 * @param sig ��ȣ ��ȣ
 */
void LastMethod( int sig )
{
	gbStop = true;
}

/**
 * @ingroup TestSipServer
 * @brief C++ SIP stack �� �̿��� �ʰ��� SIP ����
 * @param argc 
 * @param argv 
 * @returns ���� �����ϸ� 0 �� �����ϰ� ������ �߻��ϸ� -1 �� �����Ѵ�.
 */
int main( int argc, char * argv[] )
{
	CSipStackSetup clsSetup;

	signal( SIGINT, LastMethod );
	signal( SIGTERM, LastMethod );
	signal( SIGABRT, LastMethod );
#ifndef WIN32
	signal( SIGKILL, LastMethod );
	signal( SIGQUIT, LastMethod );
	signal( SIGPIPE, SIG_IGN );
#endif

	gstrUserFileName = CDirectory::GetProgramDirectory();
	CDirectory::AppendName( gstrUserFileName, "userlist.db" );

	gclsUserMap.ReadFile( gstrUserFileName.c_str() );
	

	// N���� IP�ּҸ� ����ϴ� ȣ��Ʈ������ SIP �������ݷ� ����� IP�ּҸ� ���� �Է��� �ּ���.
	// Vmware ���� ����ϴ� ��� N���� IP�ּҰ� ȣ��Ʈ�� �����մϴ�.
	GetLocalIp( clsSetup.m_strLocalIp );

	// SIP ������ listen port �� ����� UDP ��Ʈ ��ȣ�� �־��ּ���.
	clsSetup.m_iLocalUdpPort = 5080;

	// UDP ���� �������� �⺻ ������ 1���̴�. �̸� �����Ϸ��� CSipStackSetup.m_iUdpThreadCount �� �����ϸ� �ȴ�.

	if( gclsSipServer.Start( clsSetup ) == false )
	{
		printf( "SipServer start error\n" );
		return -1;
	}

	while( gbStop == false )
	{
		sleep(1);
	}

	gclsSipServer.Stop();

	return 0;
}
