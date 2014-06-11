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
	bool bRes = false;

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
		}
		else
		{
			SipTcpSend( hSocket, pclsArg->m_strIp.c_str(), pclsArg->m_iPort, pclsArg->m_pclsSipMessage );

			SIP_MESSAGE_LIST clsSipMessageList;

			if( pclsArg->m_pclsSipStack->m_clsTcpConnectMap.Delete( pclsArg->m_strIp.c_str(), pclsArg->m_iPort, clsSipMessageList ) )
			{
				SIP_MESSAGE_LIST::iterator	itList;

				for( itList = clsSipMessageList.begin(); itList != clsSipMessageList.end(); ++itList )
				{
					SipTcpSend( hSocket, pclsArg->m_strIp.c_str(), pclsArg->m_iPort, *itList );
					--(*itList)->m_iUseCount;
				}
			}

			bRes = true;
		}
	}
	else
	{
		CLog::Print( LOG_ERROR, "TcpConnect(%s:%d) error", pclsArg->m_strIp.c_str(), pclsArg->m_iPort );
	}

	if( bRes == false )
	{
		CSipMessage * pclsResponse = pclsArg->m_pclsSipMessage->CreateResponse( SIP_CONNECT_ERROR );
		if( pclsResponse )
		{
			pclsArg->m_pclsSipStack->RecvSipMessage( 0, pclsResponse );
		}

		SIP_MESSAGE_LIST clsSipMessageList;

		if( pclsArg->m_pclsSipStack->m_clsTcpConnectMap.Delete( pclsArg->m_strIp.c_str(), pclsArg->m_iPort, clsSipMessageList ) )
		{
			SIP_MESSAGE_LIST::iterator	itList;

			for( itList = clsSipMessageList.begin(); itList != clsSipMessageList.end(); ++itList )
			{
				pclsResponse = (*itList)->CreateResponse( SIP_CONNECT_ERROR );
				if( pclsResponse )
				{
					pclsArg->m_pclsSipStack->RecvSipMessage( 0, pclsResponse );
				}
				--(*itList)->m_iUseCount;
			}
		}
	}

	--pclsArg->m_pclsSipMessage->m_iUseCount;
	pclsArg->m_pclsSipStack->ThreadEnd( -1 );
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
	if( pclsSipStack->m_clsTcpConnectMap.Insert( pszIp, iPort ) == false )
	{
		// 이미 TCP 세션 연결 중에 있으므로 새로운 TCP 세션 연결시도하지 않는다.
		pclsSipStack->m_clsTcpConnectMap.Insert( pszIp, iPort, pclsSipMessage );
		return true;
	}

	CSipTcpClientArg * pclsArg = new CSipTcpClientArg();
	if( pclsArg == NULL )
	{
		pclsSipStack->m_clsTcpConnectMap.Delete( pszIp, iPort );
		return false;
	}

	pclsArg->m_pclsSipStack = pclsSipStack;
	pclsArg->m_strIp = pszIp;
	pclsArg->m_iPort = iPort;
	pclsArg->m_pclsSipMessage = pclsSipMessage;

	++pclsArg->m_pclsSipMessage->m_iUseCount;

	return StartThread( "SipTcpClientThread", SipTcpClientThread, pclsArg );
}

