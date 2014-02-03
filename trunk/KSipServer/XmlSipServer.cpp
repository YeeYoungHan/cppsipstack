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

#include "XmlSipServer.h"
#include "XmlElement.h"
#include "MemoryDebug.h"

CRoutePrefix::CRoutePrefix() : m_bDeletePrefix(false)
{
}

bool CIncomingRoute::IsEmpty()
{
	if( m_strToId.empty() || m_strDestId.empty() ) return true;

	return false;
}

CXmlSipServer::CXmlSipServer() : m_iFlag(FLAG_NULL)
{
}

CXmlSipServer::~CXmlSipServer()
{
}

/**
 * @ingroup KSipServer
 * @brief IP-PBX 정보를 저장한 XML 파일을 파싱하여서 내부 변수에 저장한다.
 * @param pszFileName IP-PBX 정보를 저장한 XML 파일의 full path
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CXmlSipServer::Parse( const char * pszFileName )
{
	CXmlElement clsXml;

	Clear();

	if( clsXml.ParseFile( pszFileName ) == false ) return false;

	if( clsXml.SelectElementData( "Use", m_bUse ) )
	{
		if( m_bUse == false ) return false;
	}

	clsXml.SelectElementData( "Ip", m_strIp );
	clsXml.SelectElementData( "Port", m_iPort );
	clsXml.SelectElementData( "Domain", m_strDomain );
	clsXml.SelectElementData( "UserId", m_strUserId );
	clsXml.SelectElementData( "PassWord", m_strPassWord );
	clsXml.SelectElementData( "LoginTimeout", m_iLoginTimeout );

	CXmlElement * pclsRPL = clsXml.SelectElement( "RoutePrefixList" ); 
	if( pclsRPL )
	{
		XML_ELEMENT_LIST clsList;
		XML_ELEMENT_LIST::iterator	itList;

		if( pclsRPL->SelectElementList( "RoutePrefix", clsList ) )
		{
			for( itList = clsList.begin(); itList != clsList.end(); ++itList )
			{
				if( itList->IsDataEmpty() ) continue;

				CRoutePrefix clsRoutePrefix;

				clsRoutePrefix.m_strPrefix = itList->GetData();
				itList->SelectAttribute( "DeletePrefix", clsRoutePrefix.m_bDeletePrefix );

				m_clsRoutePrefixList.push_back( clsRoutePrefix );
			}
		}
	}

	CXmlElement * pclsIRL = clsXml.SelectElement( "IncomingRouteList" ); 
	if( pclsIRL )
	{
		XML_ELEMENT_LIST clsList;
		XML_ELEMENT_LIST::iterator	itList;

		if( pclsIRL->SelectElementList( "IncomingRoute", clsList ) )
		{
			for( itList = clsList.begin(); itList != clsList.end(); ++itList )
			{
				CIncomingRoute clsIncomingRoute;

				itList->SelectAttribute( "ToId", clsIncomingRoute.m_strToId );
				itList->SelectAttribute( "DestId", clsIncomingRoute.m_strDestId );

				if( clsIncomingRoute.IsEmpty() ) continue;

				m_clsIncomingRouteList.push_back( clsIncomingRoute );
			}
		}
	}

	if( m_strIp.empty() ) return false;

	if( m_strDomain.empty() ) m_strDomain = m_strIp;
	if( m_iPort <= 0 ) m_iPort = 5060;

	return true;
}

/**
 * @ingroup KSipServer
 * @brief 내부 변수를 초기화 시킨다.
 */
void CXmlSipServer::Clear()
{
	m_strIp.clear();
	m_iPort = 5060;
	m_strDomain.clear();
	m_strUserId.clear();
	m_strPassWord.clear();
	m_clsRoutePrefixList.clear();
}
