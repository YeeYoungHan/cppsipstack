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
std::string gstrLocalIpPrefix;

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

}

void CSipCallBack::EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{
	CCallInfo clsCallInfo;
	CUserInfo clsUserInfo;

	if( gclsCallMap.Select( pszCallId, clsCallInfo ) && gclsUserMap.Select( clsCallInfo.m_strUserId.c_str(), clsUserInfo ) )
	{
		gclsHttpCallBack.Send( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort, "res|invite|180" );
	}
}

void CSipCallBack::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	CCallInfo clsCallInfo;
	CUserInfo clsUserInfo;

	if( gclsCallMap.Select( pszCallId, clsCallInfo ) && gclsUserMap.Select( clsCallInfo.m_strUserId.c_str(), clsUserInfo ) )
	{
		std::string strSdp;

		MakeSdp( pclsRtp, strSdp );

		gclsHttpCallBack.Send( clsUserInfo.m_strIp.c_str(), clsUserInfo.m_iPort, "res|invite|200|%s", strSdp.c_str() );
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
	}
}

bool CSipCallBack::MakeSdp( CSipCallRtp * pclsRtp, std::string & strSdp )
{
	char	szSdp[4096];
	int		iLen = 0;
	const char * pszAddrType = "IP4";

	iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "v=0\r\n"
					"o=CSS 4 2 IN IP4 %s\r\n"
					"s=CSS\r\n", pszAddrType, pclsRtp->m_strIp.c_str() );

	iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "c=IN %s %s\r\n", pszAddrType, pclsRtp->m_strIp.c_str() );
	iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "t=0 0\r\n" );

	switch( pclsRtp->m_iCodec )
	{
	case 0:
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "m=audio %d RTP/AVP 0\r\n", pclsRtp->m_iPort );
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:0 PCMU/8000\r\n" );
		break;
	case 8:
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "m=audio %d RTP/AVP 8\r\n", pclsRtp->m_iPort );
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:8 PCMA/8000\r\n" );
		break;
	}

	iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=sendrecv\r\n" );

	strSdp = szSdp;
	
	return true;
}