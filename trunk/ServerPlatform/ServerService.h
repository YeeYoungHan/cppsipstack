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

#ifndef _SERVER_SERVICE_H_
#define _SERVER_SERVICE_H_

#include <string>
#include "MonitorCallBack.h"

/**
 * @ingroup ServerPlatform
 * @brief MS ������ ���� ���� ���� ���� Ŭ����
 */
class CServerService
{
public:
	void SetBuildDate( const char * pszDate, const char * pszTime )
	{
		m_strBuildDate = pszDate;
		m_strBuildDate.append( " " );
		m_strBuildDate.append( pszTime );
	}

	/** ���� �̸� */
	std::string m_strName;

	/** ���� display �̸� */
	std::string m_strDisplayName;

	/** ���� ���� */
	std::string m_strDescription;

	/** ���� ���� */
	std::string m_strVersion;

	/** ���� ���� ���� �̸� - ���� ���α׷� ���� ������ ����� ���� �̸� */
	std::string m_strConfigFileName;

	/** ���� ���� ��¥ */
	std::string	m_strBuildDate;
};

typedef int (*ServerFunc)();

// ServerSignal.h
extern bool gbStop;

int ServerMain( int argc, char * argv[], CServerService & clsService, ServerFunc pFunc );
const char * GetConfigFileName();
void ServerSignal();

bool IsMonitorThreadRun();

// ServerThread.cpp
bool StartMonitorServerThread( IMonitorCallBack * pclsCallBack );

#endif
