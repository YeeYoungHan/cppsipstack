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

#include "Log.h"
#include "KSipServer.h"
#include "SipServerSetup.h"
#include "SipStackDefine.h"
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
 * @ingroup KSipServer
 * @brief 생성자
 */
CSipServerSetup::CSipServerSetup() : m_iUdpPort(5060), m_iUdpThreadCount(10)
	, m_iTcpPort(5060), m_iTcpThreadCount(10), m_iTcpRecvTimeout(SIP_TCP_RECV_TIMEOUT)
	, m_iTlsPort(0), m_iTlsAcceptTimeout(SIP_TLS_ACCEPT_TIMEOUT), m_iStackExecutePeriod(20), m_iTimerD(32000), m_iTimerJ(32000), m_bIpv6(false)
	, m_iMinRegisterTimeout(300)
	, m_bUseRtpRelay(false), m_iBeginRtpPort(10000), m_iEndRtpPort(60000)
	, m_iSendOptionsPeriod(0)
	, m_iLogLevel(0), m_iLogMaxSize(20000000)
	, m_iMonitorPort(6000)
	, m_iFileSize(0)
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
	CXmlElement clsXml, * pclsElement;

	if( clsXml.ParseFile( pszFileName ) == false ) return false;

	// SIP 설정
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
	pclsElement->SelectElementData( "CallPickupId", m_strCallPickupId );
	pclsElement->SelectElementData( "StackExecutePeriod", m_iStackExecutePeriod );
	pclsElement->SelectElementData( "TimerD", m_iTimerD );
	pclsElement->SelectElementData( "TimerJ", m_iTimerJ );
	pclsElement->SelectElementData( "Ipv6", m_bIpv6 );

	// 로그
	pclsElement = clsXml.SelectElement( "Log" );
	if( pclsElement == NULL ) return false;

	pclsElement->SelectElementData( "Folder", m_strLogFolder );

	// XML 폴더
	pclsElement = clsXml.SelectElement( "XmlFolder" );
	if( pclsElement )
	{
		pclsElement->SelectElementData( "User", m_strUserXmlFolder );
		pclsElement->SelectElementData( "SipServer", m_strSipServerXmlFolder );
	}

	// CDR
	pclsElement = clsXml.SelectElement( "Cdr" );
	if( pclsElement == NULL ) return false;

	pclsElement->SelectElementData( "Folder", m_strCdrFolder );

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
 * @ingroup KSipServer
 * @brief 수정된 설정 파일을 읽는다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipServerSetup::Read( )
{
	if( m_strFileName.length() == 0 ) return false;

	CXmlElement clsXml;

	if( clsXml.ParseFile( m_strFileName.c_str() ) == false ) return false;

	Read( clsXml );
	SetFileSizeTime();

	return true;
}

/**
 * @brief 설정 파일의 정보 중에서 실시간으로 변경 가능한 항목을 다시 저장한다.
 * @param clsXml 설정 파일의 내용을 저장한 변수
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

	// RTP relay 설정
	pclsElement = clsXml.SelectElement( "RtpRelay" );
	if( pclsElement )
	{
		pclsElement->SelectElementData( "UseRtpRelay", m_bUseRtpRelay );
		pclsElement->SelectElementData( "BeginPort", m_iBeginRtpPort );
		pclsElement->SelectElementData( "EndPort", m_iEndRtpPort );
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
	m_clsAllowSipUserAgentMap.DeleteAll();
	m_clsAllowClientIpMap.DeleteAll();

	pclsElement = clsXml.SelectElement( "Security" );
	if( pclsElement )
	{
		InsertStringMap( pclsElement, "DenySipUserAgentList", "SipUserAgent", m_clsDenySipUserAgentMap );
		InsertStringMap( pclsElement, "AllowSipUserAgentList", "SipUserAgent", m_clsAllowSipUserAgentMap );
		InsertStringMap( pclsElement, "AllowClientIpList", "Ip", m_clsAllowClientIpMap );
	}

	return true;
}

/**
 * @ingroup KSipServer
 * @brief 입력된 아이디가 Call PickUp 아이디인지 검사한다.
 * @param pszId 아이디
 * @returns 입력된 아이디가 Call PickUp 아이디이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServerSetup::IsCallPickupId( const char * pszId )
{
	if( m_strCallPickupId.empty() ) return false;

	if( !strcmp( m_strCallPickupId.c_str(), pszId ) ) return true;

	return false;
}

/**
 * @ingroup KSipServer
 * @brief 모니터링 클라이언트 IP 주소인가?
 * @param pszIp		클라이언트 IP 주소
 * @returns 모니터링 클라이언트 IP 주소이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServerSetup::IsMonitorIp( const char * pszIp )
{
	return m_clsMonitorIpMap.Select( pszIp );
}

/**
 * @ingroup KSipServer
 * @brief 허용된 SIP User-Agent 인지 검사한다.
 * @param pszSipUserAgent SIP User-Agent 문자열
 * @returns 허용된 SIP User-Agent 이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServerSetup::IsAllowUserAgent( const char * pszSipUserAgent )
{
	// 허용된 SIP UserAgent 자료구조가 저장되어 있지 않으면 모두 허용한다.
	if( m_clsAllowSipUserAgentMap.GetCount() == 0 ) return true;

	return m_clsAllowSipUserAgentMap.Select( pszSipUserAgent );
}

/**
 * @ingroup KSipServer
 * @brief 허용되지 않은 SIP User-Agent 인지 검사한다.
 * @param pszSipUserAgent SIP User-Agent 문자열
 * @returns 허용되지 않은 SIP User-Agent 이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServerSetup::IsDenyUserAgent( const char * pszSipUserAgent )
{
	return m_clsDenySipUserAgentMap.Select( pszSipUserAgent );
}

/**
 * @ingroup KSipServer
 * @brief 허용된 SIP 클라이언트 IP 주소인지 검사한다.
 * @param pszClientIp SIP 클라이언트 IP 주소
 * @returns 허용된 SIP 클라이언트 IP 주소이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServerSetup::IsAllowClientIp( const char * pszClientIp )
{
	// 허용된 클라이언트 IP 주소가 저장되어 있지 않으면 모두 허용한다.
	if( m_clsAllowClientIpMap.GetCount() == 0 ) return true;

	return m_clsAllowClientIpMap.Select( pszClientIp );
}

/**
 * @ingroup KSipServer
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
 * @ingroup KSipServer
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
