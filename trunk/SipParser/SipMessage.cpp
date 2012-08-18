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

#include "SipParserDefine.h"
#include "SipMessage.h"
#include "SipStatusCode.h"
#include "SipUtility.h"

CSipMessage::CSipMessage() : m_iStatusCode(-1), m_iContentLength(0), m_iExpires(-1), m_iMaxForwards(-1), m_iUseCount(0)
{
}

CSipMessage::~CSipMessage()
{
}

/**
 * @brief SIP ��� ���ڿ��� �Ľ��Ͽ� CSipMessage Ŭ������ ��� ������ �����Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ�
 * @param iTextLen	pszText ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipMessage::Parse( const char * pszText, int iTextLen )
{
	if( pszText == NULL || iTextLen <= 4 ) return -1;

	int iPos, iCurPos, iValuePos, iValueLen, n;
	const char * pszName, * pszValue;
	CSipHeader	clsHeader;

	if( !strncmp( pszText, "SIP/", 4 ) )
	{
		iCurPos = ParseStatusLine( pszText, iTextLen );
	}
	else
	{
		iCurPos = ParseRequestLine( pszText, iTextLen );
	}

	if( iCurPos == -1 ) return -1;

	while( iCurPos < iTextLen )
	{
		iPos = clsHeader.Parse( pszText + iCurPos, iTextLen - iCurPos );
		if( iPos == -1 ) return -1;
		iCurPos += iPos;

		if( clsHeader.m_strName.length() == 0 ) break;

		pszName = clsHeader.m_strName.c_str();
		pszValue = clsHeader.m_strValue.c_str();
		iValuePos = 0;
		iValueLen = clsHeader.m_strValue.length();

		if( !strcasecmp( pszName, "Via" ) || !strcasecmp( pszName, "v" ) )
		{
			while( iValuePos < iValueLen )
			{
				if( pszValue[iValuePos] == ' ' || pszValue[iValuePos] == '\t' || pszValue[iValuePos] == ',' )
				{
					++iValuePos;
					continue;
				}

				CSipVia	clsVia;

				n = clsVia.Parse( pszValue + iValuePos, iValueLen - iValuePos );
				if( n == -1 ) return -1;
				iValuePos += n;

				m_clsViaList.push_back( clsVia );
			}
		}
		else if( !strcasecmp( pszName, "Max-Forwards" ) )
		{
			m_iMaxForwards = atoi( pszValue );
		}
		else if( !strcasecmp( pszName, "From" ) || !strcasecmp( pszName, "f" ) )
		{
			if( m_clsFrom.Parse( pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "To" ) || !strcasecmp( pszName, "t" ) )
		{
			if( m_clsTo.Parse( pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "CSeq" ) )
		{
			if( m_clsCSeq.Parse( pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Call-ID" ) || !strcasecmp( pszName, "i" ) )
		{
			if( m_clsCallId.Parse( pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Contact" ) || !strcasecmp( pszName, "m" ) )
		{
			if( ParseSipFrom( m_clsContactList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Record-Route" ) )
		{
			if( ParseSipFrom( m_clsRecordRouteList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Route" ) )
		{
			if( ParseSipFrom( m_clsRouteList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Authorization" ) )
		{
			if( ParseSipCredential( m_clsAuthorizationList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Proxy-Authorization" ) )
		{
			if( ParseSipCredential( m_clsProxyAuthorizationList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "WWW-Authenticate" ) )
		{
			if( ParseSipChallenge( m_clsWwwAuthenticateList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Proxy-Authenticate" ) )
		{
			if( ParseSipChallenge( m_clsProxyAuthenticateList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Content-Type" ) || !strcasecmp( pszName, "c" ) )
		{
			if( m_clsContentType.Parse( pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Content-Length" ) || !strcasecmp( pszName, "l" ) )
		{
			m_iContentLength = atoi( pszValue );
		}
		else if( !strcasecmp( pszName, "Accept-Encoding" ) )
		{
			if( ParseSipAcceptData( m_clsAcceptEncodingList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Accept-Language" ) )
		{
			if( ParseSipAcceptData( m_clsAcceptLanguageList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Accept" ) )
		{
			if( ParseSipContentType( m_clsAcceptList, pszValue, iValueLen ) == -1 ) return -1;
		}
		else if( !strcasecmp( pszName, "Expires" ) )
		{
			m_iExpires = atoi( pszValue );
		}
		else if( !strcasecmp( pszName, "User-Agent" ) )
		{
			m_strUserAgent = clsHeader.m_strValue;
		}
		else
		{
			m_clsHeaderList.push_back( clsHeader );
		}
	}

	if( m_iContentLength > 0 )
	{
		if( m_iContentLength > ( iTextLen - iCurPos ) ) return -1;

		m_strBody.append( pszText + iCurPos );
		return iTextLen;
	}

	return iCurPos;
}

/**
 * @brief SIP �޽��� ���ڿ��� �ۼ��Ѵ�.
 * @param pszText		SIP �޽��� ���ڿ��� ������ ���ڿ� ����
 * @param iTextSize	pszText ������ ũ��
 * @returns �����ϸ� �ۼ��� ���ڿ� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipMessage::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	int iLen, n;

	if( m_strSipVersion.empty() ) m_strSipVersion = "SIP/2.0";

	if( m_iStatusCode > 0 )
	{
		if( m_strReasonPhrase.empty() )
		{
			m_strReasonPhrase = GetReasonPhrase( m_iStatusCode );
		}

		iLen = snprintf( pszText, iTextSize, "%s %d %s\r\n", m_strSipVersion.c_str(), m_iStatusCode, m_strReasonPhrase.c_str() );
	}
	else
	{
		if( m_strSipMethod.empty() || m_clsReqUri.Empty() || m_strSipVersion.empty() ) return -1;
		iLen = snprintf( pszText, iTextSize, "%s ", m_strSipMethod.c_str() );
		n = m_clsReqUri.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, " %s\r\n", m_strSipVersion.c_str() );
	}

	for( SIP_VIA_LIST::iterator itList = m_clsViaList.begin(); itList != m_clsViaList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Via: " );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	for( SIP_FROM_LIST::iterator itList = m_clsRecordRouteList.begin(); itList != m_clsRecordRouteList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Record-Route: " );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	for( SIP_FROM_LIST::iterator itList = m_clsRouteList.begin(); itList != m_clsRouteList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Route: " );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_iMaxForwards >= 0 )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Max-Forwards: %d\r\n", m_iMaxForwards );
	}

	if( m_clsFrom.Empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "From: " );
		n = m_clsFrom.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_clsTo.Empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "To: " );
		n = m_clsTo.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_clsCallId.Empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Call-ID: " );
		n = m_clsCallId.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_clsCSeq.Empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "CSeq: " );
		n = m_clsCSeq.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	for( SIP_FROM_LIST::iterator itList = m_clsContactList.begin(); itList != m_clsContactList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Contact: " );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	for( SIP_CREDENTIAL_LIST::iterator itList = m_clsAuthorizationList.begin(); itList != m_clsAuthorizationList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Authorization: " );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	for( SIP_CREDENTIAL_LIST::iterator itList = m_clsProxyAuthorizationList.begin(); itList != m_clsProxyAuthorizationList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Proxy-Authorization: " );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	for( SIP_CHALLENGE_LIST::iterator itList = m_clsWwwAuthenticateList.begin(); itList != m_clsWwwAuthenticateList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "WWW-Authenticate: " );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	for( SIP_CHALLENGE_LIST::iterator itList = m_clsProxyAuthenticateList.begin(); itList != m_clsProxyAuthenticateList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Proxy-Authenticate: " );
		n = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_clsContentType.Empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Content-Type: " );
		n = m_clsContentType.ToString( pszText + iLen, iTextSize - iLen );
		if( n == -1 ) return -1;
		iLen += n;
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	iLen += snprintf( pszText + iLen, iTextSize - iLen, "Content-Length: %d\r\n", m_iContentLength );

	if( m_clsAcceptList.size() > 0 )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Accept: " );
		for( SIP_CONTENT_TYPE_LIST::iterator itList = m_clsAcceptList.begin(); itList != m_clsAcceptList.end(); ++itList )
		{
			if( itList != m_clsAcceptList.begin() )
			{
				iLen += snprintf( pszText + iLen, iTextSize - iLen, ", " );
			}
			n = itList->ToString( pszText + iLen, iTextSize - iLen );
			if( n == -1 ) return -1;
			iLen += n;
		}
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_clsAcceptEncodingList.size() > 0 )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Accept-Encoding: " );
		for( SIP_ACCEPT_DATA_LIST::iterator itList = m_clsAcceptEncodingList.begin(); itList != m_clsAcceptEncodingList.end(); ++itList )
		{
			if( itList != m_clsAcceptEncodingList.begin() )
			{
				iLen += snprintf( pszText + iLen, iTextSize - iLen, ", " );
			}
			n = itList->ToString( pszText + iLen, iTextSize - iLen );
			if( n == -1 ) return -1;
			iLen += n;
		}
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_clsAcceptLanguageList.size() > 0 )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Accept-Language: " );
		for( SIP_ACCEPT_DATA_LIST::iterator itList = m_clsAcceptLanguageList.begin(); itList != m_clsAcceptLanguageList.end(); ++itList )
		{
			if( itList != m_clsAcceptLanguageList.begin() )
			{
				iLen += snprintf( pszText + iLen, iTextSize - iLen, ", " );
			}
			n = itList->ToString( pszText + iLen, iTextSize - iLen );
			if( n == -1 ) return -1;
			iLen += n;
		}
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );
	}

	if( m_iExpires >= 0 )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "Expires: %d\r\n", m_iExpires );
	}

	if( m_strUserAgent.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "User-Agent: %s\r\n", m_strUserAgent.c_str() );
	}

	for( SIP_HEADER_LIST::iterator itList = m_clsHeaderList.begin(); itList != m_clsHeaderList.end(); ++itList )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "%s: %s\r\n", itList->m_strName.c_str(), itList->m_strValue.c_str() );
	}

	iLen += snprintf( pszText + iLen, iTextSize - iLen, "\r\n" );

	if( m_iContentLength > 0 )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "%s", m_strBody.c_str() );
	}

	return iLen;
}

/**
 * @ingroup SipParser
 * @brief ��Ʈ��ũ�� ������ �� �ִ� SIP �޽��� ���ڿ��� �ۼ��Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipMessage::MakePacket()
{
	char	szPacket[SIP_MESSAGE_MAX_LEN];

	if( ToString( szPacket, sizeof(szPacket) ) == -1 ) return false;

	m_strPacket = szPacket;

	return true;
}

/**
 * @ingroup SipParser
 * @brief ��� ������ �ʱ�ȭ��Ų��.
 */
