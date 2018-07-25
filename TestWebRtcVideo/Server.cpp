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
#include "Directory.h"
#include "Log.h"
#include "RtpThread.h"
#include "MemoryDebug.h"

std::string gstrLocalIp;

bool StartServer( )
{
	CTcpStackSetup clsHttpSetup;

	const char * pszDocumentRoot = "C:\\OpenProject\\CppSipStack\\trunk\\TestWebRtcVideo\\html";
	const char * pszPemFile = NULL;

#ifdef WIN32
	CLog::SetDirectory( "c:\\temp\\http" );
#ifdef _DEBUG
	CLog::SetLevel( LOG_INFO | LOG_DEBUG | LOG_NETWORK );
#endif
#else
	CLog::SetDirectory( "/tmp/http" );
	CLog::SetLevel( LOG_INFO | LOG_DEBUG | LOG_NETWORK );
#endif

	InitNetwork();
	InitDtls();

	// HTTP 수신 포트 번호를 설정한다.
	clsHttpSetup.m_iListenPort = 8080;
	clsHttpSetup.m_iMaxSocketPerThread = 1;
	clsHttpSetup.m_iThreadMaxCount = 0;
	clsHttpSetup.m_bUseThreadPipe = false;

	if( pszPemFile )
	{
		clsHttpSetup.m_iListenPort = 443;
		clsHttpSetup.m_bUseTls = true;
		clsHttpSetup.m_strCertFile = pszPemFile;
	}

	// HTTP 서버에서 사용할 Document root 폴더를 설정한다.
	gclsHttpCallBack.m_strDocumentRoot = pszDocumentRoot;

	if( CDirectory::IsDirectory( gclsHttpCallBack.m_strDocumentRoot.c_str() ) == false )
	{
		printf( "[%s] is not directory\n", gclsHttpCallBack.m_strDocumentRoot.c_str() );
		return false;
	}

	// HTTP 서버를 시작한다. HTTP 요청이 수신되면 이에 대한 이벤트를 CSimpleHttpServer 객체로 전달한다.
	if( gclsHttpStack.Start( &clsHttpSetup, &gclsHttpCallBack ) == false )
	{
		printf( "gclsHttpStack.Start error\n" );
		return false;
	}

	return true;
}

bool StopServer()
{
	FinalDtls();
	gclsHttpStack.Stop();
	SSLFinal();

	// 모든 쓰레드가 종료될 때까지 대기한다.
	sleep(2);

#ifdef WIN32
	CLog::Release();
#endif

	return 0;
}
