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

#include "KSipServer.h"
#include "SipServerSetup.h"
#include "XmlElement.h"
#include "Log.h"

CSipServerSetup gclsSetup;

/**
 * @ingroup KSipServer
 * @brief 생성자
 */
CSipServerSetup::CSipServerSetup() : m_iUdpPort(5060), m_iUdpThreadCount(10), m_iDbPort(3306), m_iLogLevel(0), m_iLogMaxSize(20000000)
{
}

/**
 * @ingroup KSipServer
 * @brief 소멸자
 */
CSipServerSetup::~CSipServerSetup()
{
}

/**
 * @ingroup KSipServer
 * @brief 설정 파일을 읽어서 멤버 변수에 저장한다.
 * @param pszFileName 설정 파일 full path
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipServerSetup::Read( const char * pszFileName )
{
	CXmlElement clsXml, * pclsElement, * pclsClient;

	if( clsXml.ParseFile( pszFileName ) == false ) return false;

	pclsElement = clsXml.SelectElement( "Sip" );
	if( pclsElement == NULL ) return false;

	pclsElement->SelectElementData( "LocalIp", m_strLocalIp );
	pclsElement->SelectElementData( "UdpPort", m_iUdpPort );
	pclsElement->SelectElementData( "UdpThreadCount", m_iUdpThreadCount );
	pclsElement->SelectElementData( "Realm", m_strRealm );
	pclsElement->SelectElementData( "SendOptionsPeriod", m_iSendOptionsPeriod );

	pclsElement = clsXml.SelectElement( "Log" );
	if( pclsElement == NULL ) return false;

	pclsElement->SelectElementData( "Folder", m_strLogFolder );
	pclsClient = pclsElement->SelectElement( "Level" );
	if( pclsClient )
	{
		bool bTemp;

		pclsClient->SelectAttribute( "Debug", bTemp );
		if( bTemp ) m_iLogLevel |= LOG_DEBUG;

		pclsClient->SelectAttribute( "Info", bTemp );
		if( bTemp ) m_iLogLevel |= LOG_INFO;

		pclsClient->SelectAttribute( "Network", bTemp );
		if( bTemp ) m_iLogLevel |= LOG_NETWORK;

		pclsClient->SelectAttribute( "Sql", bTemp );
		if( bTemp ) m_iLogLevel |= LOG_SQL;
	}

	pclsElement->SelectElementData( "MaxSize", m_iLogMaxSize );

	pclsElement = clsXml.SelectElement( "XmlFolder" );
	if( pclsElement )
	{
		pclsElement->SelectElementData( "User", m_strUserXmlFolder );
		pclsElement->SelectElementData( "SipServer", m_strSipServerXmlFolder );
	}

#ifdef USE_MYSQL
	pclsElement = clsXml.SelectElement( "Db" );
	if( pclsElement )
	{
		int iTemp;

		pclsElement->SelectElementData( "Host", m_strDbHost );
		pclsElement->SelectElementData( "UserId", m_strDbUserId );
		pclsElement->SelectElementData( "PassWord", m_strDbPassWord );
		pclsElement->SelectElementData( "DataBase", m_strDbName );

		if( pclsElement->SelectElementData( "Port", iTemp ) )
		{
			m_iDbPort = iTemp;
		}
	}
#endif

	if( m_strUserXmlFolder.empty() == false )
	{
		m_eType = E_DT_XML;
	}
	else
	{
#ifdef USE_MYSQL
		m_eType = E_DT_MYSQL;

		if( m_strDbHost.empty() )
		{
			CLog::Print( LOG_ERROR, "Db.Host data is not found" );
			return false;
		}

		if( m_strDbUserId.empty() )
		{
			CLog::Print( LOG_ERROR, "Db.UserId data is not found" );
			return false;
		}

		if( m_strDbName.empty() )
		{
			CLog::Print( LOG_ERROR, "Db.DataBase data is not found" );
			return false;
		}
#else
		CLog::Print( LOG_ERROR, "XmlFolder.User data is not found" );
		return false;
#endif
	}

	if( m_eType == E_DT_XML )
	{
		pclsElement = clsXml.SelectElement( "Cdr" );
		if( pclsElement == NULL ) return false;

		pclsElement->SelectElementData( "Folder", m_strCdrFolder );
	}

	pclsElement = clsXml.SelectElement( "Monitor" );
	if( pclsElement == NULL ) return false;

	pclsElement->SelectElementData( "Port", m_iMonitorPort );
	pclsClient = pclsElement->SelectElement( "ClientIpList" );
	if( pclsClient )
	{
		XML_ELEMENT_LIST clsList;
		XML_ELEMENT_LIST::iterator	itList;

		if( pclsClient->SelectElementList( "ClientIp", clsList ) )
		{
			for( itList = clsList.begin(); itList != clsList.end(); ++itList )
			{
				if( itList->IsDataEmpty() ) continue;

				m_clsMonitorIpList.push_back( itList->GetData() );
			}
		}
	}

	return true;
}

bool CSipServerSetup::IsMonitorIp( const char * pszIp )
{
	CLIENT_IP_LIST::iterator	itList;

	for( itList = m_clsMonitorIpList.begin(); itList != m_clsMonitorIpList.end(); ++itList )
	{
		if( !strcmp( itList->c_str(), pszIp ) )
		{
			return true;
		}
	}

	return false;
}
