/* 
 * Copyright (C) 2021 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
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

#include "CallMap.h"

CCallMap gclsCallMap;

void CCallMap::Insert( std::string & strCallId, CSipMessage & clsMessage )
{
	CALL_MAP::iterator itMap;
	SIP_VIA_LIST::iterator itVia;
	CCallInfo clsCallInfo;

	clsMessage.m_clsCallId.ToString( clsCallInfo.m_strRecvCallId );

	itVia = clsMessage.m_clsViaList.begin();
	if( itVia != clsMessage.m_clsViaList.end() )
	{
		const char * pszBranch = itVia->SelectParamValue( "branch" );
		if( pszBranch )
		{
			clsCallInfo.m_strBranch = pszBranch;
		}
	}

	itMap = m_clsMap.find( strCallId );
	if( itMap == m_clsMap.end() )
	{
		m_clsMap.insert( CALL_MAP::value_type( strCallId, clsCallInfo ) );
	}
	else
	{
		itMap->second = clsCallInfo;
	}
}

bool CCallMap::Select( std::string & strCallId, CCallInfo & clsCallInfo )
{
	CALL_MAP::iterator itMap;

	itMap = m_clsMap.find( strCallId );
	if( itMap != m_clsMap.end() )
	{
		clsCallInfo = itMap->second;
		return true;
	}

	return false;
}