void CSipMessage::Clear()
{
	m_strSipMethod.clear();
	m_clsReqUri.Clear();
	m_strSipVersion.clear();

	m_iStatusCode = -1;
	m_strReasonPhrase.clear();

	m_clsFrom.Clear();
	m_clsTo.Clear();

	m_clsViaList.clear();        
	m_clsContactList.clear();    
	m_clsRecordRouteList.clear();
	m_clsRouteList.clear();      

	m_clsAcceptList.clear();            
                            
	m_clsAcceptEncodingList.clear();    
	m_clsAcceptLanguageList.clear();    
	                            
	m_clsAuthorizationList.clear();     
	m_clsProxyAuthorizationList.clear();
	                            
	m_clsWwwAuthenticateList.clear();   
	m_clsProxyAuthenticateList.clear(); 
	                            
	m_clsHeaderList.clear();            

	m_clsCSeq.Clear();       
	m_clsCallId.Clear();     
	m_clsContentType.Clear();
	m_iContentLength = 0;
	                 
	m_iExpires = -1;      
	m_iMaxForwards = -1;  
	                 
	m_strUserAgent.clear();  
	m_strBody.clear();
}

/**
 * @ingroup SipParser
 * @brief SIP ��û �޽������� �˻��Ѵ�.
 * @returns SIP ��û �޽����̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipMessage::IsRequest()
{
	if( m_strSipMethod.empty() ) return false;

	return true;
}

/**
 * @ingroup SipParser
 * @brief SIP �޼ҵ尡 �Է��� �޼ҵ�� ������ �˻��Ѵ�.
 * @param pszMethod SIP �޼ҵ�
 * @returns SIP �޼ҵ尡 �Է��� �޼ҵ�� ������ true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipMessage::IsMethod( const char * pszMethod )
{
	if( pszMethod == NULL ) return false;

	if( m_strSipMethod.empty() == false )
	{
		if( !strcmp( m_strSipMethod.c_str(), pszMethod ) ) return true;
	}
	else
	{
		if( !strcmp( m_clsCSeq.m_strMethod.c_str(), pszMethod ) ) return true;
	}

	return false;
}

/**
 * @ingroup SipParser
 * @brief Call-ID �� �������� �˻��Ѵ�.
 * @param pclsMessage SIP �޽��� ���� ��ü
 * @returns Call-ID �� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipMessage::IsEqualCallId( CSipMessage * pclsMessage )
{
	if( pclsMessage == NULL ) return false;

	return m_clsCallId.IsEqual( &pclsMessage->m_clsCallId );
}

/**
 * @ingroup SipParser
 * @brief Call-ID ���ڿ��� �����´�.
 * @param strCallId Call-ID ���ڿ��� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipMessage::GetCallId( std::string & strCallId )
{
	strCallId.clear();

	if( m_clsCallId.Empty() ) return false;
	m_clsCallId.ToString( strCallId );

	return true;
}

/**
 * @ingroup SipParser
 * @brief Top Via ����� �߽� IP �ּҿ� ��Ʈ ��ȣ�� �߰��Ѵ�.
 * @param pszIp SIP �޽��� �߽� IP �ּ�
 * @param iPort SIP �޽��� �߽� ��Ʈ ��ȣ
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipMessage::AddIpPortToTopVia( const char * pszIp, int iPort )
{
	SIP_VIA_LIST::iterator itViaList = m_clsViaList.begin();
	if( itViaList == m_clsViaList.end() ) return false;

	SIP_PARAMETER_LIST::iterator	itList;
	bool bRport = false, bReceived = false;

	char	szNum[11];

	snprintf( szNum, sizeof(szNum), "%d", iPort );

	for( itList = itViaList->m_clsParamList.begin(); itList != itViaList->m_clsParamList.end(); ++itList )
	{
		if( !strcmp( itList->m_strName.c_str(), "rport" ) )
		{
			itList->m_strValue = szNum;
			bRport = true;
		}
		else if( !strcmp( itList->m_strName.c_str(), "received" ) )
		{
			itList->m_strValue = pszIp;
			bReceived = true;
		}

		if( bRport && bReceived ) break;
	}

	if( bRport == false && itViaList->m_iPort != iPort )
	{
		AddSipParameter( itViaList->m_clsParamList, "rport", szNum );
	}

	if( bReceived == false && strcmp( itViaList->m_strHost.c_str(), pszIp ) )
	{
		AddSipParameter( itViaList->m_clsParamList, "received", pszIp );
	}

	return true;
}

/**
 * @ingroup SipParser
 * @brief Via ����� �߰��Ѵ�.
 * @param pszIp			Via ����� ������ IP �ּ�
 * @param iPort			Via ����� ������ ��Ʈ ��ȣ
 * @param pszBranch Via branch tag ���ڿ�
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipMessage::AddVia( const char * pszIp, int iPort, const char * pszBranch )
{
	CSipVia clsVia;

	clsVia.m_strProtocolName = "SIP";
	clsVia.m_strProtocolVersion = "2.0";
	clsVia.m_strTransport = "UDP";
	clsVia.m_strHost = pszIp;
	clsVia.m_iPort = iPort;

	AddSipParameter( clsVia.m_clsParamList, "rport", NULL );

	if( pszBranch == NULL || strlen(pszBranch) == 0 )
	{
		char	szBranch[SIP_BRANCH_MAX_SIZE];

		SipMakeBranch( szBranch, sizeof(szBranch) );
		AddSipParameter( clsVia.m_clsParamList, "branch", szBranch );
	}
	else
	{
		AddSipParameter( clsVia.m_clsParamList, "branch", pszBranch );
	}

	m_clsViaList.push_front( clsVia );

	return true;
}

/**
 * @ingroup SipParser
 * @brief Route ����� �߰��Ѵ�.
 * @param pszIp		Route ����� ������ IP �ּ�
 * @param iPort		Route ����� ������ ��Ʈ ��ȣ
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipMessage::AddRoute( const char * pszIp, int iPort )
{
	CSipFrom clsFrom;

	clsFrom.m_clsUri.m_strProtocol = "sip";
	clsFrom.m_clsUri.m_strHost = pszIp;
	clsFrom.m_clsUri.m_iPort = iPort;

	AddSipParameter( clsFrom.m_clsUri.m_clsUriParamList, "lr", NULL );

	m_clsRouteList.push_front( clsFrom );

	return true;
}

/**
 * @ingroup SipParser
 * @brief SIP ���� �޽��� ��ü�� �����Ѵ�.
 * @param iStatus		SIP ���� �ڵ�
 * @param pszToTag	To ����� tag
 * @returns �����ϸ� SIP ���� �޽��� ��ü�� �����͸� �����ϰ� �����ϸ� NULL �� �����Ѵ�.
 */
