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

#include "CallMap.h"

CCallMap gclsCallMap;

void CCallInfo::Print()
{
	printf( "Call-ID(%s) From(%s) To(%s) RTP(%s:%d=>%s:%d) result(%s) message(%s) send(%s)\n"
		, m_strCallId.c_str(), m_strFrom.c_str(), m_strTo.c_str()
		, m_clsFromRtp.m_strIp.c_str(), m_clsFromRtp.m_iPort
		, m_clsToRtp.m_strIp.c_str(), m_clsToRtp.m_iPort
		, m_bFaxError == false ? "ok" : "error"
		, m_strFaxErrorMessage.c_str()
		, m_bSend ? "true" : "false" );
}

bool CCallMap::Insert( CCallInfo & clsInfo )
{
	CALL_MAP::iterator itMap;
	bool bRes = false;

	itMap = m_clsMap.find( clsInfo.m_strCallId );
	if( itMap == m_clsMap.end() )
	{
		m_clsMap.insert( CALL_MAP::value_type( clsInfo.m_strCallId, clsInfo ) );
		bRes = true;
	}

	return bRes;
}

bool CCallMap::Update( CCallInfo & clsInfo )
{
	CALL_MAP::iterator itMap;
	bool bRes = false;

	itMap = m_clsMap.find( clsInfo.m_strCallId );
	if( itMap != m_clsMap.end() )
	{
		itMap->second = clsInfo;
		bRes = true;
	}

	return bRes;
}

bool CCallMap::Select( std::string & strCallId, CCallInfo & clsInfo )
{
	CALL_MAP::iterator itMap;
	bool bRes = false;

	itMap = m_clsMap.find( strCallId );
	if( itMap != m_clsMap.end() )
	{
		clsInfo = itMap->second;
		bRes = true;
	}

	return bRes;
}

bool CCallMap::Delete( std::string & strCallId )
{
	CALL_MAP::iterator itMap;
	bool bRes = false;

	itMap = m_clsMap.find( strCallId );
	if( itMap != m_clsMap.end() )
	{
		itMap->second.Print();
		m_clsMap.erase( itMap );
		bRes = true;
	}

	return bRes;
}

bool CCallMap::UpdateWithRtpPort( int iRtpPort, bool bFaxError, std::string & strErrorMessage )
{
	CALL_MAP::iterator itMap;

	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		if( itMap->second.m_clsFromRtp.m_iPort == iRtpPort || itMap->second.m_clsToRtp.m_iPort == iRtpPort )
		{
			itMap->second.m_bFaxError = bFaxError;
			itMap->second.m_strFaxErrorMessage = strErrorMessage;
			return true;
		}
	}

	return false;
}
