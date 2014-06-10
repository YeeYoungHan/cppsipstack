#ifndef _TCP_CONNECT_MAP_H_
#define _TCP_CONNECT_MAP_H_

#include "SipMessage.h"
#include "SipMutex.h"
#include <map>

typedef std::list< CSipMessage * > SIP_MESSAGE_LIST;

class CTcpConnectInfo
{
public:
	SIP_MESSAGE_LIST	m_clsSipMessageList;
};

// key = ip:port
typedef std::map< std::string, CTcpConnectInfo > TCP_CONNECT_MAP;

class CTcpConnectMap
{
public:
	CTcpConnectMap();
	~CTcpConnectMap();

	bool Insert( const char * pszIp, int iPort );
	bool Insert( const char * pszIp, int iPort, CSipMessage * pclsMessage );
	bool Delete( const char * pszIp, int iPort, SIP_MESSAGE_LIST & clsList );
	bool Delete( const char * pszIp, int iPort );

private:
	TCP_CONNECT_MAP m_clsMap;
	CSipMutex				m_clsMutex;

	void GetKey( const char * pszIp, int iPort, std::string & strKey );
};

#endif
