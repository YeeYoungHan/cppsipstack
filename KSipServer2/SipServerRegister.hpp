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
 * @brief SIP 응답 메시지에 WWW-Authenticate 헤더를 추가한다.
 * @param psttResponse SIP 응답 메시지
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool AddChallenge( CSipMessage * psttResponse )
{
	CSipChallenge * pclsChallenge = new CSipChallenge();
	if( pclsChallenge == NULL ) return false;

	char	szNonce[33];

	if( gclsNonceMap.GetNewValue( szNonce, sizeof(szNonce) ) == false )
	{
		CLog::Print( LOG_ERROR, "gclsNonce.GetNewValue() error" );
		return false;
	}

	pclsChallenge->m_strType = "Digest";
	pclsChallenge->m_strNonce = szNonce;
	pclsChallenge->m_strRealm = gclsSetup.m_strRealm;
	
	psttResponse->m_clsWwwAuthenticateList.push_back( pclsChallenge );

  return true;
}

/**
 * @ingroup KSipServer
 * @brief SIP 401 응답 메시지를 전송한다.
 * @param pclsMessage SIP 요청 메시지
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool SendUnAuthorizedResponse( CSipMessage * pclsMessage )
{
	CSipMessage * pclsResponse = pclsMessage->CreateResponseWithToTag( SIP_UNAUTHORIZED );
	if( pclsResponse == NULL ) return false;

	AddChallenge( pclsResponse );

	gclsUserAgent.m_clsSipStack.SendSipMessage( pclsResponse );

	return true;
}

/** 
 * @ingroup KSipServer
 * @brief	response 가 유효한 값인지 검사한다. 
 * @param	pszUserName	사용자 아이디
 * @param	pszRealm		realm
 * @param	pszNonce		nonce
 * @param	pszUri			uri
 * @param	pszResponse	response
 * @param	pszPassWord	비밀번호
 * @param	pszMethod		SIP 메소드
 * @return	response 문자열이 유효하면 true 를 리턴한다. 그렇지 않으면 false 를 리턴한다.
 */
bool CheckAuthorizationResponse( const char * pszUserName
		, const char * pszRealm
		, const char * pszNonce
		, const char * pszUri
		, const char * pszResponse
		, const char * pszPassWord
		, const char * pszMethod )
{
	char	szA1[301], szA2[201], szMd5[33], szResponse[201];
	
	snprintf( szA1, sizeof(szA1), "%s:%s:%s", pszUserName, pszRealm, pszPassWord );
	SipMd5String( szA1, szMd5 );
	snprintf( szA1, sizeof(szA1), "%s", szMd5 );
	
	snprintf( szA2, sizeof(szA2), "%s:%s", pszMethod, pszUri );
	SipMd5String( szA2, szMd5 );
	snprintf( szA2, sizeof(szA2), "%s", szMd5 );
	
	snprintf( szResponse, sizeof(szResponse), "%s:%s:%s", szA1, pszNonce, szA2 );
	SipMd5String( szResponse, szMd5 );
	snprintf( szResponse, sizeof(szResponse), "%s", szMd5 );
	
	if( strcmp( szResponse, pszResponse ) )
	{
		CLog::Print( LOG_ERROR, "response[%s] is not correct. correct response is [%s]", pszResponse, szResponse );
		return false;
	}
	
	return true;
}

/**
 * @ingroup KSipServer
 * @brief SIP 인증 결과
 */
enum ECheckAuthResult
{
	E_AUTH_OK = 0,
	E_AUTH_NONCE_NOT_FOUND,
	E_AUTH_ERROR
};

/**
 * @ingroup KSipServer
 * @brief 
 * @param pclsCredential	SIP 인증 정보 저장 객체
 * @param pszMethod				SIP 메소드
 * @param clsXmlUser			사용자 정보 저장 객체
 * @returns 인증에 성공하면 E_AUTH_OK 를 리턴한다.
 *					존재하지 않는 nonce 인 경우 E_AUTH_NONCE_NOT_FOUND 를 리턴한다.
 *					그 외의 오류는 E_AUTH_ERROR 를 리턴한다.
 */
