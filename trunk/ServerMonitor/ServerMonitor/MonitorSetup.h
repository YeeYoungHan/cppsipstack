#pragma once

#include <string>
#include <list>

typedef std::list<std::string> MONITOR_ATTRIBUTE_LIST;

class CMonitorEntry
{
public:
	std::string	m_strCommand;
	std::string	m_strTitle;

	MONITOR_ATTRIBUTE_LIST m_lstAttribute;
};

typedef std::list<CMonitorEntry> MONITOR_LIST;

class CMonitorSetup
{
public:
	CMonitorSetup();
	~CMonitorSetup();

	bool Read( const char * pszFileName );
	const char * GetErrorMessage( );

	MONITOR_LIST m_clsList;
	std::string m_strErrorMessage;
};
