#include "SipUserAgent.h"

CSipUserAgent::CSipUserAgent(void)
{
}

CSipUserAgent::~CSipUserAgent(void)
{
}

bool CSipUserAgent::AddRegisterInfo( CSipServerInfo & clsInfo )
{
	if( clsInfo.m_strIp.empty() ) return false;
	if( clsInfo.m_strUserId.empty() ) return false;

	if( clsInfo.m_strDomain.empty() )
	{
		clsInfo.m_strDomain = clsInfo.m_strIp;
	}

	m_clsRegisterList.push_back( clsInfo );

	return true;
}

bool CSipUserAgent::Start( CSipStackSetup & clsSetup )
{
	if( m_clsRegisterList.size() == 0 ) return false;

	m_clsSipStack.AddCallBack( this );

	return m_clsSipStack.Start( clsSetup );
}

bool CSipUserAgent::Stop( )
{

	return false;
}

bool CSipUserAgent::RecvRequest( int iThreadId, CSipMessage * pclsMessage )
{

	return false;
}

bool CSipUserAgent::RecvResponse( int iThreadId, CSipMessage * pclsMessage )
{

	return false;
}
