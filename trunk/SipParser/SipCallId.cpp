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
#include "SipCallId.h"
#include "SipUtility.h"
#include "MemoryDebug.h"

CSipCallId::CSipCallId()
{
}

CSipCallId::~CSipCallId()
{
}

/**
 * @ingroup SipParser
 * @brief SIP ��� ���ڿ��� �Ľ��Ͽ� CSipCallId Ŭ������ ��� ������ �����Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ�
 * @param iTextLen	pszText ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipCallId::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int iPos;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == '@' )
		{
			m_strName.append( pszText, iPos );
			m_strHost.append( pszText + iPos + 1 );
			break;
		}
	}

	if( m_strName.empty() )
	{
		m_strName.append( pszText );
	}

	return iTextLen;
}

/**
 * @ingroup SipParser
 * @brief Call-ID ���ڿ��� �ۼ��Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ� ����
 * @param iTextSize	pszText ������ ũ��
 * @returns �����ϸ� �ۼ��� ���ڿ� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipCallId::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	if( m_strHost.empty() )
	{
		return snprintf( pszText, iTextSize, "%s", m_strName.c_str() );
	}
	else
	{
		return snprintf( pszText, iTextSize, "%s@%s", m_strName.c_str(), m_strHost.c_str() );
	}
}

/**
 * @ingroup SipParser
 * @brief Call-ID ���ڿ��� �ۼ��Ѵ�.
 * @param strText		SIP ����� ���� ������ ���ڿ� ����
 */
void CSipCallId::ToString( std::string & strText )
{
	strText = m_strName;

	if( m_strHost.empty() == false )
	{
		strText.append( "@" );
		strText.append( m_strHost );
	}
}

/**
 * @ingroup SipParser
 * @brief ��� ������ �ʱ�ȭ��Ų��.
 */
void CSipCallId::Clear()
{
	m_strName.clear();
	m_strHost.clear();
}

/**
 * @ingroup SipParser
 * @brief ��������� ����Ǿ� ���� ������ true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 * @return ��������� ����Ǿ� ���� ������ true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipCallId::Empty()
{
	return m_strName.empty();
}

/**
 * @ingroup SipParser
 * @brief Call-ID �� �������� �˻��Ѵ�.
 * @param pclsCallId ���� Call-ID
 * @returns Call-ID �� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipCallId::IsEqual( CSipCallId * pclsCallId )
{
	if( pclsCallId == NULL ) return false;

	if( !strcmp( m_strName.c_str(), pclsCallId->m_strName.c_str() ) && !strcmp( m_strHost.c_str(), pclsCallId->m_strHost.c_str() ) ) return true;

	return false;
}

/**
 * @ingroup SipParser
 * @brief ���ο� Call-ID �� �����Ѵ�.
 * @param pszHost ȣ��Ʈ �̸� �Ǵ� IP �ּ�
 */
void CSipCallId::Make( const char * pszHost )
{
	char	szName[SIP_CALL_ID_NAME_MAX_SIZE];

	SipMakeCallIdName( szName, sizeof(szName) );

	m_strName = szName;

	if( pszHost ) m_strHost = pszHost;
}
