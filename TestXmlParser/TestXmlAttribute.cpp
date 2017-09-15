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
	const char * pszXml = "<Setup>\n"
	" <Log>\n"
  "  <Level Debug=\"true\" />\n"
	" </Log>\n"
	"</Setup>\n";
	CXmlElement	clsXml, * pclsElement, * pclsClient;
	bool bDebug;

	if( clsXml.Parse( pszXml, (int)strlen(pszXml) ) == -1 )
	{
		printf( "xml(%s) parser error\n", pszXml );
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

	return true;
}

bool TestXmlAttribute()
{
	if( TestXmlGetElementAttribute() == false ) return false;

	return true;
}
