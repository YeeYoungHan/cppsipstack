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

#include "SipStackThread.h"
#include "TcpSessionList.h"
#include "ServerUtility.h"
#include "SipTcpMessage.h"
#include "Log.h"
#include "MemoryDebug.h"

class CSipTcpClientArg
{
public:
	CSipStack * m_pclsSipStack;
	std::string m_strIp;
	int					m_iPort;
	CSipMessage * m_pclsSipMessage;
};

/**
 * @brief TCP 클라이언트 세션 연결을 위한 쓰레드 함수
 * @param lpParameter CThreadListEntry 객체의 포인터
 * @returns 0 을 리턴한다.
 */
#ifdef WIN32
DWORD WINAPI SipTcpClientThread( LPVOID lpParameter )
#else
void * SipTcpClientThread( void * lpParameter )
#endif
{
	CSipTcpClientArg * pclsArg = (CSipTcpClientArg *)lpParameter;
	bool bError = false;

	Socket hSocket = TcpConnect( pclsArg->m_strIp.c_str(), pclsArg->m_iPort, pclsArg->m_pclsSipStack->m_clsSetup.m_iTcpConnectTimeout );
	if( hSocket != INVALID_SOCKET )
	{
		CTcpComm		clsTcpComm;

		clsTcpComm.m_hSocket = hSocket;
		snprintf( clsTcpComm.m_szIp, sizeof(clsTcpComm.m_szIp), "%s", pclsArg->m_strIp.c_str() );
		clsTcpComm.m_iPort = pclsArg->m_iPort;
		clsTcpComm.SetUseTimeout( false );

		if( pclsArg->m_pclsSipStack->m_clsTcpThreadList.SendCommand( (char *)&clsTcpComm, sizeof(clsTcpComm) ) == false )
		{
			closesocket( hSocket );
			bError = true;
		}
		else
		{
			SipTcpSend( hSocket, pclsArg->m_strIp.c_str(), pclsArg->m_iPort, pclsArg->m_pclsSipMessage );
		}
	}
	else
	{
		CLog::Print( LOG_ERROR, "TcpConnect(%s:%d) error", pclsArg->m_strIp.c_str(), pclsArg->m_iPort );
		bError = true;
	}

	if( bError )
	{
		CSipMessage * pclsResponse = pclsArg->m_pclsSipMessage->CreateResponse( SIP_SERVICE_UNAVAILABLE );
		if( pclsResponse )
		{
			pclsArg->m_pclsSipStack->RecvSipMessage( 0, pclsResponse );
		}
	}

	--pclsArg->m_pclsSipMessage->m_iUseCount;
	delete pclsArg;

	return 0;
}

/**
 * @ingroup SipStack
 * @brief TCP 프로토콜로 SIP 메시지 수신 및 SIP 수신 이벤트를 처리하는 Thread Pool 을 시작한다.
 * @param pclsSipStack	SIP stack 포인터
 * @param pszIp					SIP 메시지를 전송할 IP 주소
 * @param iPort					SIP 메시지를 전송할 포트 번호
 * @param pszSipMessage	전송할 SIP 메시지
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool StartSipTcpClientThread( CSipStack * pclsSipStack, const char * pszIp, int iPort, CSipMessage * pclsSipMessage )
{
	CSipTcpClientArg * pclsArg = new CSipTcpClientArg();
	if( pclsArg == NULL ) return false;

	pclsArg->m_pclsSipStack = pclsSipStack;
	pclsArg->m_strIp = pszIp;
	pclsArg->m_iPort = iPort;
	pclsArg->m_pclsSipMessage = pclsSipMessage;

	++pclsArg->m_pclsSipMessage->m_iUseCount;

	return StartThread( "SipTcpClientThread", SipTcpClientThread, pclsArg );
}