ECheckAuthResult CheckAuthorization( CSipCredential * pclsCredential, const char * pszMethod, CXmlUser & clsXmlUser )
{
	if( pclsCredential->m_strUserName.empty() ) return E_AUTH_ERROR;
	if( gclsNonceMap.Select( pclsCredential->m_strNonce.c_str() ) == false ) return E_AUTH_NONCE_NOT_FOUND;
	if( SelectUser( pclsCredential->m_strUserName.c_str(), clsXmlUser ) == false ) return E_AUTH_ERROR;
	if( CheckAuthorizationResponse( pclsCredential->m_strUserName.c_str(), pclsCredential->m_strRealm.c_str(), pclsCredential->m_strNonce.c_str(), pclsCredential->m_strUri.c_str()
				, pclsCredential->m_strResponse.c_str(), clsXmlUser.m_strPassWord.c_str(), pszMethod ) == false ) return E_AUTH_ERROR;

	return E_AUTH_OK;
}

/**
 * @ingroup KSipServer
 * @brief SIP REGISTER 요청 메시지 수신 이벤트 핸들러
 * @param iThreadId		SIP UDP 쓰레드 아이디
 * @param pclsMessage SIP 요청 메시지
 * @returns SIP 요청 메시지를 처리하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServer::RecvRequestRegister( int iThreadId, CSipMessage * pclsMessage )
{
	if( pclsMessage->m_iExpires != 0 && gclsSetup.m_iMinRegisterTimeout != 0 )
	{
		if( pclsMessage->m_iExpires < gclsSetup.m_iMinRegisterTimeout )
		{
			CSipMessage * pclsResponse = pclsMessage->CreateResponseWithToTag( SIP_INTERVAL_TOO_BRIEF );
			if( pclsResponse == NULL ) return false;

			pclsResponse->AddHeader( "Min-Expires", gclsSetup.m_iMinRegisterTimeout );
			gclsUserAgent.m_clsSipStack.SendSipMessage( pclsResponse );
			return true;
		}
	}

	if( pclsMessage->m_iExpires > 0 && gclsSetup.IsAllowClientIp( pclsMessage->m_strClientIp.c_str() ) == false )
	{
		SendResponse( pclsMessage, SIP_FORBIDDEN );
		return true;
	}

	SIP_CREDENTIAL_LIST::iterator	itCL = pclsMessage->m_clsAuthorizationList.begin();

	if( itCL == pclsMessage->m_clsAuthorizationList.end() )
	{
		return SendUnAuthorizedResponse( pclsMessage );
	}

	CXmlUser	clsXmlUser;
	bool bRes = true;

	ECheckAuthResult eRes = CheckAuthorization( *itCL, pclsMessage->m_strSipMethod.c_str(), clsXmlUser );
	switch( eRes )
	{
	case E_AUTH_NONCE_NOT_FOUND:
		SendUnAuthorizedResponse( pclsMessage );
		return true;
	case E_AUTH_ERROR:
		SendResponse( pclsMessage, SIP_FORBIDDEN );
		return true;
	default:
		break;
	}

	if( pclsMessage->GetExpires() == 0 )
	{
		gclsUserMap.Delete( pclsMessage->m_clsFrom.m_clsUri.m_strUser.c_str() );

		SendResponse( pclsMessage, SIP_OK );
	}
	else
	{
		CSipFrom * pclsContact = new CSipFrom();
		if( pclsContact )
		{
			if( gclsUserMap.Insert( pclsMessage, pclsContact, &clsXmlUser ) )
			{
				CSipMessage * pclsResponse = pclsMessage->CreateResponseWithToTag( SIP_OK );
				if( pclsResponse )
				{
					pclsResponse->m_clsContactList.push_back( pclsContact );
					pclsContact = NULL;
					gclsUserAgent.m_clsSipStack.SendSipMessage( pclsResponse );
				}
				else
				{
					bRes = false;
				}
			}
			else
			{
				SendResponse( pclsMessage, SIP_BAD_REQUEST );
			}

			if( pclsContact )
			{
				delete pclsContact;
			}
		}
	}

	return bRes;
}
