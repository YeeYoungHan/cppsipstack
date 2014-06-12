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

#include "SipStackDefine.h"
#include "SipDialog.h"
#include "SipUtility.h"
#include "SipServer.h"

/**
 * @ingroup McuControlServer
 * @brief 생성자
 */
CSipDialog::CSipDialog() : m_iSeq(0), m_iContactPort(-1), m_pclsInvite(NULL)
{
}

/**
 * @ingroup McuControlServer
 * @brief 소멸자.
 */
CSipDialog::~CSipDialog()
{
	if( m_pclsInvite ) delete m_pclsInvite;
}

/**
 * @ingroup McuControlServer
 * @brief INVITE 메시지를 생성한다.
 * @returns 성공하면 INVITE 메시지를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CSipMessage * CSipDialog::CreateInvite( )
{
	CSipMessage * pclsMessage = CreateMessage( SIP_METHOD_INVITE );
	if( pclsMessage == NULL ) return NULL;

	char	szBranch[SIP_BRANCH_MAX_SIZE];

	SipMakeBranch( szBranch, sizeof(szBranch) );

	pclsMessage->AddVia( gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.m_iLocalUdpPort, szBranch );
	m_strViaBranch = szBranch;

	AddSdp( pclsMessage );

	return pclsMessage;
}

/**
 * @ingroup McuControlServer
 * @brief ACK 메시지를 생성한다.
 * @returns 성공하면 ACK 메시지를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CSipMessage * CSipDialog::CreateAck( )
{
	CSipMessage * pclsMessage = CreateMessage( SIP_METHOD_ACK );
	if( pclsMessage == NULL ) return NULL;

	pclsMessage->AddVia( gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.m_iLocalUdpPort, m_strViaBranch.c_str() );

	return pclsMessage;
}

/**
 * @ingroup McuControlServer
 * @brief CANCEL 메시지를 생성한다.
 * @returns 성공하면 CANCEL 메시지를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CSipMessage * CSipDialog::CreateCancel( )
{
	CSipMessage * pclsMessage = CreateMessage( "CANCEL" );
	if( pclsMessage == NULL ) return NULL;

	pclsMessage->AddVia( gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.m_iLocalUdpPort, m_strViaBranch.c_str() );

	return pclsMessage;
}

/**
 * @ingroup McuControlServer
 * @brief BYE 메시지를 생성한다.
 * @returns 성공하면 BYE 메시지를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CSipMessage * CSipDialog::CreateBye( )
{
	CSipMessage * pclsMessage = CreateMessage( "BYE" );
	if( pclsMessage == NULL ) return NULL;

	return pclsMessage;
}

/**
 * @ingroup McuControlServer
 * @brief NOTIFY 메시지를 생성한다.
 * @returns 성공하면 BYE 메시지를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CSipMessage * CSipDialog::CreateNotify( )
{
	CSipMessage * pclsMessage = CreateMessage( "NOTIFY" );
	if( pclsMessage == NULL ) return NULL;

	return pclsMessage;
}

/**
 * @ingroup McuControlServer
 * @brief SIP 메시지에 SDP 메시지를 추가한다.
 * @param pclsMessage SDP 메시지를 추가할 SIP 메시지 객체
 * @returns true 를 리턴한다.
 */
bool CSipDialog::AddSdp( CSipMessage * pclsMessage )
{
	pclsMessage->m_strBody = m_strLocalSdp;
	pclsMessage->m_iContentLength = (int)m_strLocalSdp.length();
	pclsMessage->m_clsContentType.Set( "application", "sdp" );
	
	return true;
}

/**
 * @ingroup McuControlServer
 * @brief local SDP 정보를 저장한다.
 * @param pclsRtp SDP 정보를 저장한 객체
 * @returns true 를 리턴한다.
 */
bool CSipDialog::SetLocalSdp( const char * pszSdp )
{
	m_strLocalSdp = pszSdp;

	return true;
}

/**
 * @ingroup McuControlServer
 * @brief remote SDP 정보를 저장한다.
 * @param pszSdp SDP 정보를 저장한 문자열
 * @returns true 를 리턴한다.
 */
bool CSipDialog::SetRemoteSdp( const char * pszSdp )
{
	m_strRemoteSdp = pszSdp;

	return true;
}

/**
 * @ingroup McuControlServer
 * @brief SIP 요청 메시지를 생성한다.
 * @param pszSipMethod SIP 메소드
 * @returns 성공하면 SIP 요청 메시지를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CSipMessage * CSipDialog::CreateMessage( const char * pszSipMethod )
{
	CSipMessage * pclsMessage = new CSipMessage();
	if( pclsMessage == NULL ) return NULL;

	if( pclsMessage->m_clsCallId.Parse( m_strCallId.c_str(), (int)m_strCallId.length() ) == -1 )
	{
		delete pclsMessage;
		return NULL;
	}

	pclsMessage->m_strSipMethod = pszSipMethod;

	if( m_strContactUri.empty() == false )
	{
		pclsMessage->m_clsReqUri.Parse( m_strContactUri.c_str(), (int)m_strContactUri.length() );
	}
	else
	{
		pclsMessage->m_clsReqUri.Set( "sip", m_strToId.c_str(), m_strContactIp.c_str(), m_iContactPort );
	}

	if( strcmp( pszSipMethod, SIP_METHOD_ACK ) && strcmp( pszSipMethod, "CANCEL" ) )
	{
		++m_iSeq;
	}
	pclsMessage->m_clsCSeq.Set( m_iSeq, pszSipMethod );

	pclsMessage->m_clsFrom.m_clsUri.Set( "sip", m_strFromId.c_str(), gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.m_iLocalUdpPort );
	pclsMessage->m_clsFrom.InsertParam( "tag", m_strFromTag.c_str() );

	pclsMessage->m_clsTo.m_clsUri.Set( "sip", m_strToId.c_str(), m_strContactIp.c_str(), m_iContactPort );
	if( m_strToTag.empty() == false )
	{
		pclsMessage->m_clsTo.InsertParam( "tag", m_strToTag.c_str() );
	}

	// Route
	pclsMessage->AddRoute( m_strContactIp.c_str(), m_iContactPort );

	return pclsMessage;
}