CSipMessage * CSipMessage::CreateResponse( int iStatus, const char * pszToTag )
{
	if( IsRequest() == false ) return NULL;

	CSipMessage * pclsResponse = new CSipMessage();
	if( pclsResponse == NULL ) return NULL;

	pclsResponse->m_iStatusCode = iStatus;
	pclsResponse->m_clsTo = m_clsTo;
	pclsResponse->m_clsFrom = m_clsFrom;
	pclsResponse->m_clsViaList = m_clsViaList;
	pclsResponse->m_clsCallId = m_clsCallId;
	pclsResponse->m_clsCSeq = m_clsCSeq;

	if( pszToTag )
	{
		pclsResponse->m_clsTo.InsertParam( "tag", pszToTag );
	}

	return pclsResponse;
}

CSipMessage * CSipMessage::CreateResponseWithToTag( int iStatus )
{
	if( IsRequest() == false ) return NULL;

	CSipMessage * pclsResponse = new CSipMessage();
	if( pclsResponse == NULL ) return NULL;

	pclsResponse->m_iStatusCode = iStatus;
	pclsResponse->m_clsTo = m_clsTo;
	pclsResponse->m_clsFrom = m_clsFrom;
	pclsResponse->m_clsViaList = m_clsViaList;
	pclsResponse->m_clsCallId = m_clsCallId;
	pclsResponse->m_clsCSeq = m_clsCSeq;

	if( pclsResponse->m_clsTo.SelectParam( "tag" ) == false )
	{
		pclsResponse->m_clsTo.InsertTag();
	}

	return pclsResponse;
}

