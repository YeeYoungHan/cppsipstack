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
#include "SipUserAgentMFCDefine.h"
#include "SipUserAgentMFCEventClass.h"

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
		switch( wParam )
		{
		case SMC_REGISER:
			{
				CEventRegister * pclsParam = (CEventRegister *)lParam;
				m_pclsCallBack->EventRegister( &pclsParam->m_clsInfo, pclsParam->m_iStatus );
				delete pclsParam;
			}
			break;
		}
	}

	return WMR_TRUE;
}

void CSipUserAgentMFC::EventRegister( CSipServerInfo * pclsInfo, int iStatus )
{
	CEventRegister * pclsParam = new CEventRegister( pclsInfo, iStatus );
	if( pclsParam == NULL ) return;

	_SendMessage( SMC_REGISER, (LPARAM)pclsParam );
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

LRESULT CSipUserAgentMFC::_SendMessage( WPARAM wParam, LPARAM lParam )
{
	if( m_hWnd == 0 ) return WMR_FALSE;

	if( wParam == SMC_REGISER )
	{
		PostMessage( m_hWnd, SIP_MESSAGE_ID, wParam, lParam );
		return WMR_TRUE;
	}

	return SendMessage( m_hWnd, SIP_MESSAGE_ID, wParam, lParam );
}
