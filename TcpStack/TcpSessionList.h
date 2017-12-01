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

#ifndef _TCP_SESSION_LIST2_H_
#define _TCP_SESSION_LIST2_H_

#include "SipTcp.h"
#include "SipMutex.h"
#include "TlsFunction.h"
#include <string>

#define TCP_INIT_BUF_SIZE		16384

class CTcpComm;
class ITcpStackCallBack;

/**
 * @ingroup TcpStack
 * @brief TCP 세션별로 응용에서 사용할 변수를 위한 인터페이스
 */
class ITcpSessionApp
{
public:
	virtual ~ITcpSessionApp(){};
};

/**
 * @ingroup TcpStack
 * @brief TCP 쓰레드 별로 관리하는 TCP 세션 정보
 */
class CTcpSessionInfo
{
public:
	CTcpSessionInfo();
	~CTcpSessionInfo();

	bool Send( const char * pszPacket, int iPacketLen );
	void Log( const char * pszPacket, int iPacketLen, bool bSend );
	void Clear();

	std::string		m_strIp;
	int						m_iPort;

	Socket				m_hSocket;
	SSL						* m_psttSsl;

	CSipMutex			m_clsMutex;

	time_t				m_iConnectTime;
	time_t				m_iRecvTime;
	time_t				m_iSendTime;	// 클라이언트로 연결된 경우만 설정한다.

	int						m_iThreadIndex;
	int						m_iSessionIndex;

	ITcpSessionApp * m_pclsApp;
};

/**
 * @ingroup TcpStack
 * @brief TCP 쓰레드 별로 관리하는 TCP 세션 정보를 저장하는 클래스 - m_bUseThreadPipe 가 true 일 때에 사용된다.
 */
class CTcpStackSessionList
{
public:
	CTcpStackSessionList( );
	~CTcpStackSessionList( );

	bool Init( int iThreadIndex, int iPollFdMax );
	bool Insert( Socket hSocket );
	int Insert( CTcpComm & clsTcpComm );
	bool Delete( int iIndex );
	void DeleteAll( );
	void DeleteTimeout( int iTimeout, int iNoPacketTimeout
		, void (*BeforeDelete)( CTcpStackSessionList * pclsSessionList, int iIndex, void * pclsArg ), void * pclsArg );

	bool Send( int iIndex, const char * pszPacket, int iPacketLen );
	bool SendAll( const char * pszPacket, int iPacketLen, ITcpStackCallBack * pclsCallBack );
	bool SendAllExcept( const char * pszPacket, int iPacketLen, ITcpStackCallBack * pclsCallBack, int iThreadIndex, int iSessionIndex );

	struct pollfd * m_psttPollFd;
	CTcpSessionInfo * m_pclsSession;

	int	m_iPollFdMax;
	int m_iPoolFdCount;

private:
	void Insert( int iIndex, CTcpComm & clsTcpComm );
	void ClearFd( int iIndex );
};

#endif
