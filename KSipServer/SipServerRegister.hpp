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

bool AddChallenge( CSipMessage * psttResponse )
{
	CSipChallenge clsChallenge;

	char	szNonce[33];

	if( gclsNonceMap.GetNewValue( szNonce, sizeof(szNonce) ) == false )
	{
		CLog::Print( LOG_ERROR, "gclsNonce.GetNewValue() error" );
		return false;
	}

	clsChallenge.m_strType = "Digest";
	clsChallenge.m_strNonce = szNonce;
	clsChallenge.m_strRealm = gclsSetup.m_strRealm;
	
	psttResponse->m_clsWwwAuthenticateList.push_back( clsChallenge );

  return true;
}

bool SendUnAuthorizedResponse( CSipMessage * pclsMessage )
{
	CSipMessage * pclsResponse = pclsMessage->CreateResponseWithToTag( SIP_UNAUTHORIZED );
	if( pclsResponse == NULL ) return false;

	AddChallenge( pclsResponse );

	if( gclsSipStack.SendSipMessage( pclsResponse ) == false )
	{
		delete pclsResponse;
		return false;
	}

	return true;
}

/** 
 *	@brief	response 가 유효한 값인지 검사한다. 
 *	@param	pszUserName	사용자 아이디
 *	@param	pszRealm		realm
 *	@param	pszNonce		nonce
 *	@param	pszUri			uri
 *	@param	pszResponse	response
 *  @param	pszPassWord	비밀번호
 *	@param	pszMethod		SIP 메소드
 *	@return	response 문자열이 유효하면 true 를 리턴한다. 그렇지 않으면 false 를 리턴한다.
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
	Md5String( szA1, szMd5 );
	snprintf( szA1, sizeof(szA1), "%s", szMd5 );
	
	snprintf( szA2, sizeof(szA2), "%s:%s", pszMethod, pszUri );
	Md5String( szA2, szMd5 );
	snprintf( szA2, sizeof(szA2), "%s", szMd5 );
	
	snprintf( szResponse, sizeof(szResponse), "%s:%s:%s", szA1, pszNonce, szA2 );
	Md5String( szResponse, szMd5 );
	snprintf( szResponse, sizeof(szResponse), "%s", szMd5 );
	
	if( strcmp( szResponse, pszResponse ) )
	{
		CLog::Print( LOG_ERROR, "response[%s] is not correct. correct response is [%s]", pszResponse, szResponse );
		return false;
	}
	
	return true;
}

enum ECheckAuthResult
{
	E_AUTH_OK = 0,
	E_AUTH_NONCE_NOT_FOUND,
	E_AUTH_ERROR
};

ECheckAuthResult CheckAuthorization( CSipCredential * pclsCredential, const char * pszMethod )
{
	if( pclsCredential->m_strUserName.empty() ) return E_AUTH_ERROR;
	if( gclsNonceMap.Select( pclsCredential->m_strNonce.c_str() ) == false ) return E_AUTH_NONCE_NOT_FOUND;

	CXmlUser	clsXmlUser;

	if( SelectUser( pclsCredential->m_strUserName.c_str(), clsXmlUser ) == false ) return E_AUTH_ERROR;
	if( CheckAuthorizationResponse( pclsCredential->m_strUserName.c_str(), pclsCredential->m_strRealm.c_str(), pclsCredential->m_strNonce.c_str(), pclsCredential->m_strUri.c_str()
				, pclsCredential->m_strResponse.c_str(), clsXmlUser.m_strPassWord.c_str(), pszMethod ) == false ) return E_AUTH_ERROR;

	return E_AUTH_OK;
}

bool CSipServer::RecvRequestRegister( int iThreadId, CSipMessage * pclsMessage )
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
		return true;
	case E_AUTH_ERROR:
		SendResponse( pclsMessage, SIP_FORBIDDEN );
		return true;
	default:
		break;
	}

	if( pclsMessage->m_iExpires == 0 )
	{
		gclsUserMap.Delete( pclsMessage->m_clsFrom.m_clsUri.m_strUser.c_str() );
	}
	else
	{
		gclsUserMap.Insert( pclsMessage );
	}

	SendResponse( pclsMessage, SIP_OK );

	return true;
}
