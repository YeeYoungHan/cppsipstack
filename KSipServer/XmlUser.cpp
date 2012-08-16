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

#include "SipParserDefine.h"
#include "XmlUser.h"
#include "XmlElement.h"
#include "SipServerSetup.h"
#include "Directory.h"

CXmlUser::CXmlUser() : m_bDnd(false)
{
}

CXmlUser::~CXmlUser()
{
}

bool CXmlUser::Parse( const char * pszFileName )
{
	CXmlElement clsXml;

	if( clsXml.ParseFile( pszFileName ) == false ) return false;

	clsXml.SelectElementData( "Id", m_strId );
	clsXml.SelectElementData( "PassWord", m_strPassWord );

	std::string	strTemp;

	if( clsXml.SelectElementData( "DND", strTemp ) )
	{
		if( !strcasecmp( strTemp.c_str(), "true" ) )
		{
			m_bDnd = true;
		}
	}

	if( m_strId.empty() ) return false;

	return true;
}

void CXmlUser::Clear()
{
	m_strId.clear();
	m_strPassWord.clear();
}

bool SelectUser( const char * pszUserId, CXmlUser & clsUser )
{
	if( gclsSetup.m_strUserXmlFolder.empty() == false )
	{
		std::string	strFileName = gclsSetup.m_strUserXmlFolder;

		CDirectory::AppendName( strFileName, pszUserId );
		strFileName.append( ".xml" );

		return clsUser.Parse( strFileName.c_str() );
	}
	
	return false;
}
