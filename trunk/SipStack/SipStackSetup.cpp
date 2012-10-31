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
#include "SipStackSetup.h"
#include "SipMd5.h"
#include "SipUtility.h"

/**
 * @ingroup SipStack
 * @brief ������
 */
CSipStackSetup::CSipStackSetup() : m_iLocalUdpPort(5060), m_iUdpThreadCount(1)
	, m_iLocalTcpPort(5060), m_iTcpThreadCount(1), m_iTcpMaxSocketPerThread(SIP_TCP_MAX_SOCKET_PER_THREAD), m_iTcpRecvTimeout(SIP_TCP_RECV_TIMEOUT)
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
	if( m_strLocalIp.empty() ) return false;

	// UDP ���� ���� ����
	if( m_iLocalUdpPort <= 0 || m_iLocalUdpPort > 65535 ) return false;
	if( m_iUdpThreadCount <= 0 ) return false;

	// TCP ���� ���� ����
	if( m_iLocalTcpPort < 0 || m_iLocalTcpPort > 65535 ) m_iLocalTcpPort = 0;

	if( m_iTcpThreadCount < 0 ) 
	{
		if( m_iLocalTcpPort > 0 ) return false;
		m_iTcpThreadCount = 1;
	}
	
	if( m_iTcpMaxSocketPerThread < 0 ) 
	{
		if( m_iLocalTcpPort > 0 ) return false;
		m_iTcpMaxSocketPerThread = SIP_TCP_MAX_SOCKET_PER_THREAD;
	}

	if( m_iTcpRecvTimeout < 0 )
	{
		m_iTcpRecvTimeout = SIP_TCP_RECV_TIMEOUT;
	}

	if( m_strUserAgent.empty() == false )
	{
		m_strUserAgent.append( "_" );
		m_strUserAgent.append( SIP_USER_AGENT );
	}

	char	szTemp[101], szMd5[17];
	struct timeval sttTime;
	
	gettimeofday( &sttTime, NULL );

	snprintf( szTemp, sizeof(szTemp), "%s:%d:%d:%d", m_strLocalIp.c_str(), m_iLocalUdpPort, sttTime.tv_sec, sttTime.tv_usec );
	SipMd5String16( szTemp, szMd5 );

	SipSetSystemId( szMd5 );

	return true;
}
