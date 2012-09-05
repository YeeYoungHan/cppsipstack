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

CXmlSipServer::CXmlSipServer() : m_iFlag(FLAG_NULL)
{
}

CXmlSipServer::~CXmlSipServer()
{
}

bool CXmlSipServer::Parse( const char * pszFileName )
{
	CXmlElement clsXml;

	Clear();

	if( clsXml.ParseFile( pszFileName ) == false ) return false;

	clsXml.SelectElementData( "Ip", m_strIp );
	clsXml.SelectElementData( "Port", m_iPort );
	clsXml.SelectElementData( "Domain", m_strDomain );
	clsXml.SelectElementData( "UserId", m_strUserId );
	clsXml.SelectElementData( "PassWord", m_strPassWord );

	CXmlElement * pclsRPL = clsXml.SelectElement( "RoutePrefixList" ); 
	if( pclsRPL )
	{
		XML_ELEMENT_LIST clsList;
		XML_ELEMENT_LIST::iterator	itList;

		if( pclsRPL->SelectElementList( "RoutePrefix", clsList ) )
		{
			for( itList = clsList.begin(); itList != clsList.end(); ++itList )
			{
				m_clsRoutePrefixList.push_back( itList->GetData() );
			}
		}
	}

	if( m_strIp.empty() ) return false;

	if( m_strDomain.empty() ) m_strDomain = m_strIp;
	if( m_iPort <= 0 ) m_iPort = 5060;

	return true;
}

void CXmlSipServer::Clear()
{
	m_strIp.clear();
	m_iPort = 5060;
	m_strDomain.clear();
	m_strUserId.clear();
	m_strPassWord.clear();
	m_clsRoutePrefixList.clear();
}
