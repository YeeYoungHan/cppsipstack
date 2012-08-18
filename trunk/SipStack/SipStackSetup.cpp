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

/**
 * @ingroup SipStack
 * @brief 생성자
 */
CSipStackSetup::CSipStackSetup() : m_iLocalUdpPort(5060), m_iUdpThreadCount(1)
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
	if( m_strLocalIp.empty() ) return false;
	if( m_iLocalUdpPort <= 0 || m_iLocalUdpPort > 65535 ) return false;
	if( m_iUdpThreadCount <= 0 ) return false;

	if( m_strUserAgent.empty() == false )
	{
		m_strUserAgent.append( "_" );
		m_strUserAgent.append( SIP_USER_AGENT );
	}

	return true;
}
