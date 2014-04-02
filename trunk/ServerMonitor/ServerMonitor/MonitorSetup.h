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
typedef std::vector<std::string> SEND_COMMAND_LIST;

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
	SEND_COMMAND_LIST m_clsSendCommandList;

	std::string m_strErrorMessage;
	int m_iCurrentCommandIndex;
};

extern CMonitorSetup gclsMonitorSetup;
