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
	CSipCallRtp clsRtp;
	char	szTag[SIP_TAG_MAX_SIZE];
	CSipMessage * pclsResponse = NULL;
	SIP_DIALOG_MAP::iterator itMap;

	pclsMessage->GetCallId( strCallId );

	if( GetSipCallRtp( pclsMessage, clsRtp ) == false )
	{
		pclsResponse = pclsMessage->CreateResponse( SIP_NOT_ACCEPTABLE_HERE );
		m_clsSipStack.SendSipMessage( pclsResponse );
		return true;
	}

	// ReINVITE 인지 검사한다.
	m_clsMutex.acquire();
	itMap = m_clsMap.find( strCallId );
	if( itMap != m_clsMap.end() )
	{
		bReINVITE = true;
		
		itMap->second.SetRemoteRtp( &clsRtp );

		pclsResponse = pclsMessage->CreateResponse( SIP_OK );
		itMap->second.AddSdp( pclsResponse );
	}
	m_clsMutex.release();

	if( bReINVITE )
	{
		if( pclsResponse ) m_clsSipStack.SendSipMessage( pclsResponse );
		m_pclsCallBack->EventReInvite( strCallId.c_str(), &clsRtp );
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

	pclsResponse = pclsMessage->CreateResponse( SIP_RINGING, szTag );
	if( pclsResponse == NULL ) return false;
	m_clsSipStack.SendSipMessage( pclsResponse );

	CSipDialog	clsDialog( &m_clsSipStack );

	clsDialog.m_strFromId = pclsMessage->m_clsTo.m_clsUri.m_strUser;
	clsDialog.m_strFromTag = szTag;
	clsDialog.m_eTransport = pclsMessage->m_eTransport;

	clsDialog.m_strToId = pclsMessage->m_clsFrom.m_clsUri.m_strUser;
	pclsMessage->m_clsFrom.SelectParam( "tag", clsDialog.m_strToTag );

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

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strCallId );
	if( itMap == m_clsMap.end() )
	{
		m_clsMap.insert( SIP_DIALOG_MAP::value_type( strCallId, clsDialog ) );
		itMap = m_clsMap.find( strCallId );
		if( itMap != m_clsMap.end() )
		{
			itMap->second.m_pclsInvite = new CSipMessage();
			if( itMap->second.m_pclsInvite )
			{
				*itMap->second.m_pclsInvite = *pclsMessage;
				itMap->second.m_pclsInvite->m_clsTo.InsertParam( "tag", szTag );
			}
		}
	}
	m_clsMutex.release();

	if( m_pclsCallBack )
	{
		m_pclsCallBack->EventIncomingCall( strCallId.c_str(), pclsMessage->m_clsFrom.m_clsUri.m_strUser.c_str()
			, pclsMessage->m_clsTo.m_clsUri.m_strUser.c_str(), &clsRtp );
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
	CSipCallRtp clsRtp;
	bool bRtp = false;

	if( GetSipCallRtp( pclsMessage, clsRtp ) ) bRtp = true;

	if( SetInviteResponse( pclsMessage, bRtp ? &clsRtp : NULL ) )
	{
		std::string	strCallId;

		pclsMessage->GetCallId( strCallId );

		if( pclsMessage->m_iStatusCode == SIP_TRYING || pclsMessage->m_iStatusCode == SIP_UNAUTHORIZED || pclsMessage->m_iStatusCode == SIP_PROXY_AUTHENTICATION_REQUIRED )
		{
			
		}
		else if( pclsMessage->m_iStatusCode > 100 && pclsMessage->m_iStatusCode < 200 )
		{
			if( m_pclsCallBack ) m_pclsCallBack->EventCallRing( strCallId.c_str(), pclsMessage->m_iStatusCode, bRtp ? &clsRtp : NULL );
		}
		else if( pclsMessage->m_iStatusCode >= 200 && pclsMessage->m_iStatusCode < 300 )
		{
			if( m_pclsCallBack ) m_pclsCallBack->EventCallStart( strCallId.c_str(), bRtp ? &clsRtp : NULL );
		}
		else
		{
			if( m_pclsCallBack ) m_pclsCallBack->EventCallEnd( strCallId.c_str(), pclsMessage->m_iStatusCode );

			Delete( strCallId.c_str() );
		}
	}

	return true;
}
