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

	if( clsXml.Parse( pszXml, (int)strlen(pszXml) ) == -1 )
	{
		printf( "xml(%s) parser error - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	const char * pszData = clsXml.SelectElementData( pszName );
	if( pszData == NULL ) 
	{
		printf( "xml(%s) LocalIp is not found - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( strcmp( pszData, pszValue ) )
	{
		printf( "xml(%s) LocalIp is not correct - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	return true;
}

static bool TestXmlSelectData( const char * pszXml, const char * pszName, const char * pszChildName, const char * pszValue )
{
	CXmlSearch	clsXml;

	if( clsXml.Parse( pszXml, (int)strlen(pszXml) ) == -1 )
	{
		printf( "xml(%s) parser error - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	const char * pszData = clsXml.SelectElementData( pszName, pszChildName );
	if( pszData == NULL ) 
	{
		printf( "xml(%s) LocalIp is not found - %s:%d\n", pszXml, __FUNCTION__, __LINE__ );
		return false;
	}

	if( strcmp( pszData, pszValue ) )
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

	return true;
}
