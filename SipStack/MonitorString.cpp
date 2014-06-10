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

#include "SipStackDefine.h"
#include "MonitorString.h"
#include "TimeString.h"
#include "MemoryDebug.h"

CMonitorString::CMonitorString()
{
}

CMonitorString::~CMonitorString()
{
}

void CMonitorString::AddCol( const char * pszValue )
{
	m_strBuf.append( pszValue );
	m_strBuf.append( MR_COL_SEP );
}

void CMonitorString::AddCol( const std::string & strValue )
{
	m_strBuf.append( strValue );
	m_strBuf.append( MR_COL_SEP );
}

void CMonitorString::AddCol( const std::string & strIp, int iPort )
{
	m_strBuf.append( strIp );
	m_strBuf.append( ":" );
	AddCol( iPort );
}

void CMonitorString::AddCol( int iValue )
{
	char	szValue[21];

	snprintf( szValue, sizeof(szValue), "%d", iValue );
	AddCol( szValue );
}

void CMonitorString::AddCol( time_t iTime )
{
	char	szTime[21];

	GetDateTimeString( iTime, szTime, sizeof(szTime) );
	AddCol( szTime );
}

void CMonitorString::AddRow( const char * pszValue )
{
	m_strBuf.append( pszValue );
	m_strBuf.append( MR_ROW_SEP );
}

void CMonitorString::AddRow( const std::string & strValue )
{
	m_strBuf.append( strValue );
	m_strBuf.append( MR_ROW_SEP );
}

void CMonitorString::AddRow( const std::string & strIp, int iPort )
{
	m_strBuf.append( strIp );
	m_strBuf.append( ":" );
	AddRow( iPort );
}

void CMonitorString::AddRow( int iValue )
{
	char	szValue[21];

	snprintf( szValue, sizeof(szValue), "%d", iValue );
	AddRow( szValue );
}

void CMonitorString::AddRow( time_t iTime )
{
	char	szTime[21];

	GetDateTimeString( iTime, szTime, sizeof(szTime) );
	AddRow( szTime );
}

void CMonitorString::Clear( )
{
	m_strBuf.clear();
}

const char * CMonitorString::GetString()
{
	return m_strBuf.c_str();
}

int CMonitorString::GetLength()
{
	return m_strBuf.length();
}
