#pragma once

#include <string>
#include <vector>
#include <list>

typedef std::list<std::string> MONITOR_ATTRIBUTE_LIST;

class CMonitorEntry
{
public:
	std::string	m_strCommand;
	std::string	m_strTitle;

	MONITOR_ATTRIBUTE_LIST m_lstAttribute;
};

typedef std::vector<CMonitorEntry> MONITOR_LIST;

class CMonitorSetup
{
public:
	CMonitorSetup();
	~CMonitorSetup();

	bool Read( const char * pszFileName );
	bool Select( const char * pszCommand, CMonitorEntry & clsEntry );
	int GetCount( );

	const char * GetNextCommand( );
	const char * GetErrorMessage( );

	MONITOR_LIST m_clsList;
	std::string m_strErrorMessage;
	int m_iCurrentCommandIndex;
};

extern CMonitorSetup gclsMonitorSetup;
