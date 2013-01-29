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

#include "SipPlatformDefine.h"
#include "RoomMap.h"
#include "McuControlServer.h"

CRoomMap gclsRoomMap;

CRoomMap::CRoomMap() : m_iRoomIndex(0)
{
}

CRoomMap::~CRoomMap()
{
}

bool CRoomMap::GetNewRoomId( std::string & strRoomId )
{
	bool bRes = false;
	char szTemp[11];

	m_clsMutex.acquire();
	strRoomId = MCU_CONFERENCE_PREFIX;
	strRoomId.append( "999" );

	snprintf( szTemp, sizeof(szTemp), "%d", m_iRoomIndex );
	strRoomId.append( szTemp );

	++m_iRoomIndex;
	bRes = true;
	m_clsMutex.release();

	return bRes;
}
