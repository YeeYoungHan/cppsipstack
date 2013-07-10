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
#include "SipUserAgent.h"
#include "SipUtility.h"

/**
 * @ingroup SipUserAgent
 * @brief 생성자
 */
CSipDialog::CSipDialog() : m_iSeq(0), m_iContactPort(-1), m_eTransport(E_SIP_UDP)
	, m_iLocalRtpPort(-1), m_iRemoteRtpPort(-1), m_iCodec(-1), m_pclsInvite(NULL)
{
	memset( &m_sttInviteTime, 0, sizeof(m_sttInviteTime) );
	memset( &m_sttCancelTime, 0, sizeof(m_sttCancelTime) );
	memset( &m_sttStartTime, 0, sizeof(m_sttStartTime) );
	memset( &m_sttEndTime, 0, sizeof(m_sttEndTime) );
}

/**
 * @ingroup SipUserAgent
 * @brief 소멸자.
 */
CSipDialog::~CSipDialog()
{
	if( m_pclsInvite ) delete m_pclsInvite;
}

/**
 * @ingroup SipUserAgent
 * @brief INVITE 메시지를 생성한다.
 * @returns 성공하면 INVITE 메시지를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CSipMessage * CSipDialog::CreateInvite( )
{
	CSipMessage * pclsMessage = CreateMessage( "INVITE" );
	if( pclsMessage == NULL ) return NULL;

	char	szBranch[SIP_BRANCH_MAX_SIZE];

	SipMakeBranch( szBranch, sizeof(szBranch) );

	pclsMessage->AddVia( gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.GetLocalPort( m_eTransport ), szBranch, m_eTransport );
	m_strViaBranch = szBranch;

	AddSdp( pclsMessage );

	return pclsMessage;
}

/**
 * @ingroup SipUserAgent
 * @brief ACK 메시지를 생성한다.
 * @returns 성공하면 ACK 메시지를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CSipMessage * CSipDialog::CreateAck( )
{
	CSipMessage * pclsMessage = CreateMessage( "ACK" );
	if( pclsMessage == NULL ) return NULL;

	pclsMessage->AddVia( gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.GetLocalPort( m_eTransport ), m_strViaBranch.c_str(), m_eTransport );

	return pclsMessage;
}

/**
 * @ingroup SipUserAgent
 * @brief CANCEL 메시지를 생성한다.
 * @returns 성공하면 CANCEL 메시지를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CSipMessage * CSipDialog::CreateCancel( )
{
	CSipMessage * pclsMessage = CreateMessage( "CANCEL" );
	if( pclsMessage == NULL ) return NULL;

	pclsMessage->AddVia( gclsSipStack.m_clsSetup.m_strLocalIp.c_str(), gclsSipStack.m_clsSetup.GetLocalPort( m_eTransport ), m_strViaBranch.c_str(), m_eTransport );

	return pclsMessage;
}

/**
 * @ingroup SipUserAgent
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
 * @ingroup SipUserAgent
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
 * @ingroup SipUserAgent
 * @brief SIP 메시지에 SDP 메시지를 추가한다.
 * @param pclsMessage SDP 메시지를 추가할 SIP 메시지 객체
 * @returns true 를 리턴한다.
 */
bool CSipDialog::AddSdp( CSipMessage * pclsMessage )
{
	char	szSdp[1024];
	int		iLen = 0;

	iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "v=0\r\n"
					"o=CSS 4 2 IN IP4 %s\r\n"
					"s=CSS\r\n", m_strLocalRtpIp.c_str() );

	iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "c=IN IP4 %s\r\n", m_strLocalRtpIp.c_str() );
	iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "t=0 0\r\n" );

	if( pclsMessage->IsRequest() && m_clsCodecList.size() > 0 )
	{
		CODEC_LIST::iterator	itList;
		bool bFound;

		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "m=audio %d RTP/AVP", m_iLocalRtpPort );

		for( itList = m_clsCodecList.begin(); itList != m_clsCodecList.end(); ++itList )
		{
			bFound = false;

			switch( *itList )
			{
			case 0:
			case 3:
			case 4:
			case 8:
			case 18:
				bFound = true;
				break;
			}

			if( bFound )
			{
				iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, " %d", *itList );
			}
		}

		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, " 101\r\n" );

		for( itList = m_clsCodecList.begin(); itList != m_clsCodecList.end(); ++itList )
		{
			switch( *itList )
			{
			case 0:
				iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:0 PCMU/8000\r\n" );
				break;
			case 3:
				iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:3 GSM/8000\r\n" );
				break;
			case 4:
				iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:4 G723/8000\r\n" );
				break;
			case 8:
				iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:8 PCMA/8000\r\n" );
				break;
			case 18:
				iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:18 G729/8000\r\n" );
				break;
			}
		}
	}
	else
	{
		switch( m_iCodec )
		{
		case 0:
			iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "m=audio %d RTP/AVP 0 101\r\n", m_iLocalRtpPort );
			iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:0 PCMU/8000\r\n" );
			break;
		case 3:
			iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "m=audio %d RTP/AVP 3 101\r\n", m_iLocalRtpPort );
			iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:3 GSM/8000\r\n" );
			break;
		case 4:
			iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "m=audio %d RTP/AVP 4 101\r\n", m_iLocalRtpPort );
			iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:4 G723/8000\r\n" );
			break;
		case 8:
			iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "m=audio %d RTP/AVP 8 101\r\n", m_iLocalRtpPort );
			iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:8 PCMA/8000\r\n" );
			break;
		case 18:
			iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "m=audio %d RTP/AVP 18 101\r\n", m_iLocalRtpPort );
			iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:18 G729/8000\r\n" );
			break;
		}
	}

	iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=rtpmap:101 telephone-event/8000\r\n"
		"a=fmtp:101 0-15\r\n" );

	switch( m_eLocalDirection )
	{
	case E_RTP_SEND_RECV:
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=sendrecv\r\n" );
		break;
	case E_RTP_SEND:
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=sendonly\r\n" );
		break;
	case E_RTP_RECV:
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=recvonly\r\n" );
		break;
	case E_RTP_INACTIVE:
		iLen += snprintf( szSdp + iLen, sizeof(szSdp)-iLen, "a=inactive\r\n" );
		break;
	}

	pclsMessage->m_strBody = szSdp;
	pclsMessage->m_iContentLength = iLen;
	pclsMessage->m_clsContentType.Set( "application", "sdp" );
	
	return true;
}

