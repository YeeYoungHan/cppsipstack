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
 * @brief ������
 */
CSipServerSetup::CSipServerSetup() : m_iUdpPort(5060), m_iUdpThreadCount(10), m_iLogLevel(0), m_iLogMaxSize(20000000), m_iDbPort(3306)
{
}

/**
 * @ingroup KSipServer
 * @brief �Ҹ���
 */
CSipServerSetup::~CSipServerSetup()
{
}

/**
 * @ingroup KSipServer
 * @brief ���� ������ �о ��� ������ �����Ѵ�.
 * @param pszFileName ���� ���� full path
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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

	return true;
}
