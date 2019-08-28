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

#include "SipCallDumpSetup.h"
#include "Log.h"
#include <sys/stat.h>
#include "MemoryDebug.h"

CSipCallDumpSetup gclsSetup;

CSipCallDumpSetup::CSipCallDumpSetup() : m_iPacketSnapLen(DEFAULT_PACKET_SNAP_LEN), m_iPacketReadTimeout(DEFAULT_PACKET_READ_TIMEOUT), m_iRtpRecvTimeout(30)
{
}

CSipCallDumpSetup::~CSipCallDumpSetup()
{
}


/**
 * @ingroup SipCallDump
 * @brief 설정 파일을 읽어서 멤버 변수에 저장한다.
 * @param pszFileName 설정 파일 full path
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipCallDumpSetup::Read( const char * pszFileName )
{
	CXmlElement clsXml, * pclsElement;

	if( clsXml.ParseFile( pszFileName ) == false ) return false;

	pclsElement = clsXml.SelectElement( "PacketDump" );
	if( pclsElement == NULL )
	{
		printf( "Setup PacketDump is not defined\n" );
		return false;
	}

	pclsElement->SelectElementTrimData( "Device", m_strPacketDevice );
	pclsElement->SelectElementTrimData( "Folder", m_strPacketFolder );

	if( m_strPacketDevice.empty() )
	{
		printf( "Setup PacketDump > Device is not defined\n" );
		return false;
	}

	if( m_strPacketFolder.empty() )
	{
		printf( "Setup PacketDump > Folder is not defined\n" );
		return false;
	}

	CDirectory::Create( m_strPacketFolder.c_str() );

	pclsElement->SelectElementData( "PacketSnapLen", m_iPacketSnapLen );
	pclsElement->SelectElementData( "PacketReadTimeout", m_iPacketReadTimeout );

	Read( clsXml );

	m_strFileName = pszFileName;
	SetFileSizeTime();

	return true;
}

/**
 * @ingroup SipCallDump
 * @brief 수정된 설정 파일을 읽는다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipCallDumpSetup::Read( )
{
	if( m_strFileName.length() == 0 ) return false;

	CXmlElement clsXml;

	if( clsXml.ParseFile( m_strFileName.c_str() ) == false ) return false;

	Read( clsXml );
	SetFileSizeTime();

	return true;
}

/**
 * @ingroup SipCallDump
 * @brief 설정 파일의 정보 중에서 실시간으로 변경 가능한 항목을 다시 저장한다.
 * @param clsXml 설정 파일의 내용을 저장한 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipCallDumpSetup::Read( CXmlElement & clsXml )
{
	CXmlElement * pclsElement;

	// RTP
	pclsElement = clsXml.SelectElement( "Rtp" );
	if( pclsElement )
	{
		pclsElement->SelectElementData( "RecvTimeout", m_iRtpRecvTimeout );
	}

	// TCP
	m_clsMutex.acquire();
	m_clsTcpPortMap.clear();

	pclsElement = clsXml.SelectElement( "Tcp" );
	if( pclsElement )
	{
		CXmlElement * pclsTcpPortList = pclsElement->SelectElement( "TcpPortList" );
		if( pclsTcpPortList )
		{
			XML_ELEMENT_LIST clsTcpPortList;
			XML_ELEMENT_LIST::iterator itTP;
			PORT_MAP::iterator itPM;

			pclsTcpPortList->SelectElementList( "TcpPort", clsTcpPortList );

			for( itTP = clsTcpPortList.begin(); itTP != clsTcpPortList.end(); ++itTP )
			{
				int iPort = atoi( itTP->GetData() );

				itPM = m_clsTcpPortMap.find( iPort );
				if( itPM == m_clsTcpPortMap.end() )
				{
					m_clsTcpPortMap.insert( PORT_MAP::value_type( iPort, iPort ) );
				}
			}
		}
	}
	m_clsMutex.release();

	// 로그
	pclsElement = clsXml.SelectElement( "Log" );
	if( pclsElement )
	{
		int iLogLevel = 0, iLogMaxSize = 0;
		std::string strLogFolder;

		pclsElement->SelectElementTrimData( "Folder", strLogFolder );
		if( strLogFolder.empty() )
		{
			printf( "Setup Log > Folder is not defined\n" );
			return false;
		}

		CLog::SetDirectory( strLogFolder.c_str() );

		CXmlElement * pclsClient = pclsElement->SelectElement( "Level" );
		if( pclsClient )
		{
			bool bTemp;

			pclsClient->SelectAttribute( "Debug", bTemp );
			if( bTemp ) iLogLevel |= LOG_DEBUG;

			pclsClient->SelectAttribute( "Info", bTemp );
			if( bTemp ) iLogLevel |= LOG_INFO;

			pclsClient->SelectAttribute( "Network", bTemp );
			if( bTemp ) iLogLevel |= LOG_NETWORK;

			pclsClient->SelectAttribute( "Sql", bTemp );
			if( bTemp ) iLogLevel |= LOG_SQL;
		}

		pclsElement->SelectElementData( "MaxSize", iLogMaxSize );

		CLog::SetLevel( iLogLevel );
		CLog::SetMaxLogSize( iLogMaxSize );
	}

	return true;
}

/**
 * @ingroup SipCallDump
 * @brief 설정파일이 수정되었는지 확인한다.
 * @returns 설정파일이 수정되었으면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipCallDumpSetup::IsChange()
{
	struct stat	clsStat;

	if( stat( m_strFileName.c_str(), &clsStat ) == 0 )
	{
		if( m_iFileSize != clsStat.st_size || m_iFileTime != clsStat.st_mtime ) return true;
	}

	return false;
}

/**
 * @ingroup SipCallDump
 * @brief TCP 에서 사용하는 SIP 포트 번호인지 검사한다.
 * @param iPort	포트 번호
 * @returns TCP 에서 사용하는 SIP 포트 번호이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipCallDumpSetup::IsTcpSipPort( int iPort )
{
	PORT_MAP::iterator itPM;
	bool bRes = false;

	m_clsMutex.acquire();
	itPM = m_clsTcpPortMap.find( iPort );
	if( itPM != m_clsTcpPortMap.end() )
	{
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipCallDump
 * @brief 설정파일의 저장 시간을 저장한다.
 */
void CSipCallDumpSetup::SetFileSizeTime( )
{
	struct stat	clsStat;

	if( stat( m_strFileName.c_str(), &clsStat ) == 0 )
	{
		m_iFileSize = clsStat.st_size;
		m_iFileTime = clsStat.st_mtime;
	}
}
