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

#include "SipSpeedSetup.h"
#include "XmlElement.h"
#include <stdio.h>

CSipSpeedSetup gclsSetup;

CSipSpeedSetup::CSipSpeedSetup()
{
}

CSipSpeedSetup::~CSipSpeedSetup()
{
}

bool CSipSpeedSetup::Read( const char * pszFileName )
{
	CXmlElement clsXml, * pclsElement;

	if( clsXml.ParseFile( pszFileName ) == false ) 
	{
		printf( "%s clsXml.ParseFile(%s) error\n", __FUNCTION__, pszFileName );
		return false;
	}

	// SIP 서버 설정
	pclsElement = clsXml.SelectElement( "SipServer" );
	if( pclsElement == NULL )
	{
		printf( "%s SipServer tag is not found\n", __FUNCTION__ );
		return false;
	}

	pclsElement->SelectElementData( "Ip", m_strSipServerIp );
	pclsElement->SelectElementData( "Port", m_iSipServerPort );
	pclsElement->SelectElementData( "Domain", m_strSipDomain );

	// 발신자 설정
	pclsElement = clsXml.SelectElement( "Caller" );
	if( pclsElement == NULL )
	{
		printf( "%s Caller tag is not found\n", __FUNCTION__ );
		return false;
	}

	pclsElement->SelectElementData( "Id", m_strCallerId );
	pclsElement->SelectElementData( "PassWord", m_strCallerPassWord );

	// 수신자 설정
	pclsElement = clsXml.SelectElement( "Callee" );
	if( pclsElement == NULL ) 
	{
		printf( "%s Callee tag is not found\n", __FUNCTION__ );
		return false;
	}

	pclsElement->SelectElementData( "Id", m_strCalleeId );
	pclsElement->SelectElementData( "PassWord", m_strCalleePassWord );

	// 테스트 설정
	pclsElement = clsXml.SelectElement( "Test" );
	if( pclsElement == NULL )
	{
		printf( "%s Test tag is not found\n", __FUNCTION__ );
		return false;
	}

	pclsElement->SelectElementData( "TotalCount", m_iCallTotalCount );
	pclsElement->SelectElementData( "ConcurrentCount", m_iCallConcurrentCount );

	return true;
}
