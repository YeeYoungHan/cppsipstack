#ifndef _SIP_USER_AGENT_MFC_EVENT_CLASS_H_
#define _SIP_USER_AGENT_MFC_EVENT_CLASS_H_

class CEventRegister
{
public:
	CEventRegister( CSipServerInfo * pclsInfo, int iStatus ) : m_pclsInfo(pclsInfo), m_iStatus(iStatus)
	{
	}

	CSipServerInfo * m_pclsInfo;
	int		m_iStatus;
};

#endif
