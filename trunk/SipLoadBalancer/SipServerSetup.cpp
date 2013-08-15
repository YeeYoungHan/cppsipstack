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

#include "SipServer.h"
#include "SipServerSetup.h"
#include "SipServerMap.h"
#include "Log.h"
#include <string.h>

CSipServerSetup gclsSetup;

/**
 * @ingroup SipLoadBalancer
 * @brief ������
 */
CSipServerSetup::CSipServerSetup() : m_iUdpPort(5060), m_iUdpThreadCount(10)
	, m_iTcpPort(5060), m_iTcpThreadCount(10), m_iTcpRecvTimeout(300)
	, m_iTlsPort(5061), m_iTlsAcceptTimeout(10)
	, m_iLogLevel(0), m_iLogMaxSize(20000000)
{
}

/**
 * @ingroup SipLoadBalancer
 * @brief �Ҹ���
 */
CSipServerSetup::~CSipServerSetup()
{
}

/**
 * @ingroup SipLoadBalancer
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
	if( pclsElement == NULL ) 
	{
		CLog::Print( LOG_ERROR, "file(%s) does not contain Sip element", pszFileName );
		return false;
	}

	pclsElement->SelectElementData( "LocalIp", m_strLocalIp );
	pclsElement->SelectElementData( "UdpPort", m_iUdpPort );
	pclsElement->SelectElementData( "UdpThreadCount", m_iUdpThreadCount );
	pclsElement->SelectElementData( "TcpPort", m_iTcpPort );
	pclsElement->SelectElementData( "TcpThreadCount", m_iTcpThreadCount );
	pclsElement->SelectElementData( "TcpRecvTimeout", m_iTcpRecvTimeout );
	pclsElement->SelectElementData( "TlsPort", m_iTlsPort );
	pclsElement->SelectElementData( "TlsAcceptTimeout", m_iTlsAcceptTimeout );
	pclsElement->SelectElementData( "CertFile", m_strCertFile );

	if( ReadSipServer( clsXml ) == false ) return false;

	// �α�
	pclsElement = clsXml.SelectElement( "Log" );
	if( pclsElement == NULL ) 
	{
		CLog::Print( LOG_ERROR, "file(%s) does not contain Log element", pszFileName );
		return false;
	}

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

	// ����͸�
	pclsElement = clsXml.SelectElement( "Monitor" );
	if( pclsElement ) 
	{
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
	}

	// ����
	pclsElement = clsXml.SelectElement( "Security" );
	if( pclsElement )
	{
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
	}

	return true;
}

/**
 * @ingroup SipLoadBalancer
 * @brief ���� ���Ͽ��� SipServer ������ �о SipServerMap �� �����Ѵ�.
 * @param pszFileName ���� ���� �̸�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipServerSetup::ReadSipServer( const char * pszFileName )
{
	CXmlElement clsXml;

	if( clsXml.ParseFile( pszFileName ) == false ) return false;

	return ReadSipServer( clsXml );
}

/**
 * @ingroup SipLoadBalancer
 * @brief ���� ���Ͽ��� SipServer ������ �о SipServerMap �� �����Ѵ�.
 * @param clsXml ���������� ���� XML ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipServerSetup::ReadSipServer( CXmlElement & clsXml )
{
	CXmlElement * pclsElement;

	pclsElement = clsXml.SelectElement( "SipServerList" );
	if( pclsElement == NULL ) 
	{
		CLog::Print( LOG_ERROR, "does not contain SipServerList element" );
		return false;
	}

	XML_ELEMENT_LIST clsList;
	XML_ELEMENT_LIST::iterator	itList;

	if( pclsElement->SelectElementList( "SipServer", clsList ) )
	{
		for( itList = clsList.begin(); itList != clsList.end(); ++itList )
		{
			const char * pszIp = itList->SelectAttribute( "Ip" );
			if( pszIp == NULL || strlen(pszIp) == 0 ) continue;

			int iPort = 5060;
			itList->SelectAttribute( "Port", iPort );

			gclsSipServerMap.Insert( pszIp, iPort );
		}
	}

	return true;
}


/**
 * @ingroup SipLoadBalancer
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
