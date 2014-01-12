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
#include "SipCSeq.h"
#include <stdlib.h>
#include "MemoryDebug.h"

CSipCSeq::CSipCSeq() : m_iDigit(-1)
{
}

CSipCSeq::~CSipCSeq()
{
}

/**
 * @ingroup SipParser
 * @brief SIP ��� ���ڿ��� �Ľ��Ͽ� CSipCSeq Ŭ������ ��� ������ �����Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ�
 * @param iTextLen	pszText ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipCSeq::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int iPos;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( m_iDigit == -1 )
		{
			if( pszText[iPos] == ' ' )
			{
				std::string	strDigit;

				strDigit.append( pszText, iPos );
				m_iDigit = atoi( strDigit.c_str() );
			}
		}
		else if( pszText[iPos] == ' ' || pszText[iPos] == '\t' )
		{
			continue;
		}
		else
		{
			m_strMethod.append( pszText + iPos );
			break;
		}
	}

	if( m_iDigit == -1 ) return -1;

	return iTextLen;
}

/**
 * @ingroup SipParser
 * @brief SIP �޽����� ���Ե� ���ڿ��� �ۼ��Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ� ����
 * @param iTextSize	pszText ������ ũ��
 * @returns �����ϸ� �ۼ��� ���ڿ� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipCSeq::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;
	if( m_iDigit == -1 || m_strMethod.empty() ) return 0;

	return snprintf( pszText, iTextSize, "%d %s", m_iDigit, m_strMethod.c_str() );
}

/**
 * @ingroup SipParser
 * @brief SIP CSeq ��� ������ �����Ѵ�.
 * @param iDigit		CSeq ����
 * @param pszMethod SIP �޼ҵ�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSipCSeq::Set( int iDigit, const char * pszMethod )
{
	if( iDigit < 0 || pszMethod == NULL ) return false;

	m_iDigit = iDigit;
	m_strMethod = pszMethod;

	return true;
}

/**
 * @ingroup SipParser
 * @brief ��� ������ �ʱ�ȭ��Ų��.
 */
void CSipCSeq::Clear()
{
	m_iDigit = -1;
	m_strMethod.clear();
}

/**
 * @ingroup SipParser
 * @brief ��������� ����Ǿ� ���� ������ true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 * @returns ��������� ����Ǿ� ���� ������ true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipCSeq::Empty()
{
	if( m_iDigit == -1 ) return true;
	return m_strMethod.empty();
}
