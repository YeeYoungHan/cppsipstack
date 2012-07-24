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

#include "SipServerInfo.h"
#include "SipUtility.h"
#include "SipUserAgent.h"

CSipServerInfo::CSipServerInfo(void) : m_iPort(5060), m_iLoginTimeout(3600), m_iSeqNo(0)
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

	// REGISTER sip:127.0.0.1 SIP/2.0
	pclsRequest->m_clsReqUri.m_strProtocol = "sip";
	pclsRequest->m_clsReqUri.m_strHost = m_strDomain;

	pclsRequest->m_strSipMethod = "REGISTER";
	pclsRequest->m_strSipVersion = "SIP/2.0";

	// To
	pclsRequest->m_clsTo.m_clsUri.m_strProtocol = "sip";
	pclsRequest->m_clsTo.m_clsUri.m_strUser = m_strUserId;
	pclsRequest->m_clsTo.m_clsUri.m_strHost = m_strDomain;

	// From
	pclsRequest->m_clsFrom = pclsRequest->m_clsTo;
	pclsRequest->m_clsFrom.AddTag();

	// Expires: 300
	pclsRequest->m_iExpires = m_iLoginTimeout;
	
	// CSeq: 1 REGISTER
	++m_iSeqNo;
	pclsRequest->m_clsCSeq.m_iDigit = m_iSeqNo;
	pclsRequest->m_clsCSeq.m_strMethod = "REGISTER";

	// Route
	pclsRequest->AddRoute( m_strIp.c_str(), m_iPort );

	// Call-Id
	pclsRequest->m_clsCallId.Make( gclsSipStack.m_clsSetup.m_strLocalIp.c_str() );

	return pclsRequest;
}
