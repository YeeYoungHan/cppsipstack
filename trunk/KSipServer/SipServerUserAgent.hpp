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
 * @ingroup KSipServer
 * @brief SIP REGISTER 응답 메시지 수신 이벤트 핸들러
 * @param pclsInfo	SIP REGISTER 응답 메시지를 전송한 IP-PBX 정보 저장 객체
 * @param iStatus		SIP REGISTER 응답 코드
 */
void CSipServer::EventRegister( CSipServerInfo * pclsInfo, int iStatus )
{

}

/**
 * @ingroup KSipServer
 * @brief SIP 통화 요청 수신에 대한 인증 확인 이벤트 핸들러
 * @param pclsMessage	SIP INVITE 요청 메시지
 * @return 인증에 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServer::EventIncomingRequestAuth( CSipMessage * pclsMessage )
{
	CUserInfo clsUserInfo;

	if( gclsUserMap.Select( pclsMessage->m_clsFrom.m_clsUri.m_strUser.c_str(), clsUserInfo ) == false )
	{
		SIP_CREDENTIAL_LIST::iterator	itCL = pclsMessage->m_clsAuthorizationList.begin();

		if( itCL == pclsMessage->m_clsAuthorizationList.end() )
		{
			return SendUnAuthorizedResponse( pclsMessage );
		}

		ECheckAuthResult eRes = CheckAuthorization( &(*itCL), pclsMessage->m_strSipMethod.c_str() );
		switch( eRes )
		{
		case E_AUTH_NONCE_NOT_FOUND:
			SendUnAuthorizedResponse( pclsMessage );
			return false;
		case E_AUTH_ERROR:
			SendResponse( pclsMessage, SIP_FORBIDDEN );
			return false;
		default:
			break;
		}
	}

	std::string	strIp;
	int		iPort;

	if( pclsMessage->GetTopViaIpPort( strIp, iPort ) == false )
	{
		SendResponse( pclsMessage, SIP_BAD_REQUEST );
		return false;
	}

	if( strcmp( clsUserInfo.m_strIp.c_str(), strIp.c_str() ) || clsUserInfo.m_iPort != iPort )
	{
		SendResponse( pclsMessage, SIP_FORBIDDEN );
		return false;
	}

	return true;
}

/**
 * @ingroup KSipServer
 * @brief SIP 통화 요청 수신 이벤트 핸들러
 * @param	pszCallId	SIP Call-ID
 * @param pszFrom		SIP From 사용자 아이디
 * @param pszTo			SIP To 사용자 아이디
 * @param pclsRtp		RTP 정보 저장 객체
 */
void CSipServer::EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
{
	CXmlUser	clsXmlUser;

	if( SelectUser( pszTo, clsXmlUser ) == false )
	{
		gclsUserAgent.StopCall( pszCallId );
		return;
	}

	if( clsXmlUser.m_bDnd )
	{
		gclsUserAgent.StopCall( pszCallId );
		return;
	}

	if( clsXmlUser.m_strCallForward.empty() == false )
	{
		CSipMessage * pclsInvite = gclsUserAgent.DeleteIncomingCall( pszCallId );
		if( pclsInvite )
		{
			CSipMessage * pclsResponse = pclsInvite->CreateResponseWithToTag( SIP_MOVED_TEMPORARILY );
			if( pclsResponse )
			{
				CSipFrom clsContact;

				clsContact.m_clsUri.m_strProtocol = "sip";
				clsContact.m_clsUri.m_strUser = clsXmlUser.m_strCallForward;
				clsContact.m_clsUri.m_strHost = gclsSetup.m_strLocalIp;
				clsContact.m_clsUri.m_iPort = gclsSetup.m_iUdpPort;

				pclsResponse->m_clsContactList.push_back( clsContact );

				gclsSipStack.SendSipMessage( pclsResponse );
				return;
			}
		}

		gclsUserAgent.StopCall( pszCallId );
		return;
	}

	CUserInfo	clsUserInfo;

	if( gclsUserMap.Select( pszTo, clsUserInfo ) == false )
	{
		gclsUserAgent.StopCall( pszCallId );
		return;
	}

	std::string	strCallId;
	CSipCallRoute	clsRoute;

	clsRoute.m_strDestIp = clsUserInfo.m_strIp;
	clsRoute.m_iDestPort = clsUserInfo.m_iPort;

	if( gclsUserAgent.StartCall( pszFrom, pszTo, pclsRtp, &clsRoute, strCallId ) == false )
	{
		gclsUserAgent.StopCall( pszCallId );
		return;
	}

	gclsCallMap.Insert( pszCallId, strCallId.c_str() );
}

/**
 * @ingroup KSipServer
 * @brief SIP Ring / Session Progress 수신 이벤트 핸들러
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP 응답 코드
 * @param pclsRtp			RTP 정보 저장 객체
 */
void CSipServer::EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{
	std::string	strCallId;

	if( gclsCallMap.Select( pszCallId, strCallId ) )
	{
		gclsUserAgent.RingCall( strCallId.c_str(), iSipStatus, pclsRtp );
	}
	else if( gclsTransCallMap.Select( pszCallId, strCallId ) )
	{
		gclsUserAgent.SendNotify( strCallId.c_str(), iSipStatus );
	}
}

/**
 * @ingroup KSipServer
 * @brief SIP 통화 연결 이벤트 핸들러
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP 정보 저장 객체
 */
