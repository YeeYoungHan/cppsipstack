#ifndef _CALL_MAP_H_
#define _CALL_MAP_H_

#include <string>
#include <map>
#include "SipMutex.h"
#include "SipMessage.h"

class CCallInfo
{
public:
	CCallInfo() : m_bRecvFromDevice(false), m_bInviteToMCU(false)
	{}

	std::string	m_strRoomId;
	std::string m_strToId;
	CSipMessage m_clsSipMessage;
	bool				m_bRecvFromDevice;
	bool				m_bInviteToMCU;
};

// key = SIP Call-Id
typedef std::map< std::string, CCallInfo > CALL_MAP;

class CCallMap
{
public:
	CCallMap();
	~CCallMap();

	bool Insert( CSipMessage * pclsMessage );
	bool Insert( CSipMessage * pclsMessage, std::string & strRoomId );
	bool Update( CSipMessage * pclsMessage );
	bool Delete( CSipMessage * pclsMessage );
	bool Select( CSipMessage * pclsMessage, CCallInfo & clsCallInfo );

private:
	CALL_MAP	m_clsMap;
	CSipMutex	m_clsMutex;
};

extern CCallMap gclsCallMap;

#endif
