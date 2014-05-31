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
#include "MemoryDebug.h"

/**
 * @ingroup SipStack
 * @brief ������
 */
CSipStackSetup::CSipStackSetup() : m_iLocalUdpPort(5060), m_iUdpThreadCount(1)
	, m_iLocalTcpPort(0), m_iLocalTlsPort(0)
	, m_iTcpThreadCount(1), m_iTcpMaxSocketPerThread(SIP_TCP_MAX_SOCKET_PER_THREAD)
	, m_iTcpRecvTimeout(SIP_TCP_RECV_TIMEOUT), m_iTcpConnectTimeout(SIP_TCP_CONNECT_TIMEOUT)
	, m_iTlsAcceptTimeout(SIP_TLS_ACCEPT_TIMEOUT), m_bUseSipCompactForm(false), m_iStackExecutePeriod(20)
	, m_iTimerD(32000), m_iTimerJ(32000)
{
}

/**
 * @ingroup SipStack
 * @brief �Ҹ���
 */
CSipStackSetup::~CSipStackSetup()
{
}

/**
 * @ingroup SipStack
 * @brief ���� �׸��� ��ȿ���� �˻��Ѵ�.
 * @returns ���� �׸��� ��ȿ�ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipStackSetup::Check( )
{
	if( m_strLocalIp.empty() ) 
	{
		CLog::Print( LOG_ERROR, "%s m_strLocalIp is empty", __FUNCTION__ );
		return false;
	}

	// UDP ���� ���� ����
	if( m_iLocalUdpPort <= 0 || m_iLocalUdpPort > 65535 ) 
	{
		CLog::Print( LOG_ERROR, "%s m_iLocalUdpPort(%d) is invalid", __FUNCTION__, m_iLocalUdpPort );
		return false;
	}

	if( m_iUdpThreadCount <= 0 ) 
	{
		CLog::Print( LOG_ERROR, "%s m_iUdpThreadCount(%d) is invalid", __FUNCTION__, m_iUdpThreadCount );
		return false;
	}

	// TCP ���� ���� ����
	if( m_iLocalTcpPort < 0 || m_iLocalTcpPort > 65535 ) m_iLocalTcpPort = 0;
	if( m_iLocalTlsPort < 0 || m_iLocalTlsPort > 65535 ) m_iLocalTlsPort = 0;

	if( m_iTcpThreadCount < 0 ) 
	{
		if( m_iLocalTcpPort > 0 || m_iLocalTlsPort > 0 ) 
		{
			CLog::Print( LOG_ERROR, "%s m_iTcpThreadCount(%d) is invalid", __FUNCTION__, m_iTcpThreadCount );
			return false;
		}
		m_iTcpThreadCount = 1;
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

	// TLS ���� ���� ����
	if( m_iTlsAcceptTimeout < 0 ) m_iTlsAcceptTimeout = 0;

	if( m_strUserAgent.empty() == false )
	{
		m_strUserAgent.append( "_" );
		m_strUserAgent.append( SIP_USER_AGENT );
	}

	// SIP stack ���� ����
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

	srand( sttTime.tv_usec );

	uint32_t iIp = inet_addr(m_strLocalIp.c_str()); 
	uint16_t iPort = m_iLocalUdpPort;

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
 * @brief ���� �������ݿ� ���� ���� ��Ʈ ��ȣ�� �����Ѵ�.
 * @returns ���� �������ݿ� ���� ���� ��Ʈ ��ȣ�� �����Ѵ�.
 */
int CSipStackSetup::GetLocalPort( ESipTransport eTransport )
{
	switch( eTransport )
	{
	case E_SIP_TCP:
		return m_iLocalTlsPort;
	case E_SIP_TLS:	
		return m_iLocalTcpPort;
	default:
		return m_iLocalUdpPort;
	}
}
