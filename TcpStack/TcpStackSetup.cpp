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

#include "SipPlatformDefine.h"
#include "TcpStackSetup.h"
#include "Log.h"
#include "MemoryDebug.h"

CTcpStackSetup::CTcpStackSetup() : m_iListenPort(0)
	, m_iThreadInitCount(1), m_iThreadMaxCount(10), m_iMaxSocketPerThread(10)
	, m_iTcpRecvTimeout(3600), m_iTcpNoPacketTimeout(10), m_iTcpConnectTimeout(10)
	, m_bUseTls(false), m_bUseThreadPipe(true)
{
}

CTcpStackSetup::~CTcpStackSetup()
{
}

/**
 * @ingroup TcpStack
 * @brief XML element 에서 TCP stack 설정 사항을 가져온다.
 * @param clsXml XML element
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CTcpStackSetup::Parse( CXmlElement & clsXml )
{
	clsXml.SelectElementData( "ListenIp", m_strListenIp );
	clsXml.SelectElementData( "ListenPort", m_iListenPort );
	clsXml.SelectElementData( "ThreadInitCount", m_iThreadInitCount );
	clsXml.SelectElementData( "ThreadMaxCount", m_iThreadMaxCount );
	clsXml.SelectElementData( "MaxSocketPerThread", m_iMaxSocketPerThread );
	clsXml.SelectElementData( "TcpRecvTimeout", m_iTcpRecvTimeout );
	clsXml.SelectElementData( "TcpNoPacketTimeout", m_iTcpNoPacketTimeout );
	clsXml.SelectElementData( "TcpConnectTimeout", m_iTcpConnectTimeout );
	clsXml.SelectElementData( "UseTls", m_bUseTls );
	clsXml.SelectElementTrimData( "CertFile", m_strCertFile );
	clsXml.SelectElementData( "UseThreadPipe", m_bUseThreadPipe );

	if( m_iListenPort > 0 && m_bUseTls && m_strCertFile.empty() )
	{
		CLog::Print( LOG_ERROR, "%s CertFile is not defined", __FUNCTION__ );
		return false;
	}

	return true;
}
