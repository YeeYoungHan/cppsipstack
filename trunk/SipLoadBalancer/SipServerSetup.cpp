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
#include <sys/stat.h>
#include "MemoryDebug.h"

CSipServerSetup gclsSetup;

/**
 * @brief XML 에 저장된 element 리스트를 문자열 맵 자료구조에 저장한다.
 * @param pclsElement		리스트를 저장한 XML element
 * @param pszTagName		문자열 리스트 tag 이름
 * @param pszSubTagName 문자열 리스트의 항목 tag 이름
 * @param clsMap				문자열 맵 자료구조
 */
void InsertStringMap( CXmlElement * pclsElement, const char * pszTagName, const char * pszSubTagName, CStringMap & clsMap )
{
	CXmlElement * pclsClient;

	pclsClient = pclsElement->SelectElement( pszTagName );
	if( pclsClient )
	{
		XML_ELEMENT_LIST clsList;
		XML_ELEMENT_LIST::iterator	itList;

		if( pclsClient->SelectElementList( pszSubTagName, clsList ) )
		{
			for( itList = clsList.begin(); itList != clsList.end(); ++itList )
			{
				if( itList->IsDataEmpty() ) continue;

				clsMap.Insert( itList->GetData(), "" );
			}
		}
	}
}

/**
 * @ingroup SipLoadBalancer
 * @brief 생성자
 */
CSipServerSetup::CSipServerSetup() : m_iUdpPort(5060), m_iUdpThreadCount(10)
	, m_iTcpPort(5060), m_iTcpThreadCount(10), m_iTcpRecvTimeout(300)
	, m_iTlsPort(5061), m_iTlsAcceptTimeout(10)
	, m_iLogLevel(0), m_iLogMaxSize(20000000)
	, m_iMonitorPort(6000), m_iFileSize(0)
{
}

/**
 * @ingroup SipLoadBalancer
 * @brief 소멸자
 */
CSipServerSetup::~CSipServerSetup()
{
}

/**
 * @ingroup SipLoadBalancer
 * @brief 설정 파일을 읽어서 멤버 변수에 저장한다.
 * @param pszFileName 설정 파일 full path
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipServerSetup::Read( const char * pszFileName )
{
	CXmlElement clsXml, * pclsElement;

	if( clsXml.ParseFile( pszFileName ) == false ) return false;

	// SIP 설정
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

	// 로그
	pclsElement = clsXml.SelectElement( "Log" );
	if( pclsElement == NULL ) 
	{
		CLog::Print( LOG_ERROR, "file(%s) does not contain Log element", pszFileName );
		return false;
	}

	pclsElement->SelectElementData( "Folder", m_strLogFolder );

	// 모니터링
	pclsElement = clsXml.SelectElement( "Monitor" );
	if( pclsElement ) 
	{
		pclsElement->SelectElementData( "Port", m_iMonitorPort );
	}

	Read( clsXml );

	m_strFileName = pszFileName;
	SetFileSizeTime();

	return true;
}

/**
 * @ingroup SipLoadBalancer
 * @brief 설정 파일에서 실시간으로 수정 가능한 항목을 읽는다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipServerSetup::Read( )
{
	CXmlElement clsXml;

	if( clsXml.ParseFile( m_strFileName.c_str() ) == false ) return false;
	if( ReadSipServer( clsXml ) == false ) return false;
	if( Read( clsXml ) == false ) return false;

	SetFileSizeTime();

	return true;
}

/**
 * @ingroup SipLoadBalancer
 * @brief 설정 파일에서 SIP 서버 리스트를 제외한 실시간으로 수정 가능한 항목을 읽는다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipServerSetup::Read( CXmlElement & clsXml )
{
	CXmlElement * pclsElement;

	// 로그
	pclsElement = clsXml.SelectElement( "Log" );
	if( pclsElement ) 
	{
		m_iLogLevel = 0;

		CXmlElement * pclsClient = pclsElement->SelectElement( "Level" );
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

		CLog::SetLevel( m_iLogLevel );
		CLog::SetMaxLogSize( m_iLogMaxSize );
	}

	// 모니터링
	m_clsMonitorIpMap.DeleteAll();

	pclsElement = clsXml.SelectElement( "Monitor" );
	if( pclsElement ) 
	{
		InsertStringMap( pclsElement, "ClientIpList", "ClientIp", m_clsMonitorIpMap );
	}

	// 보안
	m_clsDenySipUserAgentMap.DeleteAll();

	pclsElement = clsXml.SelectElement( "Security" );
	if( pclsElement )
	{
		InsertStringMap( pclsElement, "DenySipUserAgentList", "SipUserAgent", m_clsDenySipUserAgentMap );
	}

	return true;
}

/**
 * @ingroup SipLoadBalancer
 * @brief 설정 파일에서 SipServer 정보를 읽어서 SipServerMap 에 저장한다.
 * @param clsXml 설정파일을 읽은 XML 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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

	gclsSipServerMap.SetDeleteAll();

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

			bool bUse = true;
			itList->SelectAttribute( "Use", bUse );

			if( gclsSipServerMap.Insert( pszIp, iPort, bUse ) == false )
			{
				CLog::Print( LOG_ERROR, "sipserver(%s:%d) is not inserted to sipservermap" );
			}
		}
	}

	gclsSipServerMap.DeleteIfSet();

	return true;
}

/**
 * @ingroup SipLoadBalancer
 * @brief 모니터링 클라이언트 IP 주소인가?
 * @param pszIp		클라이언트 IP 주소
 * @returns 모니터링 클라이언트 IP 주소이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServerSetup::IsMonitorIp( const char * pszIp )
{
	return m_clsMonitorIpMap.Select( pszIp );
}

/**
 * @ingroup SipLoadBalancer
 * @brief 설정파일이 수정되었는지 확인한다.
 * @returns 설정파일이 수정되었으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServerSetup::IsChange()
{
	struct stat	clsStat;

	if( stat( m_strFileName.c_str(), &clsStat ) == 0 )
	{
		if( m_iFileSize != clsStat.st_size || m_iFileTime != clsStat.st_mtime ) return true;
	}

	return false;
}

/**
 * @ingroup SipLoadBalancer
 * @brief 설정파일의 저장 시간을 저장한다.
 */
void CSipServerSetup::SetFileSizeTime( )
{
	struct stat	clsStat;

	if( stat( m_strFileName.c_str(), &clsStat ) == 0 )
	{
		m_iFileSize = clsStat.st_size;
		m_iFileTime = clsStat.st_mtime;
	}
}
