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
