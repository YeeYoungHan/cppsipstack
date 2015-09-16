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

#include "SipUserAgent.h"
#include "SipServerInfo.h"
#include "SipUtility.h"
#include "SipMd5.h"
#include "MemoryDebug.h"

/**
 * @ingroup SipUserAgent
 * @brief ������
 */
CSipServerInfo::CSipServerInfo() : m_iPort(5060), m_iLoginTimeout(3600)
	, m_eTransport(E_SIP_UDP), m_iNatTimeout(0)
	, m_iNextSendTime(0), m_iSeqNo(0), m_bAuth(false), m_bDelete(false)
{
	ClearLogin();
}

/**
 * @ingroup SipUserAgent
 * @brief �Ҹ���
 */
CSipServerInfo::~CSipServerInfo()
{
}

/**
 * @ingroup SipUserAgent
 * @brief ������ IP-PBX �������� �˻��Ѵ�.
 * @param clsInfo IP-PBX ����
 * @returns ������ IP-PBX �����̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipServerInfo::Equal( CSipServerInfo & clsInfo )
{
	if( !strcmp( clsInfo.m_strIp.c_str(), m_strIp.c_str() ) &&
			!strcmp( clsInfo.m_strUserId.c_str(), m_strUserId.c_str() ) &&
			clsInfo.m_iPort == m_iPort &&
			clsInfo.m_eTransport == m_eTransport )
	{
		return true;
	}

	return false;
}

bool CSipServerInfo::Equal( const char * pszIp, int iPort, ESipTransport eTransport )
{
	if( !strcmp( pszIp, m_strIp.c_str() ) &&
			iPort == m_iPort &&
			eTransport == m_eTransport )
	{
		return true;
	}

	return false;
}

/**
 * @ingroup SipUserAgent
 * @brief IP-PBX ������ �����Ѵ�.
 * @param clsInfo IP-PBX ����
 */
void CSipServerInfo::Update( CSipServerInfo & clsInfo )
{
	m_strDomain = clsInfo.m_strDomain;
	m_strPassWord = clsInfo.m_strPassWord;
	m_iLoginTimeout = clsInfo.m_iLoginTimeout;
}

/**
 * @ingroup SipUserAgent
 * @brief �α��ε� ������ �ʱ�ȭ��Ų��.
 */
void CSipServerInfo::ClearLogin()
{
	m_bLogin = false;
	m_iLoginTime = 0;
	m_iSendTime = 0;
	m_iResponseTime = 0;
	m_clsCallId.Clear();
}

/**
 * @ingroup SipUserAgent
 * @brief SIP REGISTER �޽����� �����Ѵ�.
 * @param pclsSipStack SIP stack
 * @param pclsResponse SIP REGISTER ��û�� ���� ���� �޽���
 * @returns �����ϸ� REGISTER �޽����� �����ϰ� �����ϸ� NULL �� �����Ѵ�.
 */
CSipMessage * CSipServerInfo::CreateRegister( CSipStack * pclsSipStack, CSipMessage * pclsResponse )
{
	CSipMessage * pclsRequest = new CSipMessage();
	if( pclsRequest == NULL ) return NULL;

	// REGISTER sip:127.0.0.1 SIP/2.0
	pclsRequest->m_strSipMethod = SIP_METHOD_REGISTER;
	pclsRequest->m_clsReqUri.Set( SIP_PROTOCOL, NULL, m_strDomain.c_str(), m_iPort );

	// To
	pclsRequest->m_clsTo.m_clsUri.Set( SIP_PROTOCOL, m_strUserId.c_str(), m_strDomain.c_str(), m_iPort );

	// From
	pclsRequest->m_clsFrom = pclsRequest->m_clsTo;
	pclsRequest->m_clsFrom.InsertTag();

	// Expires: 300
	pclsRequest->m_iExpires = m_iLoginTimeout;
	
	// CSeq: 1 REGISTER
	++m_iSeqNo;
	if( m_iSeqNo >= 2000000000 ) m_iSeqNo = 1;
	pclsRequest->m_clsCSeq.m_iDigit = m_iSeqNo;
	pclsRequest->m_clsCSeq.m_strMethod = SIP_METHOD_REGISTER;

	// Route
	pclsRequest->AddRoute( m_strIp.c_str(), m_iPort, m_eTransport );

	// Call-Id
	if( m_clsCallId.Empty() )
	{
		pclsRequest->m_clsCallId.Make( pclsSipStack->m_clsSetup.m_strLocalIp.c_str() );
		m_clsCallId = pclsRequest->m_clsCallId;
	}
	else
	{
		pclsRequest->m_clsCallId = m_clsCallId;
	}

	m_bAuth = false;

	if( pclsResponse )
	{
		m_bAuth = AddAuth( pclsRequest, pclsResponse );

		/* �Ʒ��� �ҽ� �ڵ� ������ FreeSwitch �� REGISTER ���� �ʾƼ� �ּ� ó����
		std::string	strToTag;

		if( pclsResponse->m_clsTo.SelectParam( SIP_TAG, strToTag ) )
		{
			pclsRequest->m_clsTo.InsertParam( SIP_TAG, strToTag.c_str() );
		}
		*/
	}

	pclsRequest->m_eTransport = m_eTransport;

	return pclsRequest;
}

