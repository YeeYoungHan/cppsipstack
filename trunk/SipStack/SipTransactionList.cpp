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

#include "SipTransactionList.h"
#include "MemoryDebug.h"

int CSipTransactionList::m_arrICTReSendTime[MAX_ICT_RESEND_COUNT] = { 500
	, 1500
	, 3500
	, 7500
	, 11500
	, 15500
	, 19500
	, 23500
	, 27500
	, 31500
	, 32000 };

CSipTransactionList::CSipTransactionList() : m_pclsSipStack(NULL)
{

}

CSipTransactionList::~CSipTransactionList()
{

}

/**
 * @ingroup SipStack
 * @brief Transaction List �� ������ CSipStack ��ü�� �����͸� �����Ѵ�. ������ ����� ���ؼ� CSipStack ��ü�� �ʿ��ϴ�.
 * @param pclsSipStack CSipStack ��ü�� ������
 */
void CSipTransactionList::SetSipStack( CSipStack * pclsSipStack )
{
	m_pclsSipStack = pclsSipStack;
}

/**
 * @ingroup SipStack
 * @brief Transaction List �� ������ Ű�� �����´�.
 * @param pclsMessage SIP �޽��� ���� ����ü
 * @param strKey			Transaction List �� ������ Ű
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipTransactionList::GetKey( CSipMessage * pclsMessage, std::string & strKey )
{
	strKey.clear();

	if( pclsMessage == NULL ) return false;

	SIP_VIA_LIST::iterator itList = pclsMessage->m_clsViaList.begin();
	if( itList == pclsMessage->m_clsViaList.end() ) return false;

	const char * pszBranch = itList->SelectParamValue( SIP_BRANCH );
	if( pszBranch )
	{
		if( !strncmp( pszBranch, VIA_PREFIX, VIA_PREFIX_LEN ) )
		{
			strKey.append( pszBranch + VIA_PREFIX_LEN );
			strKey.append( " " );

			char	szNum[11];
			snprintf( szNum, sizeof(szNum), "%d", pclsMessage->m_clsCSeq.m_iDigit );

			strKey.append( szNum );

			if( !strcmp( pclsMessage->m_clsCSeq.m_strMethod.c_str(), SIP_METHOD_ACK ) )
			{
				strKey.append( SIP_METHOD_INVITE );
			}
			else
			{
				strKey.append( pclsMessage->m_clsCSeq.m_strMethod );
			}
		}
	}

	if( strKey.empty() )
	{
		char szCallId[255];

		if( pclsMessage->m_clsCallId.ToString( szCallId, sizeof(szCallId) ) == -1 ) return false;
		strKey = szCallId;
	}

	return true;
}

/**
 * @ingroup SipStack
 * @brief Transaction List �� ������ Ű�� �����´�.
 * @param pclsMessage SIP �޽��� ���� ����ü
 * @param pszMethod		SIP Via branch �� Ű�� ������ ��� ����� SIP �޼ҵ�
 * @param strKey			Transaction List �� ������ Ű
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipTransactionList::GetKey( CSipMessage * pclsMessage, const char * pszMethod, std::string & strKey )
{
	strKey.clear();

	if( pclsMessage == NULL ) return false;

	SIP_VIA_LIST::iterator itList = pclsMessage->m_clsViaList.begin();
	if( itList == pclsMessage->m_clsViaList.end() ) return false;

	const char * pszBranch = itList->SelectParamValue( SIP_BRANCH );
	if( pszBranch )
	{
		if( !strncmp( pszBranch, VIA_PREFIX, VIA_PREFIX_LEN ) )
		{
			strKey.append( pszBranch + VIA_PREFIX_LEN );
			strKey.append( " " );

			char	szNum[11];
			snprintf( szNum, sizeof(szNum), "%d", pclsMessage->m_clsCSeq.m_iDigit );

			strKey.append( szNum );
			strKey.append( pszMethod );
		}
	}

	if( strKey.empty() )
	{
		char szCallId[255];

		if( pclsMessage->m_clsCallId.ToString( szCallId, sizeof(szCallId) ) == -1 ) return false;
		strKey = szCallId;
	}

	return true;
}
