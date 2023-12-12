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

/**
 * @ingroup ServerMonitor
 * @brief ����͸� �÷� �����ִ� ���
 */
enum EMonitorAttributeType
{
	E_MAT_NULL = 0,
	E_MAT_COMMA_SEP
};

/**
 * @ingroup ServerMonitor
 * @brief �ϳ��� ����͸� �÷� ���� ���� Ŭ����
 */
class CMonitorAttribute
{
public:
	CMonitorAttribute() : m_eType(E_MAT_NULL)
	{
	}

	std::string						m_strName;
	EMonitorAttributeType	m_eType;
	std::string						m_strShowIfEqual;
	std::string						m_strShowIfNotEqual;
};

typedef std::vector< CMonitorAttribute > MONITOR_ATTRIBUTE_LIST;

/**
 * @ingroup ServerMonitor
 * @brief �ϳ��� ����͸� ��� ���� ���� Ŭ����
 */
class CMonitorEntry
{
public:
	CMonitorEntry() : m_bUpdateRow(false)
	{
	}

	EMonitorAttributeType GetType( int iCol )
	{
		if( iCol < 0 || iCol >= (int)m_lstAttribute.size() ) return E_MAT_NULL;

		return m_lstAttribute[iCol].m_eType;
	}

	bool GetShowIfEqual( int iCol, std::string & strText )
	{
		if( iCol < 0 || iCol >= (int)m_lstAttribute.size() ) return false;

		if( m_lstAttribute[iCol].m_strShowIfEqual.empty() ) return false;

		strText = m_lstAttribute[iCol].m_strShowIfEqual;

		return true;
	}

	bool GetShowIfNotEqual( int iCol, std::string & strText )
	{
		if( iCol < 0 || iCol >= (int)m_lstAttribute.size() ) return false;

		if( m_lstAttribute[iCol].m_strShowIfNotEqual.empty() ) return false;

		strText = m_lstAttribute[iCol].m_strShowIfNotEqual;

		return true;
	}

	std::string	m_strCommand;
	std::string	m_strTitle;
	bool m_bUpdateRow;

	MONITOR_ATTRIBUTE_LIST m_lstAttribute;
};

typedef std::vector<CMonitorEntry> MONITOR_LIST;
typedef std::vector<std::string> SEND_COMMAND_LIST;

/**
 * @ingroup ServerMonitor
 * @brief ����͸� ����� �����ϴ� �ڷᱸ�� Ŭ����
 */
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
