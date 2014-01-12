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
#include "SipContentType.h"
#include <stdio.h>
#include <string.h>
#include "MemoryDebug.h"

CSipContentType::CSipContentType()
{
}

CSipContentType::~CSipContentType()
{
}

/**
 * @ingroup SipParser
 * @brief SIP ��� ���ڿ��� �Ľ��Ͽ� CSipContentType Ŭ������ ��� ������ �����Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ�
 * @param iTextLen	pszText ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipContentType::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos, iSubTypePos = -1, iParamPos = -1;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == '/' )
		{
			m_strType.append( pszText, iPos );
			iSubTypePos = iPos + 1;
		}
		else if( iSubTypePos != -1 )
		{
			if( pszText[iPos] == ';' || pszText[iPos] == ',' )
			{
				m_strSubType.append( pszText + iSubTypePos, iPos - iSubTypePos );
				iParamPos = iPos + 1;
				break;
			}
		}
		else if( pszText[iPos] == ',' )
		{
			break;
		}
	}

	if( m_strType.empty() )
	{
		m_strType.append( pszText );
	}
	else if( iSubTypePos != -1 && m_strSubType.empty() )
	{
		m_strSubType.append( pszText + iSubTypePos );
	}

	if( iParamPos != -1 )
	{
		int iRet = HeaderListParamParse( pszText + iPos, iTextLen - iPos );
		if( iRet == -1 ) return -1;

		iPos += iRet;
	}

	return iPos;
}

/**
 * @ingroup SipParser
 * @brief SIP �޽����� ���Ե� ���ڿ��� �ۼ��Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ� ����
 * @param iTextSize	pszText ������ ũ��
 * @returns �����ϸ� �ۼ��� ���ڿ� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipContentType::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	int iLen, iPos;

	iLen = snprintf( pszText, iTextSize, "%s/%s", m_strType.c_str(), m_strSubType.c_str() );

	iPos = ParamToString( pszText + iLen, iTextSize - iLen );
	if( iPos == -1 ) return -1;
	iLen += iPos;

	return iLen;
}

/**
 * @ingroup SipParser
 * @brief ��� ������ �ʱ�ȭ��Ų��.
 */
void CSipContentType::Clear()
{
	m_strType.clear();
	m_strSubType.clear();
	ClearParam();
}

/**
 * @ingroup SipParser
 * @brief ��������� ����Ǿ� ���� ������ true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 * @returns ��������� ����Ǿ� ���� ������ true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipContentType::Empty()
{
	if( m_strType.empty() || m_strSubType.empty() ) return true;

	return false;
}

/**
 * @ingroup SipParser
 * @brief Content-Type �� �����Ѵ�.
 * @param pszType			type
 * @param pszSubType	subtype
 */
void CSipContentType::Set( const char * pszType, const char * pszSubType )
{
	if( pszType == NULL || pszSubType == NULL ) return;

	m_strType = pszType;
	m_strSubType = pszSubType;
}

/**
 * @ingroup SipParser
 * @brief Content-Type �� �������� �˻��Ѵ�. 
 * @param pszType			type
 * @param pszSubType	subtype
 * @returns Content-Type �� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipContentType::IsEqual( const char * pszType, const char * pszSubType )
{
	if( pszType == NULL || pszSubType == NULL ) return false;

	if( !strcmp( m_strType.c_str(), pszType ) && !strcmp( m_strSubType.c_str(), pszSubType ) ) return true;

	return false;
}

/**
 * @brief SIP Content-Type ����� �Ľ��Ѵ�.
 * @param clsList		Content-Type ����Ʈ
 * @param pszText		SIP ����� ���� ������ ���ڿ�
 * @param iTextLen	pszText ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int ParseSipContentType( SIP_CONTENT_TYPE_LIST & clsList, const char * pszText, int iTextLen )
{
	int iPos, iCurPos = 0;
	CSipContentType	clsContentType;

	while( iCurPos < iTextLen )
	{
		if( pszText[iCurPos] == ' ' || pszText[iCurPos] == '\t' || pszText[iCurPos] == ',' )
		{
			++iCurPos;
			continue;
		}

		iPos = clsContentType.Parse( pszText + iCurPos, iTextLen - iCurPos );
		if( iPos == -1 ) return -1;
		iCurPos += iPos;

		clsList.push_back( clsContentType );
	}

	return iCurPos;
}
