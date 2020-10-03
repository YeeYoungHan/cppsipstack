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

/**
 * @ingroup SipUserAgent
 * @brief SIP INVITE 요청 메시지 수신 이벤트 핸들러
 * @param iThreadId		SIP stack 의 UDP 쓰레드 아이디
 * @param pclsMessage 수신된 SIP 요청 메시지
 * @returns 정상적으로 처리하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::RecvInviteRequest( int iThreadId, CSipMessage * pclsMessage )
{
	std::string	strCallId;
	bool	bReINVITE = false;
	CSipCallRtp clsRtp, clsLocalRtp;
	char	szTag[SIP_TAG_MAX_SIZE];
	CSipMessage * pclsResponse = NULL;
	SIP_DIALOG_MAP::iterator itMap;

	if( pclsMessage->GetCallId( strCallId ) == false )
	{
		m_clsSipStack.SendSipMessage( pclsMessage->CreateResponse( SIP_BAD_REQUEST ) );
		return true;
	}

	if( GetSipCallRtp( pclsMessage, clsRtp ) == false )
	{
		pclsResponse = pclsMessage->CreateResponse( SIP_NOT_ACCEPTABLE_HERE );
		m_clsSipStack.SendSipMessage( pclsResponse );
		return true;
	}

	// ReINVITE 인지 검사한다.
	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( strCallId );
	if( itMap != m_clsDialogMap.end() )
	{
		bReINVITE = true;
		itMap->second.SetRemoteRtp( &clsRtp );
		itMap->second.SelectLocalRtp( &clsLocalRtp );
	}
	m_clsDialogMutex.release();

	if( bReINVITE )
	{
		if( m_pclsCallBack ) m_pclsCallBack->EventReInvite( strCallId.c_str(), &clsRtp, &clsLocalRtp );

		m_clsDialogMutex.acquire();
		itMap = m_clsDialogMap.find( strCallId );
		if( itMap != m_clsDialogMap.end() )
		{
			itMap->second.SetLocalRtp( &clsLocalRtp );
			pclsResponse = pclsMessage->CreateResponse( SIP_OK );
			itMap->second.AddSdp( pclsResponse );
		}
		m_clsDialogMutex.release();

		if( pclsResponse ) m_clsSipStack.SendSipMessage( pclsResponse );

		return true;
	}

	// 새로운 INVITE 인 경우
	SipMakeTag( szTag, sizeof(szTag) );

	if( m_pclsCallBack )
	{
		if( m_pclsCallBack->EventIncomingRequestAuth( pclsMessage ) == false )
		{
			return true;
		}
	}

	// 180 Ring 을 전송한다.
	pclsResponse = pclsMessage->CreateResponse( SIP_RINGING, szTag );
	if( pclsResponse == NULL ) return false;
	m_clsSipStack.SendSipMessage( pclsResponse );

	// Dialog 를 생성한다.
	CSipDialog	clsDialog( &m_clsSipStack );
	bool bError = false;

	clsDialog.m_strFromId = pclsMessage->m_clsTo.m_clsUri.m_strUser;
	clsDialog.m_strFromTag = szTag;
	clsDialog.m_eTransport = pclsMessage->m_eTransport;

	clsDialog.m_strToId = pclsMessage->m_clsFrom.m_clsUri.m_strUser;
	pclsMessage->m_clsFrom.SelectParam( SIP_TAG, clsDialog.m_strToTag );

	clsDialog.m_strCallId = strCallId;
	clsDialog.SetRemoteRtp( &clsRtp );

	pclsMessage->GetTopViaIpPort( clsDialog.m_strContactIp, clsDialog.m_iContactPort );

	SIP_FROM_LIST::iterator	itContact = pclsMessage->m_clsContactList.begin();
	if( itContact != pclsMessage->m_clsContactList.end() )
	{
		char	szUri[255];

		itContact->m_clsUri.ToString( szUri, sizeof(szUri) );
		clsDialog.m_strContactUri = szUri;
	}

	gettimeofday( &clsDialog.m_sttInviteTime, NULL );

	clsDialog.m_pclsInvite = new CSipMessage();
	if( clsDialog.m_pclsInvite )
	{
		*clsDialog.m_pclsInvite = *pclsMessage;
		clsDialog.m_pclsInvite->m_clsTo.InsertParam( SIP_TAG, szTag );

		if( clsDialog.m_pclsInvite->m_clsRecordRouteList.size() > 0 )
		{
			clsDialog.m_clsRouteList = clsDialog.m_pclsInvite->m_clsRecordRouteList;
		}
	}
	clsDialog.m_bSendCall = false;

	// Dialog 를 저장한다.
	m_clsDialogMutex.acquire();
	itMap = m_clsDialogMap.find( strCallId );
	if( itMap == m_clsDialogMap.end() )
	{
		m_clsDialogMap.insert( SIP_DIALOG_MAP::value_type( strCallId, clsDialog ) );
	}
	else
	{
		bError = true;
	}
	m_clsDialogMutex.release();

	if( bError )
	{
		if( clsDialog.m_pclsInvite )
		{
			delete clsDialog.m_pclsInvite;
		}
	}
	else
	{
		if( m_pclsCallBack )
		{
			m_pclsCallBack->EventIncomingCall( strCallId.c_str(), pclsMessage->m_clsFrom.m_clsUri.m_strUser.c_str()
				, pclsMessage->m_clsTo.m_clsUri.m_strUser.c_str(), &clsRtp );
		}
	}

	return true;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP INVITE 응답 메시지 수신 이벤트 핸들러
 * @param iThreadId		SIP stack 의 UDP 쓰레드 아이디
 * @param pclsMessage 수신된 SIP 요청 메시지
 * @returns 정상적으로 처리하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::RecvInviteResponse( int iThreadId, CSipMessage * pclsMessage )
{
	if( pclsMessage->m_iStatusCode == SIP_TRYING ) return true;

	CSipCallRtp clsRtp;
	bool bRtp = false, bReInvite = false;
	std::string	strCallId;

	pclsMessage->GetCallId( strCallId );

	if( GetSipCallRtp( pclsMessage, clsRtp ) ) bRtp = true;

	if( SetInviteResponse( strCallId, pclsMessage, bRtp ? &clsRtp : NULL, bReInvite ) )
	{
		if( bReInvite )
		{
			if( m_pclsCallBack ) m_pclsCallBack->EventReInviteResponse( strCallId.c_str(), pclsMessage->m_iStatusCode, bRtp ? &clsRtp : NULL );
		}
		else
		{
			if( pclsMessage->m_iStatusCode > SIP_TRYING && pclsMessage->m_iStatusCode < SIP_OK )
			{
				if( m_pclsCallBack ) m_pclsCallBack->EventCallRing( strCallId.c_str(), pclsMessage->m_iStatusCode, bRtp ? &clsRtp : NULL );
			}
			else if( pclsMessage->m_iStatusCode >= SIP_OK && pclsMessage->m_iStatusCode < SIP_MULTIPLE_CHOICES )
			{
				if( m_pclsCallBack ) m_pclsCallBack->EventCallStart( strCallId.c_str(), bRtp ? &clsRtp : NULL );
			}
			else
			{
				if( m_pclsCallBack ) m_pclsCallBack->EventCallEnd( strCallId.c_str(), pclsMessage->m_iStatusCode );

				Delete( strCallId.c_str() );
			}
		}
	}

	return true;
}