/**
 * @ingroup SipUserAgent
 * @brief SIP ��û �޽����� ���� ������ �߰��Ѵ�.
 * @param pclsRequest		SIP ��û �޽���
 * @param pclsResponse	SIP ���� �޽���
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipServerInfo::AddAuth( CSipMessage * pclsRequest, CSipMessage * pclsResponse )
{
	SIP_CHALLENGE_LIST::const_iterator itAT;
	CSipCredential clsCredential;

	if( pclsResponse->m_iStatusCode == SIP_PROXY_AUTHENTICATION_REQUIRED )
	{
		if( pclsResponse->m_clsProxyAuthenticateList.size() == 0 ) return false;
		itAT = pclsResponse->m_clsProxyAuthenticateList.begin();
	}
	else
	{
		if( pclsResponse->m_clsWwwAuthenticateList.size() == 0 ) return false;
		itAT = pclsResponse->m_clsWwwAuthenticateList.begin();
	}

	clsCredential.m_strType = itAT->m_strType;
	clsCredential.m_strUserName = m_strUserId;
	clsCredential.m_strRealm = itAT->m_strRealm;
	clsCredential.m_strNonce = itAT->m_strNonce;
	clsCredential.m_strAlgorithm = itAT->m_strAlgorithm;

	clsCredential.m_strUri = "sip:";
	clsCredential.m_strUri.append( m_strDomain );

	char	szA1[1024], szA2[1024], szMd5[33], szResponse[1024];

	if( itAT->m_strQop.empty() == false && ( !strcmp( itAT->m_strQop.c_str(), "auth" ) || !strcmp( itAT->m_strQop.c_str(), "auth-int" ) ) )
	{
		clsCredential.m_strQop = itAT->m_strQop;
		clsCredential.m_strNonceCount = "00000001";
		clsCredential.m_strCnonce = "1";

		snprintf( szA1, sizeof(szA1), "%s:%s:%s", clsCredential.m_strUserName.c_str(), clsCredential.m_strRealm.c_str(), m_strPassWord.c_str() );
		SipMd5String( szA1, szMd5 );
		snprintf( szA1, sizeof(szA1), "%s", szMd5 );
		
		snprintf( szA2, sizeof(szA2), "%s:%s", pclsRequest->m_strSipMethod.c_str(), clsCredential.m_strUri.c_str() );
		SipMd5String( szA2, szMd5 );
		snprintf( szA2, sizeof(szA2), "%s", szMd5 );
		
		snprintf( szResponse, sizeof(szResponse), "%s:%s:%s:%s:%s:%s", szA1, clsCredential.m_strNonce.c_str(), clsCredential.m_strNonceCount.c_str()
			, clsCredential.m_strCnonce.c_str(), clsCredential.m_strQop.c_str(), szA2 );
		SipMd5String( szResponse, szMd5 );
		snprintf( szResponse, sizeof(szResponse), "%s", szMd5 );

		clsCredential.m_strResponse = szMd5;
	}
	else
	{
		snprintf( szA1, sizeof(szA1), "%s:%s:%s", clsCredential.m_strUserName.c_str(), clsCredential.m_strRealm.c_str(), m_strPassWord.c_str() );
		SipMd5String( szA1, szMd5 );
		snprintf( szA1, sizeof(szA1), "%s", szMd5 );
		
		snprintf( szA2, sizeof(szA2), "%s:%s", pclsRequest->m_strSipMethod.c_str(), clsCredential.m_strUri.c_str() );
		SipMd5String( szA2, szMd5 );
		snprintf( szA2, sizeof(szA2), "%s", szMd5 );
		
		snprintf( szResponse, sizeof(szResponse), "%s:%s:%s", szA1, clsCredential.m_strNonce.c_str(), szA2 );
		SipMd5String( szResponse, szMd5 );

		clsCredential.m_strResponse = szMd5;
	}

	if( pclsResponse->m_iStatusCode == SIP_PROXY_AUTHENTICATION_REQUIRED )
	{
		pclsRequest->m_clsProxyAuthorizationList.push_front( clsCredential );
	}
	else
	{
		pclsRequest->m_clsAuthorizationList.push_front( clsCredential );
	}

	return true;
}