void CSipServer::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	std::string	strCallId;

	if( gclsCallMap.Select( pszCallId, strCallId ) )
	{
		gclsUserAgent.AcceptCall( strCallId.c_str(), pclsRtp );
	}
	else if( gclsTransCallMap.Select( pszCallId, strCallId ) )
	{
		gclsUserAgent.SendNotify( strCallId.c_str(), SIP_OK );

		std::string	strReferCallId;

		if( gclsCallMap.Select( strCallId.c_str(), strReferCallId ) )
		{
			gclsUserAgent.StopCall( strReferCallId.c_str() );
			gclsCallMap.Delete( strCallId.c_str() );
		}

		gclsUserAgent.SendReInvite( strCallId.c_str(), pclsRtp );
		gclsCallMap.Insert( pszCallId, strCallId.c_str() );
		gclsTransCallMap.Delete( pszCallId );
	}
}

/**
 * @ingroup KSipServer
 * @brief SIP 통화 종료 이벤트 핸들러
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP 응답 코드. INVITE 에 대한 오류 응답으로 전화가 종료된 경우, INVITE 의 응답 코드를 저장한다.
 */
void CSipServer::EventCallEnd( const char * pszCallId, int iSipStatus )
{
	std::string	strCallId;

	if( gclsCallMap.Select( pszCallId, strCallId ) )
	{
		gclsUserAgent.StopCall( strCallId.c_str() );
		gclsCallMap.Delete( pszCallId );
	}
	else if( gclsTransCallMap.Select( pszCallId, strCallId ) )
	{
		gclsUserAgent.SendNotify( strCallId.c_str(), iSipStatus );
		gclsTransCallMap.Delete( pszCallId );
	}
}

/**
 * @ingroup KSipServer
 * @brief SIP ReINVITE 수신 이벤트 핸들러
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP 정보 저장 객체
 */
void CSipServer::EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp )
{

}

bool CSipServer::EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer )
{
	std::string	strCallId, strReferToCallId;
	CSipCallRtp clsRtp;

	if( gclsCallMap.Select( pszCallId, strCallId ) == false ) return false;
	if( gclsCallMap.Select( pszReferToCallId, strReferToCallId ) == false ) return false;

	gclsCallMap.Delete( pszCallId );
	gclsCallMap.Delete( pszReferToCallId );

	if( gclsUserAgent.GetRemoteCallRtp( strCallId.c_str(), &clsRtp ) == false ) return false;

	if( bScreenedTransfer )
	{
		CSipCallRtp clsReferToRtp;

		if( gclsUserAgent.GetRemoteCallRtp( strReferToCallId.c_str(), &clsReferToRtp ) == false ) return false;

		gclsCallMap.Insert( strCallId.c_str(), strReferToCallId.c_str() );
		gclsUserAgent.SendReInvite( strCallId.c_str(), &clsReferToRtp );
		gclsUserAgent.SendReInvite( strReferToCallId.c_str(), &clsRtp );
	}

	gclsUserAgent.StopCall( pszCallId );
	gclsUserAgent.StopCall( pszReferToCallId, SIP_REQUEST_TERMINATED );

	if( bScreenedTransfer == false )
	{
		std::string	strNewCallId, strFromId, strToId;
		CUserInfo	clsUserInfo;
		CSipCallRoute	clsRoute;

		gclsUserAgent.GetToId( strCallId.c_str(), strFromId );
		gclsUserAgent.GetToId( strReferToCallId.c_str(), strToId );

		if( gclsUserMap.Select( strToId.c_str(), clsUserInfo ) )
		{
			clsRoute.m_strDestIp = clsUserInfo.m_strIp;
			clsRoute.m_iDestPort = clsUserInfo.m_iPort;

			gclsUserAgent.StopCall( strReferToCallId.c_str() );
			gclsUserAgent.StartCall( strFromId.c_str(), strToId.c_str(), &clsRtp, &clsRoute, strNewCallId );
			gclsCallMap.Insert( strCallId.c_str(), strNewCallId.c_str() );
		}
		else
		{
			gclsUserAgent.StopCall( strCallId.c_str() );
			gclsUserAgent.StopCall( strReferToCallId.c_str() );
		}
	}

	return true;
}

bool CSipServer::EventBlindTransfer( const char * pszCallId, const char * pszReferToId )
{
	std::string	strCallId, strInviteCallId, strToId;
	CSipCallRtp clsRtp;
	CUserInfo		clsUserInfo;
	CSipCallRoute	clsRoute;

	if( gclsCallMap.Select( pszCallId, strCallId ) == false ) return false;
	if( gclsUserAgent.GetRemoteCallRtp( strCallId.c_str(), &clsRtp ) == false ) return false;
	if( gclsUserAgent.GetToId( strCallId.c_str(), strToId ) == false ) return false;
	if( gclsUserMap.Select( pszReferToId, clsUserInfo ) == false ) return false;

	clsRoute.m_strDestIp = clsUserInfo.m_strIp;
	clsRoute.m_iDestPort = clsUserInfo.m_iPort;

	if( gclsUserAgent.StartCall( strToId.c_str(), pszReferToId, &clsRtp, &clsRoute, strInviteCallId ) == false )
	{
		return false;
	}

	gclsTransCallMap.Insert( strCallId.c_str(), strInviteCallId.c_str() );

	return true;
}

/**
 * @ingroup KSipServer
 * @brief 1초 단위로 실행되는 타이머 이벤트 핸들러
 */
void CSipServer::EventTimer( )
{
	
}
