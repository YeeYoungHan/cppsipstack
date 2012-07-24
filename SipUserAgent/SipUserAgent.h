#ifndef _SIP_USER_AGENT_H_
#define _SIP_USER_AGENT_H_

#include "SipServerInfo.h"
#include "SipStack.h"

class CSipUserAgent : ISipStackCallBack
{
public:
	CSipUserAgent(void);
	~CSipUserAgent(void);

	bool AddRegisterInfo( CSipServerInfo & clsInfo );

	bool Start( CSipStackSetup & clsSetup );
	bool Stop( );

	virtual bool RecvRequest( int iThreadId, CSipMessage * pclsMessage );
	virtual bool RecvResponse( int iThreadId, CSipMessage * pclsMessage );

	SIP_SERVER_INFO_LIST	m_clsRegisterList;
	CSipStack	m_clsSipStack;
};

#endif
