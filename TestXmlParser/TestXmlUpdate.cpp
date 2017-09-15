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

bool TestXmlUpdate()
{
	std::string strXml;
	CXmlElement clsXml( "Root" );
	clsXml.InsertElementData( "Child", "1" );

	clsXml.UpdateElementData( "Child", "2" );

	strXml.clear();
	clsXml.ToString( strXml );
	if( strcmp( strXml.c_str(), "<Root>\n<Child>2</Child>\n</Root>\n" ) )
	{
		printf( "%s error\n", __FUNCTION__ );
		return false;
	}

	CXmlElement * pclsChild = clsXml.SelectElement( "Child" );
	if( pclsChild )
	{
		pclsChild->SetData( "3" );
	}

	strXml.clear();
	clsXml.ToString( strXml );
	if( strcmp( strXml.c_str(), "<Root>\n<Child>3</Child>\n</Root>\n" ) )
	{
		printf( "%s error\n", __FUNCTION__ );
		return false;
	}

	return true;
}
