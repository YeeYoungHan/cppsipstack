#ifndef _USER_MAP_H_
#define _USER_MAP_H_

#include "SipMutex.h"
#include "SipMessage.h"
#include <map>

class CUserInfo
{
public:
	std::string m_strSipServerIp;
};

typedef std::map< std::string, CUserInfo > USER_MAP;


class CUserMap
{
public:
	CUserMap();
	~CUserMap();

	bool Select( CSipMessage * pclsRequest, CUserInfo & clsUserInfo );

private:
	USER_MAP	m_clsMap;
	CSipMutex m_clsMutex;
};

extern CUserMap gclsUserMap;

#endif
