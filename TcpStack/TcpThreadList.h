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

#ifndef _TCP_THREAD_LIST2_H_
#define _TCP_THREAD_LIST2_H_

#include "SipStackDefine.h"
#include "SipTcp.h"
#include "SipMutex.h"
#include <vector>
#include <string>
#include "MonitorString.h"
#include "TcpSessionList.h"

class CTcpStack;

/**
 * @ingroup TcpStack
 * @brief TCP 쓰레드에 TCP 세션을 전달할 때에 사용되는 클래스 - m_bUseThreadPipe 가 true 일 때에 사용된다.
 */
class CTcpComm
{
public:
	CTcpComm() : m_hSocket(INVALID_SOCKET), m_iPort(0), m_psttSsl(NULL), m_bClient(false)
	{
		m_szIp[0] = '\0';
	}

	void Close()
	{
#ifdef USE_TLS
		if( m_psttSsl )
		{
			SSLClose( m_psttSsl );
			m_psttSsl = NULL;
		}
#endif

		if( m_hSocket != INVALID_SOCKET )
		{
			closesocket( m_hSocket );
			m_hSocket = INVALID_SOCKET;
		}
	}

	Socket	m_hSocket;
	char		m_szIp[INET6_ADDRSTRLEN];	// 패킷으로 전송되므로 std::string 을 사용할 수 없다.
	int			m_iPort;
	SSL			* m_psttSsl;

	/** TCP client 로 서버에 연결된 경우에 true 이다. */
	bool		m_bClient;
};

/**
 * @ingroup TcpStack
 * @brief 쓰레드 리스트에 포함되는 하나의 쓰레드 정보 저장 클래스
 */
class CTcpThreadInfo
{
public:
	CTcpThreadInfo();
	~CTcpThreadInfo();

	void Close();

	int				m_iIndex;				// 쓰레드 인덱스
	Socket		m_hSend;				// 송신 pipe
	Socket		m_hRecv;				// 수신 pipe

	CTcpStackSessionList	m_clsSessionList;
	CTcpStack	* m_pclsStack;
};

typedef std::vector< CTcpThreadInfo * > TCP_THREAD_LIST;

/**
 * @ingroup TcpStack
 * @brief 쓰레드 리스트 자료구조
 */
class CTcpThreadList
{
public:
	CTcpThreadList();
	~CTcpThreadList();

	bool Create( CTcpStack * pclsStack );
	void Destroy();

	bool SendCommand( const char * pszData, int iDataLen );
	bool SendCommand( const char * pszData, int iDataLen, int iThreadIndex );
	void SendCommandAll( const char * pszData, int iDataLen );
	static int RecvCommand( Socket hSocket, char * pszData, int iDataSize );

	bool Send( int iThreadIndex, int iSessionIndex, const char * pszPacket, int iPacketLen );
	bool SendAll( const char * pszPacket, int iPacketLen, ITcpStackCallBack * pclsCallBack );
	bool SendAllExcept( const char * pszPacket, int iPacketLen, ITcpStackCallBack * pclsCallBack, int iThreadIndex, int iSessionIndex );

	void DeleteNoUseThread();
	bool DeleteThread( int iThreadIndex );

	void GetString( CMonitorString & strBuf );

private:
	TCP_THREAD_LIST	m_clsList;
	int					m_iMaxSocketPerThread;
	int					m_iThreadIndex;
	CSipMutex		m_clsMutex;
	CTcpStack * m_pclsStack;

	bool AddThread();
	bool _SendCommand( Socket hSocket, const char * pszData, int iDataLen );
	int GetCount();
	int GetThreadIndex();
	bool SelectThreadIndex( int iThreadIndex );
};

#endif
