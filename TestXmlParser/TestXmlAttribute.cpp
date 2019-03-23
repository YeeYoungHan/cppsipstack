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

static bool TestXmlGetElementAttribute()
{
	char szXml[1024];
	double dbInput = 123456789.1234;

	snprintf( szXml, sizeof(szXml), "<Setup>\n"
		" <Log>\n"
		"  <Level Debug=\"true\" Int64=\"" LONG_LONG_FORMAT "\" Int32=\"%d\" Double=\"%f\" />\n"
		" </Log>\n"
		"</Setup>\n"
		, INT64_MAX, INT32_MAX, dbInput );

	CXmlElement	clsXml, * pclsElement, * pclsClient;
	bool bDebug;
	int32_t i32;
	int64_t i64;
	double dbValue;

	if( clsXml.Parse( szXml, (int)strlen(szXml) ) == -1 )
	{
		printf( "xml(%s) parser error\n", szXml );
		return false;
	}

	pclsElement = clsXml.SelectElement( "Log" );
	if( pclsElement == NULL ) 
	{
		printf( "Log is not found\n" );
		return false;
	}

	pclsClient = pclsElement->SelectElement( "Level" );
	if( pclsClient == NULL )
	{
		printf( "Level is not found\n" );
		return false;
	}

	if( pclsClient->SelectAttribute( "Debug", bDebug ) == false )
	{
		printf( "Debug is not found\n" );
		return false;
	}

	if( bDebug == false )
	{
		printf( "bDebug is not correct\n" );
		return false;
	}

	if( pclsClient->SelectAttribute( "Int64", i64 ) == false )
	{
		printf( "Int64 is not found\n" );
		return false;
	}

	if( i64 != INT64_MAX )
	{
		printf( "i64(" LONG_LONG_FORMAT ") != " LONG_LONG_FORMAT "\n", i64, INT64_MAX );
		return false;
	}

	if( pclsClient->SelectAttribute( "Int32", i32 ) == false )
	{
		printf( "Int32 is not found\n" );
		return false;
	}

	if( i32 != INT32_MAX )
	{
		printf( "i32(%d) != %d\n", i32, INT32_MAX );
		return false;
	}

	if( pclsClient->SelectAttribute( "Double", dbValue ) == false )
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

bool TestXmlAttribute()
{
	if( TestXmlGetElementAttribute() == false ) return false;

	return true;
}
