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

#include "SipClient.h"
#include "SipSpeedSetup.h"
#include "SipSpeedLinux.h"

CSipClient::CSipClient() : m_bCallerLogin(false), m_bCalleeLogin(false)
	, m_iCallSuccess(0), m_iCallError(0)
{
}

CSipClient::~CSipClient()
{
}

void CSipClient::EventRegister( CSipServerInfo * pclsInfo, int iStatus )
{
	std::string	strCommand;

	if( pclsInfo->m_bLogin )
	{
		strCommand = "login";
	}
	else
	{
		strCommand = "logout";
	}

	if( !strcmp( pclsInfo->m_strUserId.c_str(), gclsSetup.m_strCallerId.c_str() ) )
	{
		if( iStatus == SIP_OK )
		{
			if( pclsInfo->m_bLogin )
			{
				m_bCallerLogin = true;
			}
			else
			{
				m_bCallerLogin = false;
			}

			printf( "Caller(%s) %s success\n", gclsSetup.m_strCallerId.c_str(), strCommand.c_str() );
		}
		else
		{
			m_bCallerLogin = false;
			printf( "Caller(%s) %s error(%d)\n", gclsSetup.m_strCallerId.c_str(), strCommand.c_str(), iStatus );
		}
	}
	else if( !strcmp( pclsInfo->m_strUserId.c_str(), gclsSetup.m_strCalleeId.c_str() ) )
	{
		if( iStatus == SIP_OK )
		{
			if( pclsInfo->m_bLogin )
			{
				m_bCalleeLogin = true;
			}
			else
			{
				m_bCalleeLogin = false;
			}

			printf( "Callee(%s) %s success\n", gclsSetup.m_strCalleeId.c_str(), strCommand.c_str() );
		}
		else
		{
			m_bCalleeLogin = false;
			printf( "Callee(%s) %s error(%d)\n", gclsSetup.m_strCalleeId.c_str(), strCommand.c_str(), iStatus );
		}
	}
}

void CSipClient::EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
{
	CSipCallRtp clsRtp;

	clsRtp.m_strIp = gclsSipUserAgent.m_clsSipStack.m_clsSetup.m_strLocalIp;
	clsRtp.m_iPort = 4002;	
	clsRtp.m_iCodec = 0;

	gclsSipUserAgent.AcceptCall( pszCallId, &clsRtp );
}

void CSipClient::EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{

}

void CSipClient::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	bool bModify = false;

	m_clsMutex.acquire();
	++m_iCallSuccess;
	m_clsMutex.release();

	gclsSipUserAgent.StopCall( pszCallId );
}

void CSipClient::EventCallEnd( const char * pszCallId, int iSipStatus )
{
	if( iSipStatus != SIP_OK )
	{
		m_clsMutex.acquire();
		++m_iCallError;
		m_clsMutex.release();
	}
}
