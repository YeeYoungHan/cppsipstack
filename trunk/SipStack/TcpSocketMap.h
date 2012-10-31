

#ifndef _TCP_SOCKET_MAP_H_
#define _TCP_SOCKET_MAP_H_

#include "SipTcp.h"
#include <string>
#include <map>
#include "SipMutex.h"

/**
 * @brief TCP/TLS ���� ����
 */
class CTcpSocketInfo
{
public:
	CTcpSocketInfo();

	Socket	m_hSocket;
};

typedef std::map< std::string, CTcpSocketInfo > TCP_SOCKET_MAP;

/**
 * @brief TCP/TLS ���� ������ �����ϴ� �ڷᱸ��
 */
class CTcpSocketMap
{
public:
	CTcpSocketMap(void);
	~CTcpSocketMap(void);

	bool Insert( const char * pszIp, int iPort, Socket hSocket );
	bool Select( const char * pszIp, int iPort, Socket & hSocket );
	bool Delete( const char * pszIp, int iPort );
	void GetString( std::string & strText );

private:
	TCP_SOCKET_MAP	m_clsMap;
	CSipMutex				m_clsMutex;

	void GetKey( const char * pszIp, int iPort, std::string & strKey );
};

#endif
