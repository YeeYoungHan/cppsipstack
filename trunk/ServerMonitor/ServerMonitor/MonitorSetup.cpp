#include "StdAfx.h"
#include "MonitorSetup.h"
#include "XmlElement.h"

CMonitorSetup::CMonitorSetup()
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

const char * CMonitorSetup::GetErrorMessage( )
{
	return m_strErrorMessage.c_str();
}
