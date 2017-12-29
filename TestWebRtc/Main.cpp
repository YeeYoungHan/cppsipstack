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

#include "HttpCallBack.h"
#include "SipCallBack.h"
#include "Directory.h"
#include "Log.h"
#include "MemoryDebug.h"

int main( int argc, char * argv[] )
{
	if( argc != 3 )
	{
		printf( "[Usage] %s {local ip} {Document root path}\n", argv[0] );
		return 0;
	}

#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
#endif

	CTcpStackSetup clsHttpSetup;
	CSipStackSetup clsSipSetup;

#ifdef WIN32
	CLog::SetDirectory( "c:\\temp\\http" );
#ifdef _DEBUG
	CLog::SetLevel( LOG_INFO | LOG_DEBUG | LOG_NETWORK );
#endif
#endif

	// HTTP 수신 포트 번호를 설정한다.
	clsHttpSetup.m_iListenPort = 8080;
	clsHttpSetup.m_iMaxSocketPerThread = 1;
	clsHttpSetup.m_iThreadMaxCount = 0;
	clsHttpSetup.m_bUseThreadPipe = false;

	// HTTP 서버에서 사용할 Document root 폴더를 설정한다.
	gclsHttpCallBack.m_strDocumentRoot = argv[2];

	if( CDirectory::IsDirectory( gclsHttpCallBack.m_strDocumentRoot.c_str() ) == false )
	{
		printf( "[%s] is not directory\n", gclsHttpCallBack.m_strDocumentRoot.c_str() );
		return 0;
	}

	// HTTP 서버를 시작한다. HTTP 요청이 수신되면 이에 대한 이벤트를 CSimpleHttpServer 객체로 전달한다.
	if( gclsHttpStack.Start( &clsHttpSetup, &gclsHttpCallBack ) == false )
	{
		printf( "gclsHttpStack.Start error\n" );
		return 0;
	}

	// 2개 이상의 IP 주소를 가지고 있을 수 있으므로 SIP 통신에 사용할 IP 주소를 실행 인자로 받아들이고 해당 IP 주소와 동일 대역의 IP 주소만 RTP 용으로 사용한다.
	clsSipSetup.m_strLocalIp = argv[1];
	clsSipSetup.m_iLocalUdpPort = 5080;
	clsSipSetup.m_iLocalTcpPort = 5080;
	gstrLocalIp = clsSipSetup.m_strLocalIp;
	gstrLocalIpPrefix = clsSipSetup.m_strLocalIp;
	const char * pszLocalIp = gstrLocalIpPrefix.c_str();
	int iLen = gstrLocalIpPrefix.length();

	for( int i = iLen - 1; i >= 0; --i )
	{
		if( pszLocalIp[i] == '.' )
		{
			gstrLocalIpPrefix.erase( i+1 );
			break;
		}
	}

	if( gclsSipStack.Start( clsSipSetup, &gclsSipCallBack ) == false )
	{
		printf( "gclsSipStack.Start error\n" );
		return 0;
	}

	while( gclsHttpCallBack.m_bStop == false )
	{
		sleep(1);
	}

	gclsHttpStack.Stop();
	gclsSipStack.Stop();

	// 모든 쓰레드가 종료될 때까지 대기한다.
	sleep(2);

#ifdef WIN32
	CLog::Release();
#endif

	return 0;
}
