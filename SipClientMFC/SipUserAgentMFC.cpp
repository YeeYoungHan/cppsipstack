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

#include "StdAfx.h"
#include "SipUserAgentMFC.h"

CSipUserAgentMFC::CSipUserAgentMFC() : m_hWnd(0), m_pclsCallBack(NULL)
{
}

CSipUserAgentMFC::~CSipUserAgentMFC()
{
}

void CSipUserAgentMFC::SetWindowHandle( HWND hWnd )
{
	m_hWnd = hWnd;
}

void CSipUserAgentMFC::SetCallBack( ISipUserAgentCallBack * pclsCallBack )
{
	m_pclsCallBack = pclsCallBack;
}

LRESULT CSipUserAgentMFC::OnSipMessage( WPARAM wParam, LPARAM lParam )
{
	if( m_pclsCallBack )
	{
		
	}

	return 0;
}

void CSipUserAgentMFC::EventRegister( CSipServerInfo * pclsInfo, int iStatus )
{
}

bool CSipUserAgentMFC::EventIncomingRequestAuth( CSipMessage * pclsMessage )
{
	return true;
}

void CSipUserAgentMFC::EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
{
}

void CSipUserAgentMFC::EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{
}

void CSipUserAgentMFC::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{
}

void CSipUserAgentMFC::EventCallEnd( const char * pszCallId, int iSipStatus )
{
}

void CSipUserAgentMFC::EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp )
{
}

bool CSipUserAgentMFC::EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer )
{
	return false;
}

bool CSipUserAgentMFC::EventBlindTransfer( const char * pszCallId, const char * pszReferToId )
{
	return false;
}

bool CSipUserAgentMFC::EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage )
{
	return false;
}

void CSipUserAgentMFC::EventCallBackThreadEnd( int iThreadId )
{
}

