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
 * @brief ������
 */
CSipDialog::CSipDialog() : m_iSeq(0), m_iContactPort(-1), m_pclsInvite(NULL)
{
}

/**
 * @ingroup McuControlServer
 * @brief �Ҹ���.
 */
CSipDialog::~CSipDialog()
{
	if( m_pclsInvite ) delete m_pclsInvite;
}

/**
 * @ingroup McuControlServer
 * @brief INVITE �޽����� �����Ѵ�.
 * @returns �����ϸ� INVITE �޽����� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
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
 * @brief ACK �޽����� �����Ѵ�.
 * @returns �����ϸ� ACK �޽����� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
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
 * @brief CANCEL �޽����� �����Ѵ�.
 * @returns �����ϸ� CANCEL �޽����� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
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
 * @brief BYE �޽����� �����Ѵ�.
 * @returns �����ϸ� BYE �޽����� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CSipMessage * CSipDialog::CreateBye( )
{
	CSipMessage * pclsMessage = CreateMessage( "BYE" );
	if( pclsMessage == NULL ) return NULL;

	return pclsMessage;
}

/**
 * @ingroup McuControlServer
 * @brief NOTIFY �޽����� �����Ѵ�.
 * @returns �����ϸ� BYE �޽����� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CSipMessage * CSipDialog::CreateNotify( )
{
	CSipMessage * pclsMessage = CreateMessage( "NOTIFY" );
	if( pclsMessage == NULL ) return NULL;

	return pclsMessage;
}

/**
 * @ingroup McuControlServer
 * @brief SIP �޽����� SDP �޽����� �߰��Ѵ�.
 * @param pclsMessage SDP �޽����� �߰��� SIP �޽��� ��ü
 * @returns true �� �����Ѵ�.
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
 * @brief local SDP ������ �����Ѵ�.
 * @param pclsRtp SDP ������ ������ ��ü
 * @returns true �� �����Ѵ�.
 */
bool CSipDialog::SetLocalSdp( const char * pszSdp )
{
	m_strLocalSdp = pszSdp;

	return true;
}

/**
 * @ingroup McuControlServer
 * @brief remote SDP ������ �����Ѵ�.
 * @param pszSdp SDP ������ ������ ���ڿ�
 * @returns true �� �����Ѵ�.
 */
bool CSipDialog::SetRemoteSdp( const char * pszSdp )
{
	m_strRemoteSdp = pszSdp;

	return true;
}

/**
 * @ingroup McuControlServer
 * @brief SIP ��û �޽����� �����Ѵ�.
 * @param pszSipMethod SIP �޼ҵ�
 * @returns �����ϸ� SIP ��û �޽����� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
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
