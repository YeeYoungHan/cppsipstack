#ifndef _ROOM_MAP_H_
#define _ROOM_MAP_H_

#include <string>
#include <map>
#include "SipMutex.h"

class CRoomInfo
{
public:

};

// key = room id number
typedef std::map< std::string, CRoomInfo > ROOM_MAP;

class CRoomMap
{
public:
	CRoomMap();
	~CRoomMap();

	bool GetNewRoomId( std::string & strRoomId );	

private:
	ROOM_MAP	m_clsMap;
	CSipMutex	m_clsMutex;
	int				m_iRoomIndex;
};

extern CRoomMap gclsRoomMap;

#endif
