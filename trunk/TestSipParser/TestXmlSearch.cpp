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

static bool TestXmlSelectData()
{
	const char * pszXml = "<Setup>\n"
	" <Sip>\n"
  "  <LocalIp>192.168.0.1</LocalIp>\n"
  " </Sip>\n"
	"</Setup>\n";
	CXmlSearch	clsXml;

	if( clsXml.Parse( pszXml, (int)strlen(pszXml) ) == -1 )
	{
		printf( "xml(%s) parser error\n", pszXml );
		return false;
	}

	const char * pszData = clsXml.SelectData( "LocalIp" );
	if( pszData == NULL ) 
	{
		printf( "LocalIp is not found\n" );
		return false;
	}

	if( strcmp( pszData, "192.168.0.1" ) )
	{
		printf( "LocalIp is not correct\n" );
		return false;
	}

	return true;
}

bool TestXmlSearch( )
{
	if( TestXmlSelectData() == false ) return false;

	return true;
}
