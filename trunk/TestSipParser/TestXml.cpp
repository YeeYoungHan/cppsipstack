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

#include "XmlElement.h"
#include <string.h>
#include <stdio.h>

static bool Test( const char * pszText, const char * pszResult )
{
	CXmlElement	clsXml;
	int		iPos, iTextLen = (int)strlen(pszText);
	char	szResult[1024];

	iPos = clsXml.Parse( pszText, iTextLen );
	if( iPos == -1 )
	{
		printf( "xml(%s) parse error\n", pszText );
		return false;
	}

	clsXml.ToString( szResult, sizeof(szResult) );
	if( strcmp( pszResult, szResult ) )
	{
		printf( "xml(%s) tostring error(%s)\n", pszResult, szResult );
		return false;
	}

	return true;
}

bool TestXml()
{
	if( Test( 
		"<userlist>\r\n"
		"  <user id=\"1000\">\r\n"
		"    <password>1234</password>\r\n"
		"  </user>\r\n"
		"</userlist>\r\n",
		"<userlist>\n"
		"<user id=\"1000\">\n"
		"<password>1234</password>\n"
		"</user>\n"
		"</userlist>\n" ) == false ) return false;

	if( Test( 
		"<userlist>\r\n"
		"	<user id=\"1000\">\r\n"
		"		<password>1234</password>\r\n"
		"	</user>\r\n"
		"</userlist>\r\n",
		"<userlist>\n"
		"<user id=\"1000\">\n"
		"<password>1234</password>\n"
		"</user>\n"
		"</userlist>\n" ) == false ) return false;

	if( Test( 
		"<userlist>\n"
		"	<user id=\"1000\" password=\"1234\" />\n"
		"	<user id=\"1001\" password=\"4321\" />\n"
		"</userlist>\n",
		"<userlist>\n"
		"<user id=\"1000\" password=\"1234\"/>\n"
		"<user id=\"1001\" password=\"4321\"/>\n"
		"</userlist>\n" ) == false ) return false;

	if( Test( 
		"<setup>\n"
		"	<sip ip=\"127.0.0.1\" port=\"5060\" udp_thread_count=\"10\" />\n"
		"	<log folder=\"/usr/local/bin/log\" max_size=\"4321\" />\n"
		"</setup>\n",
		"<setup>\n"
		"<sip ip=\"127.0.0.1\" port=\"5060\" udp_thread_count=\"10\"/>\n"
		"<log folder=\"/usr/local/bin/log\" max_size=\"4321\"/>\n"
		"</setup>\n" ) == false ) return false;

	// 주석 테스트
	if( Test( 
		"<setup>\n"
		" <!-- SIP info -->\n"
		"	<sip ip=\"127.0.0.1\" port=\"5060\" udp_thread_count=\"10\" />\n"
		"	<log folder=\"/usr/local/bin/log\" max_size=\"4321\" />\n"
		"</setup>\n",
		"<setup>\n"
		"<sip ip=\"127.0.0.1\" port=\"5060\" udp_thread_count=\"10\"/>\n"
		"<log folder=\"/usr/local/bin/log\" max_size=\"4321\"/>\n"
		"</setup>\n" ) == false ) return false;

	if( Test( 
		"<setup> <!--Start-->\n"
		"	<sip ip=\"127.0.0.1\" port=\"5060\" udp_thread_count=\"10\" /><!-- SIP info -->\n"
		"	<log folder=\"/usr/local/bin/log\" max_size=\"4321\" /><!-- Log -->\n"
		"</setup><!--End-->\n",
		"<setup>\n"
		"<sip ip=\"127.0.0.1\" port=\"5060\" udp_thread_count=\"10\"/>\n"
		"<log folder=\"/usr/local/bin/log\" max_size=\"4321\"/>\n"
		"</setup>\n" ) == false ) return false;

	const char * pszXml = "<userlist>\r\n"
		"  <user id=\"1000\">\r\n"
		"    <password>1234</password>\r\n"
		"  </user>\r\n"
		"</userlist>\r\n";
	CXmlElement	clsXml;

	if( clsXml.Parse( pszXml, (int)strlen(pszXml) ) == -1 )
	{
		printf( "xml(%s) parser error\n", pszXml );
		return false;
	}

	if( strcmp( clsXml.GetName(), "userlist" ) )
	{
		printf( "xml root name error\n" );
		return false;
	}

	CXmlElement * pclsElement = clsXml.SelectElement( "user" );
	if( pclsElement == NULL )
	{
		printf( "xml SelectElement error\n" );
		return false;
	}

	const char * pszId = pclsElement->SelectAttribute( "id" );
	if( pszId == NULL )
	{
		printf( "xml SelectAttribute error\n" );
		return false;
	}

	if( strcmp( pszId, "1000" ) )
	{
		printf( "xml attribute id error\n" );
		return false;
	}

	pclsElement = pclsElement->SelectElement( "password" );
	if( pclsElement == NULL )
	{
		printf( "xml password SelectElement error\n" );
		return false;
	}

	const char * pszData = pclsElement->GetData();
	if( pszData == NULL )
	{
		printf( "xml GetData error\n" );
		return false;
	}

	if( strcmp( pszData, "1234" ) )
	{
		printf( "xml password error\n" );
		return false;
	}

	pszXml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n" 
		"<presence xmlns=\"urn:ietf:params:xml:ns:pidf\">\r\n" 
		"<tuple id=\"C\">\r\n" 
		"<address uri=\"sip:1001@192.168.1.1\">\r\n" 
		"<status>busy</status>\r\n" 
		"<phone-type>Phone</phone-type>\r\n" 
		"</address>\r\n" 
		"</tuple>\r\n" 
		"</presence>\r\n";

	if( clsXml.Parse( pszXml, (int)strlen(pszXml) ) == -1 )
	{
		printf( "xml(%s) parser error\n", pszXml );
		return false;
	}

	std::string strUrl, strStatus;

	CXmlElement * pclsPresence = clsXml.SelectElement( "presence" );
	if( pclsPresence )
	{
		CXmlElement * pclsTuple = pclsPresence->SelectElement( "tuple" );
		if( pclsTuple )
		{
			CXmlElement * pclsAddress = pclsTuple->SelectElement( "address" );
			if( pclsAddress )
			{
				pclsAddress->SelectAttribute( "uri", strUrl );
				CXmlElement * pclsStatus = pclsAddress->SelectElement( "status" );
				if( pclsStatus )
				{
					if( pclsStatus->GetData() != NULL )
					{
						strStatus = pclsStatus->GetData();
					}
				}
			}
		}
	}

	if( strcmp( strUrl.c_str(), "sip:1001@192.168.1.1" ) ) return false;
	if( strcmp( strStatus.c_str(), "busy" ) ) return false;

	return true;
}
