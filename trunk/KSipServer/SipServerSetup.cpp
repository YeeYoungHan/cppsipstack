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
#include "Log.h"
#include <string.h>
#include <sys/stat.h>

CSipServerSetup gclsSetup;

/**
 * @brief XML �� ����� element ����Ʈ�� ���ڿ� �� �ڷᱸ���� �����Ѵ�.
 * @param pclsElement		����Ʈ�� ������ XML element
 * @param pszTagName		���ڿ� ����Ʈ tag �̸�
 * @param pszSubTagName ���ڿ� ����Ʈ�� �׸� tag �̸�
 * @param clsMap				���ڿ� �� �ڷᱸ��
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
	CXmlElement clsXml, * pclsElement;

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
 * @brief ������ ���� ������ �д´�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief ���� ������ ���� �߿��� �ǽð����� ���� ������ �׸��� �ٽ� �����Ѵ�.
 * @param clsXml ���� ������ ������ ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipServerSetup::Read( CXmlElement & clsXml )
{
	CXmlElement * pclsElement;

	// �α�
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

	// ����͸�
	m_clsMonitorIpMap.DeleteAll();

	pclsElement = clsXml.SelectElement( "Monitor" );
	if( pclsElement )
	{
		InsertStringMap( pclsElement, "ClientIpList", "ClientIp", m_clsMonitorIpMap );
	}

	// ����
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
 * @brief ����͸� Ŭ���̾�Ʈ IP �ּ��ΰ�?
 * @param pszIp		Ŭ���̾�Ʈ IP �ּ�
 * @returns ����͸� Ŭ���̾�Ʈ IP �ּ��̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipServerSetup::IsMonitorIp( const char * pszIp )
{
	return m_clsMonitorIpMap.Select( pszIp );
}

/**
 * @ingroup KSipServer
 * @brief ���� SIP User-Agent ���� �˻��Ѵ�.
 * @param pszSipUserAgent SIP User-Agent ���ڿ�
 * @returns ���� SIP User-Agent �̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipServerSetup::IsAllowUserAgent( const char * pszSipUserAgent )
{
	// ���� SIP UserAgent �ڷᱸ���� ����Ǿ� ���� ������ ��� ����Ѵ�.
	if( m_clsAllowSipUserAgentMap.GetCount() == 0 ) return true;

	return m_clsAllowSipUserAgentMap.Select( pszSipUserAgent );
}

/**
 * @ingroup KSipServer
 * @brief ������ ���� SIP User-Agent ���� �˻��Ѵ�.
 * @param pszSipUserAgent SIP User-Agent ���ڿ�
 * @returns ������ ���� SIP User-Agent �̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipServerSetup::IsDenyUserAgent( const char * pszSipUserAgent )
{
	return m_clsDenySipUserAgentMap.Select( pszSipUserAgent );
}

/**
 * @ingroup KSipServer
 * @brief ���� SIP Ŭ���̾�Ʈ IP �ּ����� �˻��Ѵ�.
 * @param pszClientIp SIP Ŭ���̾�Ʈ IP �ּ�
 * @returns ���� SIP Ŭ���̾�Ʈ IP �ּ��̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipServerSetup::IsAllowClientIp( const char * pszClientIp )
{
	// ���� Ŭ���̾�Ʈ IP �ּҰ� ����Ǿ� ���� ������ ��� ����Ѵ�.
	if( m_clsAllowClientIpMap.GetCount() == 0 ) return true;

	return m_clsAllowClientIpMap.Select( pszClientIp );
}

/**
 * @ingroup KSipServer
 * @brief ���������� �����Ǿ����� Ȯ���Ѵ�.
 * @returns ���������� �����Ǿ����� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief ���������� ���� �ð��� �����Ѵ�.
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
