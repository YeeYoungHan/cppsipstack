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

		itElement->SelectAttribute( "command", clsEntry.m_strCommand );
		itElement->SelectAttribute( "title", clsEntry.m_strTitle );

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
