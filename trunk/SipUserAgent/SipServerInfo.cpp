#include "SipServerInfo.h"
#include "SipCreateMessage.h"

CSipServerInfo::CSipServerInfo(void) : m_iPort(5060), m_iLoginTimeout(3600)
{
	ClearLogin();
}

CSipServerInfo::~CSipServerInfo(void)
{
}

void CSipServerInfo::ClearLogin()
{
	m_bLogin = false;
	m_iLoginTime = 0;
	m_iSendTime = 0;
	m_strCallId.clear();
}

CSipMessage * CSipServerInfo::GetRegisterMessage( CSipMessage * pclsResponse )
{
	CSipMessage * pclsRequest = new CSipMessage();
	if( pclsRequest == NULL ) return NULL;

	char	szTag[11], szBranch[51];

	SipMakeTag( szTag, sizeof(szTag) );
	SipMakeBranch( szBranch, sizeof(szBranch) );

	pclsRequest->m_clsReqUri.m_strProtocol = "sip";
	pclsRequest->m_clsReqUri.m_strHost = m_strDomain;

	pclsRequest->m_strSipMethod = "REGISTER";
	pclsRequest->m_strSipVersion = "SIP/2.0";

	pclsRequest->m_clsFrom.m_clsUri.m_strProtocol = "sip";
	pclsRequest->m_clsFrom.m_clsUri.m_strUser = m_strUserId;
	pclsRequest->m_clsFrom.m_clsUri.m_strHost = m_strDomain;

	pclsRequest->m_clsTo = pclsRequest->m_clsFrom;

	pclsRequest->m_clsFrom.AddParam( "tag", szTag );

	return pclsRequest;
}
