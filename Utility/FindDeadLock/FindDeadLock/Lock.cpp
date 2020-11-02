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

#include "Lock.h"

CLock::CLock( const char * pszFileName ) : m_strFileName(pszFileName)
{
}

CLock::~CLock()
{
}

bool CLock::Add( const char * pszVar, int iLine )
{
	LOCK_LIST::iterator itLL;

	for( itLL = m_clsLockList.begin(); itLL != m_clsLockList.end(); ++itLL )
	{
		if( !strcmp( itLL->m_strVar.c_str(), pszVar ) )
		{
			printf( "file(%s) line(%d) : It is a mutex that is already locked at line(%d)\n", m_strFileName.c_str(), iLine, itLL->m_iLockLine );
			return false;
		}
	}

	CLockInfo clsInfo;

	clsInfo.m_strVar = pszVar;
	clsInfo.m_iLockLine = iLine;

	m_clsLockList.push_back( clsInfo );

	return true;
}

bool CLock::Del( const char * pszVar, int iLine )
{
	LOCK_LIST::reverse_iterator itLL;

	itLL = m_clsLockList.rbegin();
	if( itLL == m_clsLockList.rend() )
	{
		printf( "file(%s) line(%d) : It is not a locked mutex\n", m_strFileName.c_str(), iLine );
		return false;
	}

	if( strcmp( itLL->m_strVar.c_str(), pszVar ) )
	{
		printf( "file(%s) line(%d) : It is not the last locked mutex - last locked mutex line(%d)\n", m_strFileName.c_str(), iLine, itLL->m_iLockLine );
		return false;
	}

	m_clsLockList.pop_back();

	return true;
}

void CLock::Print( )
{
	LOCK_LIST::iterator itLL;

	for( itLL = m_clsLockList.begin(); itLL != m_clsLockList.end(); ++itLL )
	{
		printf( "file(%s) line(%d) : It is a unlocked mutex\n", m_strFileName.c_str(), itLL->m_iLockLine );
	}
}
