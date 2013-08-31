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

#pragma once

#define SIP_MESSAGE_ID	WM_USER+0x200

#include "SipUserAgentCallBack.h"

class CSipUserAgentMFC : public ISipUserAgentCallBack
{
public:
	CSipUserAgentMFC();
	virtual ~CSipUserAgentMFC();

	void SetWindowHandle( HWND hWnd );
	void SetCallBack( ISipUserAgentCallBack * pclsCallBack );

	LRESULT OnSipMessage( WPARAM wParam, LPARAM lParam );

	virtual void EventRegister( CSipServerInfo * pclsInfo, int iStatus );
	virtual bool EventIncomingRequestAuth( CSipMessage * pclsMessage );
	virtual void EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp );
	virtual void EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp );
	virtual void EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp );
	virtual void EventCallEnd( const char * pszCallId, int iSipStatus );
	virtual void EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp );
	virtual bool EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer );
	virtual bool EventBlindTransfer( const char * pszCallId, const char * pszReferToId );
	virtual bool EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage );
	virtual void EventCallBackThreadEnd( int iThreadId );

private:
	HWND	m_hWnd;
	ISipUserAgentCallBack * m_pclsCallBack;
};
