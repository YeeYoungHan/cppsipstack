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

int main( int argc, char * argv[] )
{
	CSipStackSetup clsSetup;

	// QQQ: N개의 IP주소를 사용하는 호스트에서는 SIP 프로토콜로 사용할 IP주소를 직접 입력해 주세요.
	//    : Vmware 등을 사용하는 경우 N개의 IP주소가 호스트에 존재합니다.
	GetLocalIp( clsSetup.m_strLocalIp );

	// QQQ: SIP 서버의 listen port 로 사용할 UDP 포트 번호를 넣어주세요.
	clsSetup.m_iLocalUdpPort = 5060;

	if( gclsSipServer.Start( clsSetup ) == false )
	{
		printf( "SipServer start error\n" );
		return -1;
	}

	while( 1 )
	{
		sleep(1);
	}

	return 0;
}
