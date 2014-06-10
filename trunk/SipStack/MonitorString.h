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

#ifndef _MONITOR_STRING_H_
#define _MONITOR_STRING_H_

#include <string>

class CMonitorString
{
public:
	CMonitorString();
	~CMonitorString();

	void AddCol( const char * pszValue );
	void AddCol( const std::string & strValue );
	void AddCol( const std::string & strIp, int iPort );
	void AddCol( int iValue );
	void AddCol( time_t iTime );

	void AddRow( const char * pszValue );
	void AddRow( const std::string & strValue );
	void AddRow( const std::string & strIp, int iPort );
	void AddRow( int iValue );
	void AddRow( time_t iTime );

	void Clear( );

	const char * GetString();
	int GetLength();

private:
	std::string m_strBuf;
};

#endif
