#ifndef _SIP_REGISTER_INFO_H_
#define _SIP_REGISTER_INFO_H_

#include "SipMessage.h"

class CSipServerInfo
{
public:
	CSipServerInfo(void);
	~CSipServerInfo(void);

	void ClearLogin();

	std::string		m_strIp;
	int						m_iPort;

	std::string		m_strDomain;
	std::string		m_strUserId;
	std::string		m_strPassWord;

	int						m_iLoginTimeout;

	bool					m_bLogin;
	time_t				m_iLoginTime;
	time_t				m_iSendTime;
	std::string		m_strCallId;

	CSipMessage * GetRegisterMessage( CSipMessage * pclsResponse );
};

typedef std::list< CSipServerInfo > SIP_SERVER_INFO_LIST;

#endif
