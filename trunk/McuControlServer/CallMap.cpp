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

#include "McuControlServer.h"
#include "CallMap.h"
#include "RoomMap.h"

CCallMap gclsCallMap;

CCallMap::CCallMap()
{
}

CCallMap::~CCallMap()
{
}

bool CCallMap::Insert( CSipMessage * pclsMessage )
{
	std::string strToId = pclsMessage->m_clsTo.m_clsUri.m_strUser;
	std::string strCallId;
	CALL_MAP::iterator	itMap;

	if( strToId.length() == 0 )
	{
		strToId = pclsMessage->m_clsTo.m_strDisplayName;
	}

	pclsMessage->m_clsCallId.ToString( strCallId );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strCallId );
	if( itMap == m_clsMap.end() )
	{
		CCallInfo	clsCallInfo;

		gclsRoomMap.GetNewRoomId( clsCallInfo.m_strRoomId );
		clsCallInfo.m_strToId = strToId;
		clsCallInfo.m_bRecvFromDevice = true;
		clsCallInfo.m_bInviteToMCU = true;
		clsCallInfo.m_clsSipMessage = *pclsMessage;

		m_clsMap.insert( CALL_MAP::value_type( strCallId, clsCallInfo ) );

		pclsMessage->m_clsReqUri.m_strUser = clsCallInfo.m_strRoomId;				
		pclsMessage->m_clsTo.m_clsUri.m_strUser = clsCallInfo.m_strRoomId;
	}
	m_clsMutex.release();

	return true;
}

bool CCallMap::Insert( CSipMessage * pclsMessage, std::string & strRoomId )
{
	std::string strFromId = pclsMessage->m_clsFrom.m_clsUri.m_strUser;
	std::string strCallId;
	CALL_MAP::iterator	itMap;

	if( strFromId.length() == 0 )
	{
		strFromId = pclsMessage->m_clsFrom.m_strDisplayName;
	}

	pclsMessage->m_clsCallId.ToString( strCallId );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strCallId );
	if( itMap == m_clsMap.end() )
	{
		CCallInfo	clsCallInfo;

		clsCallInfo.m_strRoomId = strRoomId;
		clsCallInfo.m_strToId = strFromId;
		clsCallInfo.m_bRecvFromDevice = false;
		clsCallInfo.m_bInviteToMCU = false;
		clsCallInfo.m_clsSipMessage = *pclsMessage;

		m_clsMap.insert( CALL_MAP::value_type( strCallId, clsCallInfo ) );
	}
	m_clsMutex.release();

	return true;
}

bool CCallMap::Update( CSipMessage * pclsMessage )
{
	std::string strCallId;
	bool bRes = false;
	CALL_MAP::iterator	itMap;

	pclsMessage->m_clsCallId.ToString( strCallId );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strCallId );
	if( itMap != m_clsMap.end() )
	{
		if( !strcmp( pclsMessage->m_strClientIp.c_str(), MCU_IP ) )
		{
			// MCU 로부터 수신한 경우
			if( pclsMessage->IsRequest() )
			{
				pclsMessage->m_clsReqUri.m_strUser = itMap->second.m_strToId;
			}

			pclsMessage->m_clsTo.m_clsUri.m_strUser = itMap->second.m_strToId;
		}
		else
		{
			// 영상 디바이스로부터 수신한 경우
			if( pclsMessage->IsRequest() )
			{
				pclsMessage->m_clsReqUri.m_strUser = itMap->second.m_strRoomId;				
			}

			pclsMessage->m_clsTo.m_clsUri.m_strUser = itMap->second.m_strRoomId;
		}

		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

bool CCallMap::Delete( CSipMessage * pclsMessage )
{
	bool bRes = false;

	return bRes;
}

bool CCallMap::Select( CSipMessage * pclsMessage, CCallInfo & clsCallInfo )
{
	std::string strCallId;
	bool bRes = false;
	CALL_MAP::iterator	itMap;

	pclsMessage->m_clsCallId.ToString( strCallId );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strCallId );
	if( itMap != m_clsMap.end() )
	{
		clsCallInfo = itMap->second;
		itMap->second.m_bRecvFromDevice = false;
		itMap->second.m_bInviteToMCU = true;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}