/**
 * @ingroup SipUserAgent
 * @brief local RTP 정보를 저장한다.
 * @param pclsRtp RTP 정보를 저장한 객체
 * @returns true 를 리턴한다.
 */
bool CSipDialog::SetLocalRtp( CSipCallRtp * pclsRtp )
{
	if( pclsRtp == NULL ) return false;
	
	m_strLocalRtpIp = pclsRtp->m_strIp;
	m_iLocalRtpPort = pclsRtp->m_iPort;
	m_iCodec = pclsRtp->m_iCodec;
	m_clsCodecList = pclsRtp->m_clsCodecList;
	m_eLocalDirection = pclsRtp->m_eDirection;

	switch( m_eLocalDirection )
	{
	case E_RTP_SEND_RECV:
	case E_RTP_INACTIVE:
		m_eRemoteDirection = m_eLocalDirection;
		break;
	case E_RTP_SEND:
		m_eRemoteDirection = E_RTP_RECV;
		break;
	case E_RTP_RECV:
		m_eRemoteDirection = E_RTP_SEND;
		break;
	}

	return true;
}

/**
 * @ingroup SipUserAgent
 * @brief remote RTP 정보를 저장한다.
 * @param pclsRtp RTP 정보를 저장한 객체
 * @returns true 를 리턴한다.
 */
bool CSipDialog::SetRemoteRtp( CSipCallRtp * pclsRtp )
{
	// ReINVITE 에서 hold 인 경우 IP 주소가 0.0.0.0 으로 수신되어서 Transfer 할 때에 정상적으로 SDP IP 주소가 전달되지 않기 위해서 수정함.
	if( strcmp( pclsRtp->m_strIp.c_str(), "0.0.0.0" ) )
	{
		m_strRemoteRtpIp = pclsRtp->m_strIp;
	}

	m_iRemoteRtpPort = pclsRtp->m_iPort;
	m_iCodec = pclsRtp->m_iCodec;
	m_eRemoteDirection = pclsRtp->m_eDirection;

	switch( m_eRemoteDirection )
	{
	case E_RTP_SEND_RECV:
	case E_RTP_INACTIVE:
		m_eLocalDirection = m_eRemoteDirection;
		break;
	case E_RTP_SEND:
		m_eLocalDirection = E_RTP_RECV;
		break;
	case E_RTP_RECV:
		m_eLocalDirection = E_RTP_SEND;
		break;
	}

	return true;
}

/**
 * @ingroup SipUserAgent
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

	pclsMessage->m_eTransport = m_eTransport;
	pclsMessage->m_strSipMethod = pszSipMethod;

	if( m_strContactUri.empty() == false )
	{
		pclsMessage->m_clsReqUri.Parse( m_strContactUri.c_str(), (int)m_strContactUri.length() );
	}
	else
	{
		pclsMessage->m_clsReqUri.Set( "sip", m_strToId.c_str(), m_strContactIp.c_str(), m_iContactPort );
		pclsMessage->m_clsReqUri.InsertTransport( m_eTransport );
	}

	if( strcmp( pszSipMethod, "ACK" ) && strcmp( pszSipMethod, "CANCEL" ) )
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
	pclsMessage->AddRoute( m_strContactIp.c_str(), m_iContactPort, m_eTransport );

	return pclsMessage;
}

/**
 * @ingroup SipUserAgent
 * @brief CDR 정보 객체에 데이터를 저장한다.
 * @param pclsCdr CDR 정보 객체
 */
void CSipDialog::GetCdr( CSipCdr * pclsCdr )
{
	// CSipDialog 의 From, To 는 SipUserAgent 가 SIP 요청 메시지를 전송하는 입장에서 저장되어 있으므로 CDR 을 위해서는 반대로 저장해 준다.
	pclsCdr->m_strFromId = m_strToId;
	pclsCdr->m_strToId = m_strFromId;
	pclsCdr->m_strCallId = m_strCallId;
	pclsCdr->m_sttInviteTime = m_sttInviteTime;
	pclsCdr->m_sttStartTime = m_sttStartTime;
	pclsCdr->m_sttEndTime = m_sttEndTime;
}
