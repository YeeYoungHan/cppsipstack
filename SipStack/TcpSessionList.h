

#ifndef _TCP_SESSION_LIST_H_
#define _TCP_SESSION_LIST_H_

#include "SipTcp.h"
#include "SipBuffer.h"
#include "TcpThreadList.h"
#include <vector>
#include <string>

/**
 * @brief TCP 쓰레드에 TCP 세션을 전달할 때에 사용되는 클래스
 */
class CTcpComm
{
public:
	Socket	m_hSocket;
	char		m_szIp[16];
	int			m_iPort;
};

/**
 * @brief TCP 쓰레드 별로 관리하는 TCP 세션 정보
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
 * @brief TCP 쓰레드 별로 관리하는 TCP 세션 정보를 저장하는 클래스
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
