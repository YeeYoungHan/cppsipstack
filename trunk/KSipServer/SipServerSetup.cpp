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
#include <string.h>

CSipServerSetup gclsSetup;

/**
 * @ingroup KSipServer
 * @brief ������
 */
CSipServerSetup::CSipServerSetup() : m_iUdpPort(5060), m_iUdpThreadCount(10)
	, m_iTcpPort(5060), m_iTcpThreadCount(10), m_iTcpRecvTimeout(300)
	, m_iTlsPort(5061), m_iTlsAcceptTimeout(10)
	, m_iMinRegisterTimeout(300)
	, m_bUseRtpRelay(false), m_iBeginRtpPort(10000), m_iEndRtpPort(60000)
	, m_iDbPort(3306), m_iLogLevel(0), m_iLogMaxSize(20000000)
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

	// SIP ����
	pclsElement = clsXml.SelectElement( "Sip" );
	if( pclsElement == NULL ) return false;

	pclsElement->SelectElementData( "LocalIp", m_strLocalIp );
	pclsElement->SelectElementData( "UdpPort", m_iUdpPort );
	pclsElement->SelectElementData( "UdpThreadCount", m_iUdpThreadCount );
	pclsElement->SelectElementData( "Realm", m_strRealm );
	pclsElement->SelectElementData( "SendOptionsPeriod", m_iSendOptionsPeriod );
	pclsElement->SelectElementData( "TcpPort", m_iTcpPort );
	pclsElement->SelectElementData( "TcpThreadCount", m_iTcpThreadCount );
	pclsElement->SelectElementData( "TcpRecvTimeout", m_iTcpRecvTimeout );
	pclsElement->SelectElementData( "TlsPort", m_iTlsPort );
	pclsElement->SelectElementData( "TlsAcceptTimeout", m_iTlsAcceptTimeout );
	pclsElement->SelectElementData( "CertFile", m_strCertFile );
	pclsElement->SelectElementData( "MinRegisterTimeout", m_iMinRegisterTimeout );

	// RTP relay ����
	pclsElement = clsXml.SelectElement( "RtpRelay" );
	if( pclsElement )
	{
		pclsElement->SelectElementData( "UseRtpRelay", m_bUseRtpRelay );
		pclsElement->SelectElementData( "BeginPort", m_iBeginRtpPort );
		pclsElement->SelectElementData( "EndPort", m_iEndRtpPort );
	}

	// �α�
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

	// XML ����
	pclsElement = clsXml.SelectElement( "XmlFolder" );
	if( pclsElement )
	{
		pclsElement->SelectElementData( "User", m_strUserXmlFolder );
		pclsElement->SelectElementData( "SipServer", m_strSipServerXmlFolder );
	}

	// DB
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

	// CDR
	if( m_eType == E_DT_XML )
	{
		pclsElement = clsXml.SelectElement( "Cdr" );
		if( pclsElement == NULL ) return false;

		pclsElement->SelectElementData( "Folder", m_strCdrFolder );
	}

	// ����͸�
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

	// ����
	pclsElement = clsXml.SelectElement( "Security" );
	if( pclsElement == NULL ) return false;

	pclsClient = pclsElement->SelectElement( "DenySipUserAgentList" );
	if( pclsClient )
	{
		XML_ELEMENT_LIST clsList;
		XML_ELEMENT_LIST::iterator	itList;

		if( pclsClient->SelectElementList( "SipUserAgent", clsList ) )
		{
			for( itList = clsList.begin(); itList != clsList.end(); ++itList )
			{
				if( itList->IsDataEmpty() ) continue;

				m_clsDenySipUserAgentMap.Insert( itList->GetData(), "" );
			}
		}
	}

	return true;
}

/**
 * @ingroup KSipServer
 * @brief ����͸� Ŭ���̾�Ʈ IP �ּ��ΰ�?
 * @param pszIp		Ŭ���̾�Ʈ IP �ּ�
 * @returns ����͸� Ŭ���̾�Ʈ IP �ּ��̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
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

bool CSipServerSetup::IsDenyUserAgent( const char * pszSipUserAgent )
{
	return m_clsDenySipUserAgentMap.Select( pszSipUserAgent );
}
