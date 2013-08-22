/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

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