/**
 * @ingroup SipParser
 * @brief Top Via ����� IP �ּҿ� ��Ʈ ��ȣ�� �����´�.
 * @param strIp		IP �ּҸ� ������ ����
 * @param iPort		��Ʈ ��ȣ�� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipMessage::GetTopViaIpPort( std::string & strIp, int & iPort )
{
	strIp.clear();
	iPort = 0;

	SIP_VIA_LIST::iterator itViaList = m_clsViaList.begin();
	if( itViaList == m_clsViaList.end() ) return false;

	std::string strTemp;

	if( SearchSipParameter( itViaList->m_clsParamList, "rport", strTemp ) )
	{
		iPort = atoi( strTemp.c_str() );
	}

	if( SearchSipParameter( itViaList->m_clsParamList, "received", strTemp ) )
	{
		strIp = strTemp;
	}

	if( iPort == 0 ) iPort = itViaList->m_iPort;
	if( strIp.empty() ) strIp = itViaList->m_strHost;

	return true;
}

int CSipMessage::GetExpires()
{
	if( m_iExpires != -1 ) return m_iExpires;

	SIP_FROM_LIST::iterator itContact = m_clsContactList.begin();
	if( itContact == m_clsContactList.end() ) return 0;

	std::string	strExpires;

	if( itContact->SelectParam( "EXPIRES", strExpires ) )
	{
		return atoi( strExpires.c_str() );
	}

	return 0;
}

int CSipMessage::ParseStatusLine( const char * pszText, int iTextLen )
{
	int		iPos, iStartPos = -1;
	char	cType = 0;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( cType != 2 )
		{
			if( pszText[iPos] == ' ' )
			{
				switch( cType )
				{
				case 0:
					m_strSipVersion.append( pszText, iPos );
					break;
				case 1:
					{
						std::string strStatusCode;

						strStatusCode.append( pszText + iStartPos, iPos - iStartPos );
						m_iStatusCode = atoi( strStatusCode.c_str() );
						break;
					}
				}

				iStartPos = iPos + 1;
				++cType;
			}
		}
		else
		{
			if( pszText[iPos] == '\r' )
			{
				if( iPos + 1 >= iTextLen || pszText[iPos+1] != '\n' ) return -1;

				m_strReasonPhrase.append( pszText + iStartPos, iPos - iStartPos );
				return iPos + 2;
			}
		}
	}

	return -1;
}

int CSipMessage::ParseRequestLine( const char * pszText, int iTextLen )
{
	int		iPos, iStartPos = -1;
	char	cType = 0;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( cType != 2 )
		{
			if( pszText[iPos] == ' ' )
			{
				switch( cType )
				{
				case 0:
					m_strSipMethod.append( pszText, iPos );
					break;
				case 1:
					{
						if( m_clsReqUri.Parse( pszText + iStartPos, iPos - iStartPos ) == -1 ) return -1;
						break;
					}
				}

				iStartPos = iPos + 1;
				++cType;
			}
		}
		else
		{
			if( pszText[iPos] == '\r' )
			{
				if( iPos + 1 >= iTextLen || pszText[iPos+1] != '\n' ) return -1;

				m_strSipVersion.append( pszText + iStartPos, iPos - iStartPos );
				return iPos + 2;
			}
		}
	}

	return -1;
}
