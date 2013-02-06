#ifndef _CALL_MAP_H_
#define _CALL_MAP_H_

#include <map>
#include <string>
#include "SipMessage.h"
#include "SipMutex.h"

class CRtpInfo
{
public:
	CRtpInfo() : m_iAudioPort(0), m_iVideoPort(0)
	{}

	int		m_iAudioPort;
	int		m_iVideoPort;
};

class CCallInfo
{
public:
	CRtpInfo	m_clsLocalRtp;
	CRtpInfo	m_clsRemoteRtp;

	std::string	m_strRoomId;
};

// key ´Â SIP Call-ID ÀÌ´Ù.
typedef std::map< std::string, CCallInfo > CALL_MAP;

class CCallMap
{
public:
	CCallMap();
	~CCallMap();

	bool Insert( CSipMessage * pclsMessage );
	bool Select( CSipMessage * pclsMessage, CCallInfo & clsCallInfo );
	bool Delete( CSipMessage * pclsMessage );

private:
	CSipMutex	m_clsMutex;
	CALL_MAP	m_clsMap;

	void GetKey( CSipMessage * pclsMessage, std::string & strKey );
};

#endif
