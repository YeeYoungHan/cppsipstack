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

#include "SipClientSetup.h"
#include "SipUdp.h"
#include "XmlElement.h"

CSipClientSetup gclsSetupFile;

CSipClientSetup::CSipClientSetup() : m_iUdpPort(5060), m_iSipServerPort(5060), m_eSipTransport(E_SIP_UDP), m_iPcapRtpDestPort(0)
{
}

CSipClientSetup::~CSipClientSetup()
{
}

bool CSipClientSetup::Read( const char * pszFileName )
{
	CXmlElement clsXml, * pclsElement;

	if( clsXml.ParseFile( pszFileName ) == false ) return false;

	// SIP 설정
	pclsElement = clsXml.SelectElement( "Sip" );
	if( pclsElement == NULL ) return false;

	pclsElement->SelectElementData( "LocalIp", m_strLocalIp );
	pclsElement->SelectElementData( "UdpPort", m_iUdpPort );
	pclsElement->SelectElementData( "PemFile", m_strPemFile );

	if( m_strLocalIp.empty() )
	{
		GetLocalIp( m_strLocalIp );
	}

	// SIP 서버 설정
	pclsElement = clsXml.SelectElement( "SipServer" );
	if( pclsElement == NULL ) return false;

	pclsElement->SelectElementData( "Ip", m_strSipServerIp );
	pclsElement->SelectElementData( "Port", m_iSipServerPort );
	pclsElement->SelectElementData( "Domain", m_strSipDomain );
	pclsElement->SelectElementData( "UserId", m_strSipUserId );
	pclsElement->SelectElementData( "PassWord", m_strSipPassWord );

	std::string strTransport;

	pclsElement->SelectElementData( "Transport", strTransport );

	if( !strcasecmp( strTransport.c_str(), "tcp" ) )
	{
		m_eSipTransport = E_SIP_TCP;
	}
	else if( !strcasecmp( strTransport.c_str(), "tls" ) )
	{
		m_eSipTransport = E_SIP_TLS;
	}
	else
	{
		m_eSipTransport = E_SIP_UDP;
	}

	if( m_strSipServerIp.empty() )
	{
		printf( "[ERROR] SetupFile SipServer > Ip is not found\n" );
		return false;
	}

	if( m_strSipUserId.empty() )
	{
		printf( "[ERROR] SetupFile SipServer > UserId is not found\n" );
		return false;
	}

	// Pcap 설정
	pclsElement = clsXml.SelectElement( "Pcap" );
	if( pclsElement == NULL ) return false;

	pclsElement->SelectElementData( "PcapFile", m_strPcapFile );
	pclsElement->SelectElementData( "RtpDestIp", m_strPcapRtpDestIp );
	pclsElement->SelectElementData( "RtpDestPort", m_iPcapRtpDestPort );
	pclsElement->SelectElementData( "SrtpCrypto", m_strPcapSrtpCrypto );
	pclsElement->SelectElementData( "SrtpKey", m_strPcapSrtpKey );

	if( m_strPcapFile.empty() )
	{
		printf( "[ERROR] SetupFile Pcap > PcapFile is not found\n" );
		return false;
	}

	if( m_strPcapRtpDestIp.empty() )
	{
		printf( "[ERROR] SetupFile Pcap > RtpDestIp is not found\n" );
		return false;
	}

	if( m_iPcapRtpDestPort <= 0 || m_iPcapRtpDestPort > 65535 )
	{
		printf( "[ERROR] SetupFile Pcap > RtpDestPort is not correct\n" );
		return false;
	}

	return true;
}
