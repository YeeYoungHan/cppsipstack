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

#include "TestXmlParser.h"

static bool TestXmlGetElementData()
{
	const char * pszXml = "<Setup>\n"
	" <Sip>\n"
  "  <LocalIp>192.168.0.1</LocalIp>\n"
  "  <LocalPort>5060</LocalPort>\n"
  "  <UseRtp>true</UseRtp>\n"
  "  <UseRtp>false</UseRtp>\n"
  " </Sip>\n"
	"</Setup>\n";

	const char * pszTabXml = "<Setup>\n"
	"\t<Sip>\n"
  "\t\t<LocalIp>192.168.0.1</LocalIp>\n"
  "\t\t<LocalPort>5060</LocalPort>\n"
  "\t\t<UseRtp>true</UseRtp>\n"
  "\t\t<UseRtp>false</UseRtp>\n"
  "\t</Sip>\n"
	"</Setup>\n";

	const char * pszUpdateXml = "<Setup>\n"
	"<Sip>\n"
  "<LocalIp>192.168.0.100</LocalIp>\n"
  "<LocalPort>5061</LocalPort>\n"
  "<UseRtp>false</UseRtp>\n"
  "<UseRtp>true</UseRtp>\n"
  "</Sip>\n"
	"</Setup>\n";
	CXmlElement	clsXml, * pclsElement;
	std::string strLocalIp, strXml;
	char szXml[2048];

	if( clsXml.Parse( pszXml, (int)strlen(pszXml) ) == -1 )
	{
		printf( "xml(%s) parser error\n", pszXml );
		return false;
	}

	strXml.clear();
	clsXml.ToString( strXml, true );
	if( strcmp( strXml.c_str(), pszTabXml ) )
	{
		printf( "xml(%s) != want xml(%s)", strXml.c_str(), pszTabXml );
		return false;
	}

	clsXml.ToString( szXml, sizeof(szXml), true );
	if( strcmp( szXml, pszTabXml ) )
	{
		printf( "xml(%s) != want xml(%s)", szXml, pszTabXml );
		return false;
	}

	pclsElement = clsXml.SelectElement( "Sip" );
	if( pclsElement == NULL ) 
	{
		printf( "Sip is not found\n" );
		return false;
	}

	// string
	if( pclsElement->SelectElementData( "LocalIp", strLocalIp ) == false )
	{
		printf( "LocalIp is not found\n" );
		return false;
	}

	if( strcmp( strLocalIp.c_str(), "192.168.0.1" ) )
	{
		printf( "LocalIp is not correct\n" );
		return false;
	}

	// int
	int iLocalPort;

	if( pclsElement->SelectElementData( "LocalPort", iLocalPort ) == false )
	{
		printf( "LocalPort is not found\n" );
		return false;
	}

	if( iLocalPort != 5060 )
	{
		printf( "LocalPort is not correct\n" );
		return false;
	}

	// bool
	bool bUseRtp;

	if( pclsElement->SelectElementData( "UseRtp", bUseRtp ) == false )
	{
		printf( "UseRtp is not found\n" );
		return false;
	}

	if( bUseRtp == false )
	{
		printf( "UseRtp is not correct\n" );
		return false;
	}

	// second bool
	if( pclsElement->SelectElementData( "UseRtp", bUseRtp, 1 ) == false )
	{
		printf( "UseRtp2 is not found\n" );
		return false;
	}

	if( bUseRtp )
	{
		printf( "UseRtp2 is not correct\n" );
		return false;
	}

	pclsElement->UpdateElementData( "LocalIp", "192.168.0.100" );
	pclsElement->UpdateElementData( "LocalPort", 5061 );
	pclsElement->UpdateElementData( "UseRtp", false, 0 );
	pclsElement->UpdateElementData( "UseRtp", true, 1 );

	strXml.clear();
	clsXml.ToString( strXml );
	if( strcmp( strXml.c_str(), pszUpdateXml ) )
	{
		printf( "xml(%s) != want xml(%s)", strXml.c_str(), pszUpdateXml );
		return false;
	}

	return true;
}

static bool TestXmlGetElementDataIntMax()
{
	char szXml[1024];

	snprintf( szXml, sizeof(szXml), "<Setup>\n"
		"<Int64>" LONG_LONG_FORMAT "</Int64>\n"
		"<Int32>%d</Int32>\n"
		"</Setup>"
		, INT64_MAX, INT32_MAX );

	CXmlElement	clsXml;
	int32_t i32;
	int64_t i64;

	if( clsXml.Parse( szXml, (int)strlen(szXml) ) == -1 )
	{
		printf( "xml(%s) parser error\n", szXml );
		return false;
	}

	if( clsXml.SelectElementData( "Int32", i32 ) == false )
	{
		printf( "Int32 is not found in xml(%s) \n", szXml );
		return false;
	}

	if( i32 != INT32_MAX )
	{
		printf( "i32(%d) != %d\n", i32, INT32_MAX );
		return false;
	}

	if( clsXml.SelectElementData( "Int64", i64 ) == false )
	{
		printf( "Int64 is not found in xml(%s) \n", szXml );
		return false;
	}

	if( i64 != INT64_MAX )
	{
		printf( "i64(" LONG_LONG_FORMAT ") != " LONG_LONG_FORMAT "\n", i64, INT64_MAX );
		return false;
	}

	return true;
}

static bool TestXmlGetElementDataDouble()
{
	char szXml[1024];
	double dbInput = 123456789.1234;
	double dbValue;

	snprintf( szXml, sizeof(szXml), "<Setup>\n"
		"<Double>%f</Double>\n"
		"</Setup>"
		, dbInput );

	CXmlElement	clsXml;

	if( clsXml.Parse( szXml, (int)strlen(szXml) ) == -1 )
	{
		printf( "xml(%s) parser error\n", szXml );
		return false;
	}

	if( clsXml.SelectElementData( "Double", dbValue ) == false )
	{
		printf( "Double is not found\n" );
		return false;
	}

	if( dbValue != dbInput )
	{
		printf( "Double(%f) != %f\n", dbValue, dbInput );
		return false;
	}

	return true;
}

bool TestXmlElement()
{
	if( TestXmlGetElementData() == false ) return false;
	if( TestXmlGetElementDataIntMax() == false ) return false;
	if( TestXmlGetElementDataDouble() == false ) return false;

	return true;
}
