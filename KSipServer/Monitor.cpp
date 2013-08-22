#include "Monitor.h"
#include "SipTcp.h"
#include "CallMap.h"
#include "SipServerMap.h"
#include "UserMap.h"
#include "RtpMap.h"
#include "MonitorDefine.h"
#include "SipServer.h"
#include "SipServerSetup.h"

CMonitor gclsMonitor;

CMonitor::CMonitor()
{
}

CMonitor::~CMonitor()
{
}

bool CMonitor::RecvRequest( const char * pszRequest, std::string & strResponse )
{
	if( !strcmp( pszRequest, MC_CALL_MAP_LIST ) )
	{
		gclsCallMap.GetString( strResponse );
	}
	else if( !strcmp( pszRequest, MC_SIP_SERVER_MAP_LIST ) )
	{
		gclsSipServerMap.GetString( strResponse );
	}
	else if( !strcmp( pszRequest, MC_USER_MAP_LIST ) )
	{
		gclsUserMap.GetString( strResponse );
	}
	else if( !strcmp( pszRequest, MC_RTP_MAP_LIST ) )
	{
		gclsRtpMap.GetString( strResponse );
	}
	else if( !strcmp( pszRequest, MC_DIALOG_MAP_LIST ) )
	{
		gclsUserAgent.GetString( strResponse );
	}
	else if( !strcmp( pszRequest, MC_SIP_STACK_COUNT_LIST ) )
	{
		gclsSipStack.GetString( strResponse );
	}
	else
	{
		return false;
	}

	return true;
}

bool CMonitor::IsMonitorIp( const char * pszIp )
{
	return gclsSetup.IsMonitorIp( pszIp );
}
