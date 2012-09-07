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

#include "SipServer.h"
#include "CallMap.h"

CCallMap gclsCallMap;
CCallMap gclsTransCallMap;

CCallInfo::CCallInfo() : m_bRecv(false)
{
}

CCallMap::CCallMap()
{
}

CCallMap::~CCallMap()
{
}

/**
 * @ingroup KSipServer
 * @brief 통화 요청 Call-ID 와 전달된 통화 요청 Call-ID 를 자료구조에 저장한다.
 * @param pszRecvCallId 통화 요청 Call-ID
 * @param pszSendCallId 전달된 통화 요청 Call-ID
 * @returns true 를 리턴한다.
 */
bool CCallMap::Insert( const char * pszRecvCallId, const char * pszSendCallId )
{
	CALL_MAP::iterator	itMap;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszRecvCallId );
	if( itMap == m_clsMap.end() )
	{
		CCallInfo	clsCallInfo;

		clsCallInfo.m_strPeerCallId = pszSendCallId;
		clsCallInfo.m_bRecv = true;
		m_clsMap.insert( CALL_MAP::value_type( pszRecvCallId, clsCallInfo ) );
	}

	itMap = m_clsMap.find( pszSendCallId );
	if( itMap == m_clsMap.end() )
	{
		CCallInfo	clsCallInfo;

		clsCallInfo.m_strPeerCallId = pszRecvCallId;
		clsCallInfo.m_bRecv = false;
		m_clsMap.insert( CALL_MAP::value_type( pszSendCallId, clsCallInfo ) );
	}
	m_clsMutex.release();

	return true;
}

/**
 * @ingroup KSipServer
 * @brief Call-ID 와 연결된 Call-ID 를 검색한다.
 * @param pszCallId SIP Call-ID
 * @param strCallId 연결된 SIP Call-ID
 * @returns 검색되면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CCallMap::Select( const char * pszCallId, std::string & strCallId )
{
	CALL_MAP::iterator	itMap;
	bool	bRes = false;

	strCallId.clear();

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		strCallId = itMap->second.m_strPeerCallId;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

bool CCallMap::Select( const char * pszCallId, CCallInfo & clsCallInfo )
{
	CALL_MAP::iterator	itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		clsCallInfo = itMap->second;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief Call-ID 와 연결된 Call-ID 를 자료구조에서 삭제한다.
 * @param pszCallId SIP Call-ID
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CCallMap::Delete( const char * pszCallId )
{
	CALL_MAP::iterator	itMap;
	bool	bRes = false;
	std::string strCallId;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		strCallId = itMap->second.m_strPeerCallId;
		m_clsMap.erase( itMap );
		bRes = true;
	}

	if( bRes )
	{
		itMap = m_clsMap.find( strCallId );
		if( itMap != m_clsMap.end() )
		{
			m_clsMap.erase( itMap );
		}
	}
	m_clsMutex.release();

	return bRes;
}

void CCallMap::StopCallAll()
{
	CALL_MAP::iterator	itMap;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		gclsUserAgent.StopCall( itMap->first.c_str() );
	}
	m_clsMutex.release();
}

int CCallMap::GetCount()
{
	int iCount;

	m_clsMutex.acquire();
	iCount = m_clsMap.size();
	m_clsMutex.release();

	return iCount;
}
