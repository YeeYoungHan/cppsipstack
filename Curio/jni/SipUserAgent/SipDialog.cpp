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

#ifndef ANDROID
#include "SipStackDefine.h"
#include "SipUtility.h"
#else
#include "../sipstack/SipStackDefine.h"
#include "../sipparser/SipUtility.h"
#endif
#include "SipDialog.h"
#include "SipUserAgent.h"

CSipDialog::CSipDialog() : m_iSeq(0), m_iContactPort(-1), m_iLocalRtpPort(-1), m_iRemoteRtpPort(-1), m_iCodec(-1), m_pclsInvite(NULL)
{
	memset( &m_sttInviteTime, 0, sizeof(m_sttInviteTime) );
	memset( &m_sttCancelTime, 0, sizeof(m_sttCancelTime) );
	memset( &m_sttStartTime, 0, sizeof(m_sttStartTime) );
	memset( &m_sttEndTime, 0, sizeof(m_sttEndTime) );
}

CSipDialog::~CSipDialog()
{
	if( m_pclsInvite ) delete m_pclsInvite;
}

CSipMessage * CSipDialog::CreateInvite( )
{
	CSipMessage * pclsMessage = CreateMessage( "INVITE" );
	if( pclsMessage == NULL ) return NULL;

	char	szBranch[SIP_BRANCH_MAX_SIZE];

	SipMakeBranch( szBranch, sizeof(szBranch) );
	pclsMessage->AddVia( gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.m_iLocalUdpPort, szBranch );
	m_strViaBranch = szBranch;

	AddSdp( pclsMessage );

	return pclsMessage;
}

CSipMessage * CSipDialog::CreateAck( )
{
	CSipMessage * pclsMessage = CreateMessage( "ACK" );
	if( pclsMessage == NULL ) return NULL;

	pclsMessage->AddVia( gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.m_iLocalUdpPort, m_strViaBranch.c_str() );

	return pclsMessage;
}

CSipMessage * CSipDialog::CreateCancel( )
{
	CSipMessage * pclsMessage = CreateMessage( "CANCEL" );
	if( pclsMessage == NULL ) return NULL;

	pclsMessage->AddVia( gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.m_iLocalUdpPort, m_strViaBranch.c_str() );

	return pclsMessage;
}

CSipMessage * CSipDialog::CreateBye( )
{
	CSipMessage * pclsMessage = CreateMessage( "BYE" );
	if( pclsMessage == NULL ) return NULL;

	return pclsMessage;
}

bool CSipDialog::AddSdp( CSipMessage * pclsMessage )
{
	char	szSdp[1024];
	int		iLen = 0;

	iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "v=0\r\n"
					"o=CSS 4 2 IN IP4 %s\r\n"
					"s=CSS\r\n", m_strLocalRtpIp.c_str() );

	iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "c=IN IP4 %s\r\n", m_strLocalRtpIp.c_str() );
	iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "t=0 0\r\n" );

	switch( m_iCodec )
	{
	case 0:
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "m=audio %d RTP/AVP 0 101\r\n", m_iLocalRtpPort );
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:0 PCMU/8000\r\n" );
		break;
	case 8:
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "m=audio %d RTP/AVP 8 101\r\n", m_iLocalRtpPort );
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:8 PCMA/8000\r\n" );
		break;
	case 3:
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "m=audio %d RTP/AVP 3 101\r\n", m_iLocalRtpPort );
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:3 GSM/8000\r\n" );
		break;
	case 18:
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "m=audio %d RTP/AVP 18 101\r\n", m_iLocalRtpPort );
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:18 G729/8000\r\n" );
		break;
	}

	iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:101 telephone-event/8000\r\n"
		"a=fmtp:101 0-15\r\n"
		"a=sendrecv\r\n" );

	pclsMessage->m_strBody = szSdp;
	pclsMessage->m_iContentLength = iLen;
	pclsMessage->m_clsContentType.Set( "application", "sdp" );
	
	return true;
}

CSipMessage * CSipDialog::CreateMessage( const char * pszSipMethod )
{
	CSipMessage * pclsMessage = new CSipMessage();
	if( pclsMessage == NULL ) return NULL;

	if( pclsMessage->m_clsCallId.Parse( m_strCallId.c_str(), m_strCallId.length() ) == -1 )
	{
		delete pclsMessage;
		return NULL;
	}

	pclsMessage->m_strSipMethod = pszSipMethod;

	if( m_strContactUri.empty() == false )
	{
		pclsMessage->m_clsReqUri.Parse( m_strContactUri.c_str(), m_strContactUri.length() );
	}
	else
	{
		pclsMessage->m_clsReqUri.Set( "sip", m_strToId.c_str(), m_strContactIp.c_str(), m_iContactPort );
	}

	if( strcmp( pszSipMethod, "ACK" ) && strcmp( pszSipMethod, "CANCEL" ) )
	{
		++m_iSeq;
	}
	pclsMessage->m_clsCSeq.Set( m_iSeq, pszSipMethod );

	pclsMessage->m_clsFrom.m_clsUri.Set( "sip", m_strFromId.c_str(), gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.m_iLocalUdpPort );
	pclsMessage->m_clsFrom.AddParam( "tag", m_strFromTag.c_str() );

	pclsMessage->m_clsTo.m_clsUri.Set( "sip", m_strToId.c_str(), m_strContactIp.c_str(), m_iContactPort );
	if( m_strToTag.empty() == false )
	{
		pclsMessage->m_clsTo.AddParam( "tag", m_strToTag.c_str() );
	}

	// Route
	pclsMessage->AddRoute( m_strContactIp.c_str(), m_iContactPort );

	return pclsMessage;
}
