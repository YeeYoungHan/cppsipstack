

#ifndef _TCP_SESSION_LIST_H_
#define _TCP_SESSION_LIST_H_

#include "SipTcp.h"
#include "SipBuffer.h"
#include "TcpThreadList.h"
#include <vector>
#include <string>

/**
 * @brief TCP �����忡 TCP ������ ������ ���� ���Ǵ� Ŭ����
 */
class CTcpComm
{
public:
	Socket	m_hSocket;
	char		m_szIp[16];
	int			m_iPort;
};

/**
 * @brief TCP ������ ���� �����ϴ� TCP ���� ����
 */
class CTcpSessionListInfo
{
public:
	CTcpSessionListInfo();
	void Clear();

	std::string		m_strIp;
	int						m_iPort;
	//SSL						* m_psttSsl;

	CSipBuffer		m_clsSipBuf;

	time_t				m_iConnectTime;
	time_t				m_iRecvTime;
};

typedef std::vector< CTcpSessionListInfo > SESSION_LIST;

/**
 * @brief TCP ������ ���� �����ϴ� TCP ���� ������ �����ϴ� Ŭ����
 */
class CTcpSessionList
{
public:
	CTcpSessionList(void);
	~CTcpSessionList(void);

	bool Init( int iPollFdMax );
	bool Insert( Socket hSocket );
	bool Delete( int iIndex, CThreadListEntry * pclsEntry );
	void DeleteTimeout( int iTimeout, CThreadListEntry * pclsEntry );

	struct pollfd * m_psttPollFd;
	SESSION_LIST	m_clsList;

	int	m_iPollFdMax;
	int m_iPoolFdCount;
};

#endif
