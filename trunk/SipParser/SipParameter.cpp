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
#include "SipParameter.h"
#include "MemoryDebug.h"

CSipParameter::CSipParameter()
{
}

CSipParameter::~CSipParameter()
{
}

/**
 * @ingroup SipParser
 * @brief Parameter ���ڿ��� �Ľ��Ͽ� CSipParameter Ŭ������ ��� ������ �����Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ�
 * @param iTextLen	pszText ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipParameter::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos, iValuePos = -1;
	bool	bStartQuote = false;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == '"' )
		{
			bStartQuote = bStartQuote ? false : true;
		}
		else if( pszText[iPos] == '=' )
		{
			if( bStartQuote ) continue;
			m_strName.append( pszText, iPos );
			iValuePos = iPos + 1;
		}
		else if( pszText[iPos] == ',' || pszText[iPos] == ';' || pszText[iPos] == '&' || pszText[iPos] == '?' )
		{
			if( bStartQuote ) continue;
			break;
		}
		else if( pszText[iPos] == '\r' )
		{
			break;
		}
	}

	if( iPos > 0 )
	{
		if( iValuePos != -1 )
		{
			m_strValue.append( pszText + iValuePos, iPos - iValuePos );
		}
		else
		{
			m_strName.append( pszText, iPos );
		}
		return iPos;
	}

	return -1;
}

/**
 * @ingroup SipParser
 * @brief SIP �޽����� ���Ե� ���ڿ��� �ۼ��Ѵ�.
 * @param pszText		SIP ����� ���� ������ ���ڿ� ����
 * @param iTextSize	pszText ������ ũ��
 * @returns �����ϸ� �ۼ��� ���ڿ� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSipParameter::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;

	if( m_strName.empty() ) return 0;

	if( m_strValue.empty() )
	{
		return snprintf( pszText, iTextSize, "%s", m_strName.c_str() );
	}

	return snprintf( pszText, iTextSize, "%s=%s", m_strName.c_str(), m_strValue.c_str() );
}

/**
 * @ingroup SipParser
 * @brief ��� ������ �ʱ�ȭ��Ų��.
 */
void CSipParameter::Clear()
{
	m_strName.clear();
	m_strValue.clear();
}

/**
 * @ingroup SipParser
 * @brief parameter ����Ʈ ���ڿ��� �Ľ��Ͽ��� parameter ����Ʈ ��ü�� �����Ѵ�.
 * @param clsList		parameter ����Ʈ ��ü
 * @param pszText		parameter ����Ʈ ���ڿ�
 * @param iTextLen	parameter ����Ʈ ���ڿ��� ����
 * @returns �����ϸ� �Ľ��� ���ڿ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int ParseSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszText, int iTextLen )
{
	CSipParameter clsParam;

	int iPos = clsParam.Parse( pszText, iTextLen );
	if( iPos == -1 ) return -1;

	clsList.push_back( clsParam );

	return iPos;
}

/**
 * @ingroup SipParser
 * @brief parameter ����Ʈ���� parameter �̸��� ���� ���� �˻��Ѵ�.
 * @param clsList		parameter ����Ʈ ��ü
 * @param pszName		parameter �̸�
 * @param strValue	parameter ���� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool SearchSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszName, std::string & strValue )
{
	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		if( !strcasecmp( itList->m_strName.c_str(), pszName ) )
		{
			strValue = itList->m_strValue;
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipParser
 * @brief parameter ����Ʈ���� parameter �̸��� ���� ���� �˻��Ѵ�.
 * @param clsList		parameter ����Ʈ ��ü
 * @param pszName		parameter �̸�
 * @returns �����ϸ� parameter ���� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
const char * SearchSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszName )
{
	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		if( !strcasecmp( itList->m_strName.c_str(), pszName ) )
		{
			return itList->m_strValue.c_str();
		}
	}

	return NULL;
}

/**
 * @ingroup SipParser
 * @brief parameter ����Ʈ�� paramter �� �߰��Ѵ�.
 * @param clsList		parameter ����Ʈ ��ü
 * @param pszName		parameter �̸�
 * @param pszValue	parameter ��
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool InsertSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszName, const char * pszValue )
{
	if( pszName == NULL ) return false;

	CSipParameter clsParam;

	clsParam.m_strName = pszName;
	if( pszValue ) clsParam.m_strValue = pszValue;

	clsList.push_back( clsParam );

	return true;
}

/**
 * @ingroup SipParser
 * @brief parameter ����Ʈ�� paramter �� �����Ѵ�.
 * @param clsList		parameter ����Ʈ ��ü
 * @param pszName		parameter �̸�
 * @param pszValue	parameter ��
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool UpdateSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszName, const char * pszValue )
{
	SIP_PARAMETER_LIST::iterator	itList;

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		if( !strcasecmp( itList->m_strName.c_str(), pszName ) )
		{
			itList->m_strValue = pszValue;

			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipParser
 * @brief parameter ����Ʈ ��ü�� parameter ����Ʈ ���ڿ��� �����Ѵ�.
 * @param clsList		parameter ����Ʈ ��ü
 * @param pszText		parameter ����Ʈ ���ڿ��� ������ ����
 * @param iTextSize parameter ����Ʈ ���ڿ��� ũ��
 * @returns parameter ����Ʈ ���ڿ��� ���̸� �����Ѵ�.
 */
int MakeSipParameterString( SIP_PARAMETER_LIST & clsList, char * pszText, int iTextSize )
{
	SIP_PARAMETER_LIST::iterator	itList;
	int iLen = 0, iPos;

	for( itList = clsList.begin(); itList != clsList.end(); ++itList )
	{
		if( iLen >= iTextSize ) return -1;
		pszText[iLen++] = ';';
		iPos = itList->ToString( pszText + iLen, iTextSize - iLen );
		if( iPos == -1 ) return -1;
		iLen += iPos;
	}

	return iLen;
}
