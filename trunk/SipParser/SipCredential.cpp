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
#include "SipCredential.h"
#include "SipChallenge.h"

CSipCredential::CSipCredential()
{
}

CSipCredential::~CSipCredential()
{
}

/**
 * @ingroup SipParser
 * @brief SIP ��� ���ڿ��� �Ľ��Ͽ� CSipCredential Ŭ������ ��� ������ �����Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ�
 * @param iTextLen	pszText ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipCredential::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int iPos, iCurPos;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == ' ' || pszText[iPos] == '\t' )
		{
			m_strType.append( pszText, iPos );
			++iPos;
			break;
		}
	}

	if( m_strType.empty() ) return -1;
	iCurPos = iPos;

	SIP_PARAMETER_LIST	clsList;
	SIP_PARAMETER_LIST::iterator	itList;
	
	while( iCurPos < iTextLen )
	{
		if( pszText[iCurPos] == ' ' || pszText[iCurPos] == '\t' || pszText[iCurPos] == ',' )
		{
			++iCurPos;
			continue;
		}

		iPos = ParseSipParameter( clsList, pszText + iCurPos, iTextLen - iCurPos );
		if( iPos == -1 ) return -1;
		iCurPos += iPos;
	}

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		const char * pszName = itList->m_strName.c_str();

		if( !strcmp( pszName, "username" ) )
		{
			CSipChallenge::DeQuoteString( itList->m_strValue, m_strUserName );
		}
		else if( !strcmp( pszName, "realm" ) )
		{
			CSipChallenge::DeQuoteString( itList->m_strValue, m_strRealm );
		}
		else if( !strcmp( pszName, "nonce" ) )
		{
			CSipChallenge::DeQuoteString( itList->m_strValue, m_strNonce );
		}
		else if( !strcmp( pszName, "uri" ) )
		{
			CSipChallenge::DeQuoteString( itList->m_strValue, m_strUri );
		}
		else if( !strcmp( pszName, "response" ) )
		{
			CSipChallenge::DeQuoteString( itList->m_strValue, m_strResponse );
		}
		else if( !strcmp( pszName, "algorithm" ) )
		{
			m_strAlgorithm = itList->m_strValue;
		}
		else if( !strcmp( pszName, "cnonce" ) )
		{
			CSipChallenge::DeQuoteString( itList->m_strValue, m_strCnonce );
		}
		else if( !strcmp( pszName, "opaque" ) )
		{
			CSipChallenge::DeQuoteString( itList->m_strValue, m_strOpaque );
		}
		else if( !strcmp( pszName, "qop" ) )
		{
			m_strQop = itList->m_strValue;
		}
		else if( !strcmp( pszName, "nc" ) )
		{
			m_strNonceCount = itList->m_strValue;
		}
		else
		{
			m_clsParamList.push_back( *itList );
		}
	}

	return iCurPos;
}

/**
 * @ingroup SipParser
 * @brief SIP �޽����� ���Ե� ���ڿ��� �ۼ��Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ� ����
 * @param iTextSize	pszText ������ ũ��
 * @returns �����ϸ� �ۼ��� ���ڿ� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipCredential::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	int iLen = 0;

	iLen = snprintf( pszText, iTextSize, "%s ", m_strType.c_str() );
	char * pszData = pszText + iLen;
	int	iPos = 0;
	iTextSize -= iLen;

	if( CSipChallenge::SetQuoteString( pszData, iPos, iTextSize, "username", m_strUserName ) == false ) return -1;
	if( CSipChallenge::SetQuoteString( pszData, iPos, iTextSize, "realm", m_strRealm ) == false ) return -1;
	if( CSipChallenge::SetQuoteString( pszData, iPos, iTextSize, "nonce", m_strNonce ) == false ) return -1;
	if( CSipChallenge::SetQuoteString( pszData, iPos, iTextSize, "uri", m_strUri ) == false ) return -1;
	if( CSipChallenge::SetQuoteString( pszData, iPos, iTextSize, "response", m_strResponse ) == false ) return -1;
	if( CSipChallenge::SetString( pszData, iPos, iTextSize, "algorithm", m_strAlgorithm ) == false ) return -1;
	if( CSipChallenge::SetQuoteString( pszData, iPos, iTextSize, "cnonce", m_strCnonce ) == false ) return -1;
	if( CSipChallenge::SetQuoteString( pszData, iPos, iTextSize, "opaque", m_strOpaque ) == false ) return -1;
	if( CSipChallenge::SetString( pszData, iPos, iTextSize, "qop", m_strQop ) == false ) return -1;
	if( CSipChallenge::SetString( pszData, iPos, iTextSize, "nc", m_strNonceCount ) == false ) return -1;

	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
	{
		if( CSipChallenge::SetString( pszData, iPos, iTextSize, itList->m_strName.c_str(), itList->m_strValue ) == false ) return -1;
	}

	return iLen + iPos;
}

/**
 * @ingroup SipParser
 * @brief ��� ������ �ʱ�ȭ��Ų��.
 */
void CSipCredential::Clear()
{
	m_strType.clear();
	m_strUserName.clear();
	m_strRealm.clear();
	m_strNonce.clear();
	m_strUri.clear();
	m_strResponse.clear();
	m_strAlgorithm.clear();
	m_strCnonce.clear();
	m_strOpaque.clear();
	m_strQop.clear();
	m_strNonceCount.clear();
}

/**
 * @brief SIP credential ���ڿ��� �Ľ��Ͽ��� credential ����Ʈ�� �����Ѵ�.
 * @param clsList credential ����Ʈ
 * @param pszText credential ���ڿ�
 * @param iTextLen credential ���ڿ� ���� 
 * @returns �����ϸ� �Ľ��� ���ڿ� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int ParseSipCredential( SIP_CREDENTIAL_LIST & clsList, const char * pszText, int iTextLen )
{
	int iPos;
	CSipCredential	clsCredential;

	iPos = clsCredential.Parse( pszText, iTextLen );
	if( iPos == -1 ) return -1;

	clsList.push_back( clsCredential );
	return iPos;
}
