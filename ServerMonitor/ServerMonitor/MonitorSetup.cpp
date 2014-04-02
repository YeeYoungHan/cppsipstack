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

#include "StdAfx.h"
#include "MonitorSetup.h"
#include "XmlElement.h"

CMonitorSetup gclsMonitorSetup;

CMonitorSetup::CMonitorSetup() : m_iCurrentCommandIndex(-1)
{
}

CMonitorSetup::~CMonitorSetup()
{
}

bool CMonitorSetup::Read( const char * pszFileName )
{
	CXmlElement clsXml, * pclsRoot;
	char	szErrMsg[1024];

	if( clsXml.ParseFile( pszFileName ) == false ) 
	{
		_snprintf( szErrMsg, sizeof(szErrMsg), "file(%s) xml parse error(%d)", pszFileName, GetLastError() );
		m_strErrorMessage = szErrMsg;
		return false;
	}

	pclsRoot = clsXml.SelectElement( "MonitorList" );
	if( pclsRoot == NULL ) 
	{
		m_strErrorMessage = "MonitorList element is not found";
		return false;
	}

	XML_ELEMENT_LIST lstElement;
	XML_ELEMENT_LIST::iterator	itElement;

	pclsRoot->SelectElementList( "MonitorEntry", lstElement );

	if( lstElement.size() == 0 )
	{
		m_strErrorMessage = "MonitorEntry element is not found";
		return false;
	}

	for( itElement = lstElement.begin(); itElement != lstElement.end(); ++itElement )
	{
		XML_ELEMENT_LIST lstAttribute;
		XML_ELEMENT_LIST::iterator	itAttribute;
		CMonitorEntry clsEntry;
		std::string strShow;

		itElement->SelectAttribute( "command", clsEntry.m_strCommand );
		itElement->SelectAttribute( "title", clsEntry.m_strTitle );
		itElement->SelectAttribute( "show", strShow );

		if( strShow.length() > 0 )
		{
			if( _stricmp( strShow.c_str(), "true" ) && _stricmp( strShow.c_str(), "on" ) && _stricmp( strShow.c_str(), "use" ) )
			{
				continue;
			}
		}

		if( clsEntry.m_strCommand.length() == 0 )
		{
			m_strErrorMessage = "MonitorEntry command attribute is not found";
			return false;
		}

		if( clsEntry.m_strTitle.length() == 0 )
		{
			m_strErrorMessage = "MonitorEntry title attribute is not found";
			return false;
		}

		itElement->SelectElementList( "MonitorAttribute", lstAttribute );

		if( lstAttribute.size() == 0 )
		{
			_snprintf( szErrMsg, sizeof(szErrMsg), "MonitorAttribute of command(%s) is not found", clsEntry.m_strCommand.c_str() );
			m_strErrorMessage = szErrMsg;
			return false;
		}

		for( itAttribute = lstAttribute.begin(); itAttribute != lstAttribute.end(); ++itAttribute )
		{
			std::string strName;

			itAttribute->SelectAttribute( "name", strName );
			if( strName.length() == 0 )
			{
				_snprintf( szErrMsg, sizeof(szErrMsg), "MonitorAttribute name of command(%s) is not found", clsEntry.m_strCommand.c_str() );
				m_strErrorMessage = szErrMsg;
				return false;
			}

			clsEntry.m_lstAttribute.push_back( strName );
		}

		m_clsList.push_back( clsEntry );
	}

	pclsRoot = clsXml.SelectElement( "SendCommandList" );
	if( pclsRoot != NULL ) 
	{
		pclsRoot->SelectElementList( "SendCommandEntry", lstElement );

		if( lstElement.size() > 0 )
		{
			for( itElement = lstElement.begin(); itElement != lstElement.end(); ++itElement )
			{
				std::string strCommand;

				itElement->SelectAttribute( "command", strCommand );

				m_clsSendCommandList.push_back( strCommand );
			}
		}
	}

	return true;
}

bool CMonitorSetup::Select( const char * pszCommand, CMonitorEntry & clsEntry )
{
	MONITOR_LIST::iterator	itCommand;

	for( itCommand = m_clsList.begin(); itCommand != m_clsList.end(); ++itCommand )
	{
		if( !strcmp( itCommand->m_strCommand.c_str(), pszCommand ) )
		{
			clsEntry = *itCommand;
			break;
		}
	}

	return true;
}

int CMonitorSetup::GetCount( )
{
	return m_clsList.size();
}

const char * CMonitorSetup::GetNextCommand( )
{
	if( m_clsList.size() == 0 ) return "";
	
	MONITOR_LIST::iterator	itCommand;

	if( m_iCurrentCommandIndex == -1 )
	{
		m_iCurrentCommandIndex = 0;
		return m_clsList[0].m_strCommand.c_str();
	}

	++m_iCurrentCommandIndex;
	if( m_iCurrentCommandIndex >= (int)m_clsList.size() )
	{
		m_iCurrentCommandIndex = 0;
	}

	return m_clsList[m_iCurrentCommandIndex].m_strCommand.c_str();
}

const char * CMonitorSetup::GetErrorMessage( )
{
	return m_strErrorMessage.c_str();
}
