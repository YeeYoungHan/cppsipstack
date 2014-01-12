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
#include "SipReason.h"
#include "StringUtility.h"
#include "MemoryDebug.h"

CSipReason::CSipReason()
{
}

CSipReason::~CSipReason()
{
}

/**
 * @ingroup SipParser
 * @brief SIP Reason ����� �Ľ��Ͽ��� ��� ������ �����Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ�
 * @param iTextLen	pszText ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipReason::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos, iCurPos;
	bool	bParam = false;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == ';' )
		{
			m_strProtocol.append( pszText, iPos );
			bParam = true;
			break;
		}
		else if( pszText[iPos] == ',' )
		{
			m_strProtocol.append( pszText, iPos );
			break;
		}
	}

	iCurPos = iPos;

	if( m_strProtocol.empty() )
	{
		m_strProtocol.append( pszText, iCurPos );
	}

	if( bParam )
	{
		int iRet = HeaderListParamParse( pszText + iCurPos, iTextLen - iCurPos );
		if( iRet == -1 ) return -1;
		iCurPos += iRet;

		SIP_PARAMETER_LIST::iterator	itList;

		for( itList = m_clsParamList.begin(); itList != m_clsParamList.end(); ++itList )
		{
			if( itList->m_strValue.empty() == false && itList->m_strValue.at(0) == '"' )
			{
				ReplaceString( itList->m_strValue, "\"", "" );
			}
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
int CSipReason::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	int iLen, iPos;

	iLen = snprintf( pszText, iTextSize, "%s", m_strProtocol.c_str() );

	iPos = ParamToString( pszText + iLen, iTextSize - iLen );
	if( iPos == -1 ) return -1;
	iLen += iPos;

	return iLen;
}

/**
 * @ingroup SipParser
 * @brief ��� ������ �ʱ�ȭ��Ų��.
 */
void CSipReason::Clear()
{
	m_strProtocol.clear();
	ClearParam();
}
