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

#include "SipCallBack.h"
#include "HttpCallBack.h"
#include "UserMap.h"
#include "CallMap.h"
#include "MemoryDebug.h"

CSipUserAgent gclsSipStack;
CSipCallBack gclsSipCallBack;

std::string gstrLocalIp;

CSipCallBack::CSipCallBack()
{
}

CSipCallBack::~CSipCallBack()
{
}

void CSipCallBack::EventRegister( CSipServerInfo * pclsInfo, int iStatus )
{
	CUserInfo clsUserInfo;

	if( gclsUserMap.Select( pclsInfo->m_strUserId.c_str(), clsUserInfo ) )
	{
		gclsHttpCallBack.Send( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort, "res|register|%d", iStatus );

		if( iStatus != SIP_OK )
		{
			gclsSipStack.DeleteRegisterInfo( *pclsInfo );
			gclsUserMap.Delete( pclsInfo->m_strUserId.c_str() );
		}
	}
}

void CSipCallBack::EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
{
	CUserInfo clsUserInfo;

	if( gclsUserMap.Select( pszTo, clsUserInfo ) )
	{
		if( gclsCallMap.SelectUserId( pszTo ) )
		{
			gclsSipStack.StopCall( pszCallId, SIP_BUSY_HERE );
		}
		else if( gclsCallMap.Insert( pszCallId, pszTo ) )
		{
			gclsCallMap.Update( pszCallId, pclsRtp->m_strIp.c_str(), pclsRtp->m_iPort );

			if( gclsHttpCallBack.Send( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort, "req|invite|%s", pszFrom ) == false )
			{
				gclsCallMap.Delete( pszCallId );
				gclsSipStack.StopCall( pszCallId, SIP_INTERNAL_SERVER_ERROR );
			}
		}
		else
		{
			gclsSipStack.StopCall( pszCallId, SIP_INTERNAL_SERVER_ERROR );
		}
	}
	else
	{
		gclsSipStack.StopCall( pszCallId, SIP_NOT_FOUND );
	}
}

void CSipCallBack::EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{
}

void CSipCallBack::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	CCallInfo clsCallInfo;
	CUserInfo clsUserInfo;

	if( gclsCallMap.Select( pszCallId, clsCallInfo ) && gclsUserMap.Select( clsCallInfo.m_strUserId.c_str(), clsUserInfo ) )
	{
		gclsCallMap.Update( pszCallId, pclsRtp->m_strIp.c_str(), pclsRtp->m_iPort );
		gclsHttpCallBack.Send( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort, "res|invite|200" );
	}
}

void CSipCallBack::EventCallEnd( const char * pszCallId, int iSipStatus )
{
	CCallInfo clsCallInfo;
	CUserInfo clsUserInfo;

	if( gclsCallMap.Select( pszCallId, clsCallInfo ) && gclsUserMap.Select( clsCallInfo.m_strUserId.c_str(), clsUserInfo ) )
	{
		if( iSipStatus <= SIP_OK )
		{
			gclsHttpCallBack.Send( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort, "req|bye" );
		}
		else
		{
			gclsHttpCallBack.Send( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort, "res|invite|%d", iSipStatus );
		}

		if( clsUserInfo.m_pclsRtpArg )
		{
			clsUserInfo.m_pclsRtpArg->m_bStop = true;
		}

		gclsCallMap.Delete( pszCallId );
	}
}
