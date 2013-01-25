#ifndef _SIP_SERVER_MAP_H_
#define _SIP_SERVER_MAP_H_

#include "SipMutex.h"
#include <vector>

class CSipServerInfo
{
public:
	std::string m_strIp;
};

typedef std::vector< CSipServerInfo > SIP_SERVER_LIST;

class CSipServerMap
{
public:
	CSipServerMap();
	~CSipServerMap();

	bool Insert( const char * pszIp );
	bool SelectNext( CSipServerInfo & clsInfo );

private:
	SIP_SERVER_LIST m_clsList;
	CSipMutex m_clsMutex;
	int				m_iIndex;
};

extern CSipServerMap gclsSipServerMap;

#endif
