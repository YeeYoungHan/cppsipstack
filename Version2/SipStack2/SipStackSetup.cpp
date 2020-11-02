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

#include "SipStackDefine.h"
#include "SipUdp.h"
#include "SipStackSetup.h"
#include "SipMd5.h"
#include "SipUtility.h"
#include "Log.h"
#include "TimeUtility.h"
#include "Random.h"
#include "MemoryDebug.h"

/**
 * @ingroup SipStack
 * @brief 생성자
 */
CSipStackSetup::CSipStackSetup() : m_iLocalUdpPort(SIP_UDP_PORT), m_iUdpThreadCount(1)
	, m_iLocalTcpPort(0), m_iLocalTlsPort(0)
	, m_iTcpThreadCount(1), m_iTcpCallBackThreadCount(0), m_iTcpMaxSocketPerThread(SIP_TCP_MAX_SOCKET_PER_THREAD)
	, m_iTcpRecvTimeout(SIP_TCP_RECV_TIMEOUT), m_iTcpConnectTimeout(SIP_TCP_CONNECT_TIMEOUT)
	, m_iTlsAcceptTimeout(SIP_TLS_ACCEPT_TIMEOUT), m_bUseSipCompactForm(false), m_iStackExecutePeriod(20)
	, m_iTimerD(32000), m_iTimerJ(32000), m_bIpv6(false)
	, m_bStateful(true), m_bTlsClient(false), m_bUseContactListenPort(false)
	, m_bUseRegisterSession(false)
	, m_iSipMessageQueueSize(1000)
	, m_iUdpBufSize(0)
{
}

/**
 * @ingroup SipStack
 * @brief 소멸자
 */
CSipStackSetup::~CSipStackSetup()
{
}

/**
 * @ingroup SipStack
 * @brief 설정 항목의 유효성을 검사한다.
 * @returns 설정 항목이 유효하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipStackSetup::Check( )
{
	if( m_strLocalIp.empty() ) 
	{
		CLog::Print( LOG_ERROR, "%s m_strLocalIp is empty", __FUNCTION__ );
		return false;
	}

	// UDP 관련 설정 점검
	if( m_iLocalUdpPort < 0 || m_iLocalUdpPort > 65535 ) m_iLocalUdpPort = 0;

	if( m_iUdpThreadCount <= 0 ) 
	{
		if( m_iLocalUdpPort > 0 )
		{
			CLog::Print( LOG_ERROR, "%s m_iUdpThreadCount(%d) is invalid", __FUNCTION__, m_iUdpThreadCount );
			return false;
		}

		m_iUdpThreadCount = 0;
	}

	// TCP 관련 설정 점검
	if( m_iLocalTcpPort < 0 || m_iLocalTcpPort > 65535 ) m_iLocalTcpPort = 0;
	if( m_iLocalTlsPort < 0 || m_iLocalTlsPort > 65535 ) m_iLocalTlsPort = 0;

	if( m_iTcpThreadCount <= 0 ) 
	{
		if( m_iLocalTcpPort > 0 || m_iLocalTlsPort > 0 ) 
		{
			CLog::Print( LOG_ERROR, "%s m_iTcpThreadCount(%d) is invalid", __FUNCTION__, m_iTcpThreadCount );
			return false;
		}

		m_iTcpThreadCount = 0;
	}

	if( m_iLocalUdpPort == 0 && m_iLocalTcpPort == 0 && m_iLocalTlsPort == 0 )
	{
		CLog::Print( LOG_ERROR, "%s udp/tcp/tls port is 0", __FUNCTION__ );
		return false;
	}
	
	if( m_iTcpMaxSocketPerThread < 0 ) 
	{
		if( m_iLocalTcpPort > 0 || m_iLocalTlsPort > 0 ) 
		{
			CLog::Print( LOG_ERROR, "%s m_iTcpMaxSocketPerThread(%d) is invalid", __FUNCTION__, m_iTcpMaxSocketPerThread );
			return false;
		}
		m_iTcpMaxSocketPerThread = SIP_TCP_MAX_SOCKET_PER_THREAD;
	}

	if( m_iTcpRecvTimeout < 0 )
	{
		m_iTcpRecvTimeout = SIP_TCP_RECV_TIMEOUT;
	}

	if( m_iTcpConnectTimeout < 0 )
	{
		m_iTcpConnectTimeout = SIP_TCP_CONNECT_TIMEOUT;
	}

	// TLS 관련 설정 점검
	if( m_iTlsAcceptTimeout < 0 ) m_iTlsAcceptTimeout = 0;

	if( m_strUserAgent.empty() == false )
	{
		m_strUserAgent.append( "_" );
		m_strUserAgent.append( SIP_USER_AGENT );
	}

	// SIP stack 설정 점검
	if( m_iStackExecutePeriod <= 0 )
	{
		m_iStackExecutePeriod = 20;
	}

	if( m_iTimerD <= 0 )
	{
		m_iTimerD = 32000;
	}

	if( m_iTimerJ <= 0 )
	{
		m_iTimerJ = 32000;
	}

	char	szId[21], szValue[10];
	struct timeval sttTime;
	
	memset( szId, 0, sizeof(szId) );
	memset( szValue, 0, sizeof(szValue) );
	gettimeofday( &sttTime, NULL );

	uint32_t iIp; 
	uint16_t iPort = m_iLocalUdpPort;

	if( strstr( m_strLocalIp.c_str(), ":" ) )
	{
		iIp = RandomGet();
	}
	else
	{
		iIp = inet_addr(m_strLocalIp.c_str());
	}

	memcpy( szValue, &iIp, 4 );
	memcpy( szValue+4, &iPort, 2 );
	memcpy( szValue+6, &sttTime.tv_sec, 2 );
	memcpy( szValue+8, &sttTime.tv_usec, 2 );

	SipMakePrintString( (unsigned char *)szValue, sizeof(szValue), szId, sizeof(szId) );

	SipSetSystemId( szId );

	return true;
}

/**
 * @ingroup SipStack
 * @brief 전송 프로토콜에 대한 로컬 포트 번호를 리턴한다.
 * @returns 전송 프로토콜에 대한 로컬 포트 번호를 리턴한다.
 */
int CSipStackSetup::GetLocalPort( ESipTransport eTransport )
{
	switch( eTransport )
	{
	case E_SIP_UDP:
		return m_iLocalUdpPort;
	case E_SIP_TCP:
		return m_iLocalTcpPort;
	case E_SIP_TLS:	
		return m_iLocalTlsPort;
	}
	
	return m_iLocalUdpPort;
}
