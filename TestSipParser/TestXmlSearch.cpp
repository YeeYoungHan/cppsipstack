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

#include "XmlSearch.h"
#include <string.h>
#include <stdio.h>

static bool TestXmlSelectData( const char * pszXml, const char * pszName, const char * pszValue )
{
	CXmlSearch	clsXml;
	std::string	strData;

	if( clsXml.Parse( pszXml, (int)strlen(pszXml) ) == -1 )
	{
		printf( "xml(%s) parser error - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( clsXml.SelectElementData( pszName, strData ) == false )
	{
		printf( "xml(%s) %s is not found - %s:%d\n", pszXml, pszName, __FUNCTION__, __LINE__ );
		return false;
	}

	if( strcmp( strData.c_str(), pszValue ) )
	{
		printf( "xml(%s) %s is not correct - %s:%d\n", pszXml, pszName, __FUNCTION__, __LINE__ );
		return false;
	}

	return true;
}

static bool TestXmlSelectData( const char * pszXml, const char * pszName, int iValue )
{
	CXmlSearch	clsXml;
	int iTemp;

	if( clsXml.Parse( pszXml, (int)strlen(pszXml) ) == -1 )
	{
		printf( "xml(%s) parser error - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( clsXml.SelectElementData( pszName, iTemp ) == false )
	{
		printf( "xml(%s) %s is not found - %s:%d\n", pszXml, pszName, __FUNCTION__, __LINE__ );
		return false;
	}

	if( iValue != iTemp )
	{
		printf( "xml(%s) %s is not correct - %s:%d\n", pszXml, pszName, __FUNCTION__, __LINE__ );
		return false;
	}

	return true;
}

static bool TestXmlSelectData( const char * pszXml, const char * pszName, bool bValue )
{
	CXmlSearch	clsXml;
	bool bTemp;

	if( clsXml.Parse( pszXml, (int)strlen(pszXml) ) == -1 )
	{
		printf( "xml(%s) parser error - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( clsXml.SelectElementData( pszName, bTemp ) == false )
	{
		printf( "xml(%s) %s is not found - %s:%d\n", pszXml, pszName, __FUNCTION__, __LINE__ );
		return false;
	}

	if( bValue != bTemp )
	{
		printf( "xml(%s) %s is not correct - %s:%d\n", pszXml, pszName, __FUNCTION__, __LINE__ );
		return false;
	}

	return true;
}

static bool TestXmlSelectData( const char * pszXml, const char * pszName, const char * pszChildName, const char * pszValue, const int iIndex = 0 )
{
	CXmlSearch	clsXml;
	std::string	strData;

	if( clsXml.Parse( pszXml, (int)strlen(pszXml) ) == -1 )
	{
		printf( "xml(%s) parser error - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( clsXml.SelectElementData( pszName, pszChildName, strData, iIndex ) == false )
	{
		printf( "xml(%s) LocalIp is not found - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( strcmp( strData.c_str(), pszValue ) )
	{
		printf( "xml(%s) LocalIp is not correct - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	return true;
}

static bool TestXmlSelectElement( const char * pszXml )
{
	CXmlSearch	clsXml;

	if( clsXml.Parse( pszXml, (int)strlen(pszXml) ) == -1 )
	{
		printf( "xml(%s) parser error - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	CXmlElement * pclsElement = clsXml.SelectElement( "LocalIp" );
	if( pclsElement == NULL )
	{
		printf( "xml(%s) LocalIp is not found - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( strcmp( pclsElement->GetData(), "192.168.0.1" ) )
	{
		printf( "xml(%s) LocalIp is not correct - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	return true;
}

static bool TestXmlSelectElementN( const char * pszXml )
{
	CXmlSearch	clsXml;

	if( clsXml.Parse( pszXml, (int)strlen(pszXml) ) == -1 )
	{
		printf( "xml(%s) parser error - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	CXmlElement * pclsElement = clsXml.SelectElement( 0, 1, "LocalIp" );
	if( pclsElement == NULL )
	{
		printf( "xml(%s) LocalIp is not found - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( strcmp( pclsElement->GetData(), "192.168.0.1" ) )
	{
		printf( "xml(%s) LocalIp is not correct - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	pclsElement = clsXml.SelectElement( 0, 2, "LocalIpList", "LocalIp" );
	if( pclsElement == NULL )
	{
		printf( "xml(%s) LocalIp is not found - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( strcmp( pclsElement->GetData(), "192.168.0.1" ) )
	{
		printf( "xml(%s) LocalIp is not correct - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	pclsElement = clsXml.SelectElement( 0, 3, "Sip", "LocalIpList", "LocalIp" );
	if( pclsElement == NULL )
	{
		printf( "xml(%s) LocalIp is not found - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( strcmp( pclsElement->GetData(), "192.168.0.1" ) )
	{
		printf( "xml(%s) LocalIp is not correct - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	//
	std::string strData;

	if( clsXml.SelectElementData( 0, strData, 1, "LocalIp" ) == false )
	{
		printf( "xml(%s) LocalIp is not found - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( strcmp( strData.c_str(), "192.168.0.1" ) )
	{
		printf( "xml(%s) LocalIp is not correct - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( clsXml.SelectElementData( 0, strData, 2, "LocalIpList", "LocalIp" ) == false )
	{
		printf( "xml(%s) LocalIp is not found - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( strcmp( strData.c_str(), "192.168.0.1" ) )
	{
		printf( "xml(%s) LocalIp is not correct - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( clsXml.SelectElementData( 0, strData, 3, "Sip", "LocalIpList", "LocalIp" ) == false )
	{
		printf( "xml(%s) LocalIp is not found - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( strcmp( strData.c_str(), "192.168.0.1" ) )
	{
		printf( "xml(%s) LocalIp is not correct - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	return true;
}

bool TestXmlSearch( )
{
	if( TestXmlSelectData("<Setup>\n"
		" <Sip>\n"
		"  <LocalIp>192.168.0.1</LocalIp>\n"
		" </Sip>\n"
		"</Setup>\n", "LocalIp", "192.168.0.1" ) == false ) return false;

	if( TestXmlSelectData("<Setup>\n"
		" <Sip>\n"
		"  <LocalPort>5060</LocalPort>\n"
		"  <LocalIp>192.168.0.1</LocalIp>\n"
		" </Sip>\n"
		"</Setup>\n", "LocalIp", "192.168.0.1" ) == false ) return false;

	if( TestXmlSelectData("<Setup>\n"
		" <Sip>\n"
		"  <LocalPort>5060</LocalPort>\n"
		"  <LocalIp>192.168.0.1</LocalIp>\n"
		" </Sip>\n"
		"</Setup>\n", "LocalPort", 5060 ) == false ) return false;

	if( TestXmlSelectData("<Setup>\n"
		" <Sip>\n"
		"  <LocalPort>5060</LocalPort>\n"
		"  <LocalIp>192.168.0.1</LocalIp>\n"
		"  <TcpPort>5061</TcpPort>\n"
		" </Sip>\n"
		"</Setup>\n", "TcpPort", 5061 ) == false ) return false;

	if( TestXmlSelectData("<Setup>\n"
		" <Sip>\n"
		"  <LocalIp>192.168.0.1</LocalIp>\n"
		"  <UseRtp>false</UseRtp>\n"
		" </Sip>\n"
		"</Setup>\n", "UseRtp", false ) == false ) return false;

	if( TestXmlSelectData("<Setup>\n"
		" <Sip>\n"
		"  <LocalIp>192.168.0.1</LocalIp>\n"
		"  <UseRtp>true</UseRtp>\n"
		" </Sip>\n"
		"</Setup>\n", "UseRtp", true ) == false ) return false;

	if( TestXmlSelectData("<Setup>\n"
		" <Sip>\n"
		"  <LocalPort>5060</LocalPort>\n"
		"  <LocalIpList>\n"
		"    <LocalIp>192.168.0.1</LocalIp>\n"
		"  </LocalIpList>\n"
		" </Sip>\n"
		"</Setup>\n", "LocalIp", "192.168.0.1" ) == false ) return false;

	if( TestXmlSelectData("<Setup>\n"
		" <Sip>\n"
		"  <LocalIp>192.168.0.1</LocalIp>\n"
		" </Sip>\n"
		"</Setup>\n", "Sip", "LocalIp", "192.168.0.1" ) == false ) return false;

	if( TestXmlSelectData("<Setup>\n"
		" <Sip>\n"
		"  <LocalPort>5060</LocalPort>\n"
		"  <LocalIp>192.168.0.1</LocalIp>\n"
		" </Sip>\n"
		"</Setup>\n", "Sip", "LocalIp", "192.168.0.1" ) == false ) return false;

	if( TestXmlSelectData("<Setup>\n"
		" <Sip>\n"
		"  <LocalPort>5060</LocalPort>\n"
		"  <LocalIpList>\n"
		"    <LocalIp>192.168.0.1</LocalIp>\n"
		"  </LocalIpList>\n"
		" </Sip>\n"
		"</Setup>\n", "LocalIpList", "LocalIp", "192.168.0.1" ) == false ) return false;

	if( TestXmlSelectData("<Setup>\n"
		" <Sip>\n"
		"  <LocalPort>5060</LocalPort>\n"
		"  <LocalIpList>\n"
		"    <LocalIp>192.168.0.1</LocalIp>\n"
		"    <LocalIp>192.168.0.2</LocalIp>\n"
		"  </LocalIpList>\n"
		" </Sip>\n"
		"</Setup>\n", "LocalIpList", "LocalIp", "192.168.0.1" ) == false ) return false;

	if( TestXmlSelectData("<Setup>\n"
		" <Sip>\n"
		"  <LocalPort>5060</LocalPort>\n"
		"  <LocalIpList>\n"
		"    <LocalIp>192.168.0.1</LocalIp>\n"
		"    <LocalIp>192.168.0.2</LocalIp>\n"
		"  </LocalIpList>\n"
		" </Sip>\n"
		"</Setup>\n", "LocalIpList", "LocalIp", "192.168.0.2", 1 ) == false ) return false;

	if( TestXmlSelectData("<Setup>\n"
		"<SipServer>\n"
		" <Sip>\n"
		"  <LocalPort>5060</LocalPort>\n"
		"  <LocalIpList>\n"
		"    <LocalIp>192.168.0.1</LocalIp>\n"
		"  </LocalIpList>\n"
		" </Sip>\n"
		"</SipServer>\n"
		"</Setup>\n", "LocalIpList", "LocalIp", "192.168.0.1" ) == false ) return false;

	if( TestXmlSelectElement("<Setup>\n"
		" <Sip>\n"
		"  <LocalIp>192.168.0.1</LocalIp>\n"
		" </Sip>\n"
		"</Setup>\n") == false ) return false;

	if( TestXmlSelectElement("<Setup>\n"
		" <Sip>\n"
		"  <LocalPort>5060</LocalPort>\n"
		"  <LocalIp>192.168.0.1</LocalIp>\n"
		" </Sip>\n"
		"</Setup>\n") == false ) return false;

	if( TestXmlSelectElement("<Setup>\n"
		" <Sip>\n"
		"  <LocalPort>5060</LocalPort>\n"
		"  <LocalIpList>\n"
		"    <LocalIp>192.168.0.1</LocalIp>\n"
		"  </LocalIpList>\n"
		" </Sip>\n"
		"</Setup>\n") == false ) return false;

	if( TestXmlSelectElementN("<Setup>\n"
		" <Sip>\n"
		"  <LocalPort>5060</LocalPort>\n"
		"  <LocalIpList>\n"
		"    <LocalIp>192.168.0.1</LocalIp>\n"
		"  </LocalIpList>\n"
		" </Sip>\n"
		"</Setup>\n") == false ) return false;

	return true;
}
