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

#include "XmlSearch.h"

CXmlSearch::CXmlSearch()
{
}

CXmlSearch::~CXmlSearch()
{
}

/**
 * @ingroup XmlParser
 * @brief XML �� ��� ���� element �߿��� �Էµ� �̸��� ��ġ�ϴ� ���� �˻��Ѵ�.
 * @param pszName element �̸�
 * @param iIndex	����. 0 �� �Է��ϸ� ù��° �˻��� element ���� �˻��Ѵ�. 2 �� �Է��ϸ� ����° �˻��� element ���� �˻��Ѵ�.
 * @returns �˻��Ǹ� �ش� ���� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
const char * CXmlSearch::SelectElementData( const char * pszName, int iIndex )
{
	XML_ELEMENT_LIST::iterator	itEL;
	int iCount = 0;
	const char * pszData = NULL;

	if( iIndex < 0 ) iIndex = 0;

	for( itEL = m_clsElementList.begin(); itEL != m_clsElementList.end(); ++itEL )
	{
		if( !strcmp( pszName, itEL->GetName() ) )
		{
			if( iCount == iIndex )
			{
				return itEL->GetData();
			}

			++iCount;
		}
		else
		{
			pszData = SelectElementData( itEL->GetElementList(), pszName, iIndex, iCount );
			if( pszData )
			{
				return pszData;
			}
		}
	}

	return NULL;
}

/**
 * @ingroup XmlParser
 * @brief XML �� ��� ���� element �߿��� �Էµ� �̸��� ��ġ�ϴ� element �� �˻��Ѵ�.
 * @param pszName �̸�
 * @param iIndex ����. 0 �� �Է��ϸ� ù��° �˻��� element �� �˻��Ѵ�. 2 �� �Է��ϸ� ����° �˻��� element �� �˻��Ѵ�.
 * @returns �˻��Ǹ� �ش� element �� �����͸� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CXmlElement * CXmlSearch::SelectElement( const char * pszName, int iIndex )
{
	XML_ELEMENT_LIST::iterator	itEL;
	CXmlElement * pclsElement = NULL;
	int iCount = 0;

	if( iIndex < 0 ) iIndex = 0;

	for( itEL = m_clsElementList.begin(); itEL != m_clsElementList.end(); ++itEL )
	{
		if( !strcmp( pszName, itEL->GetName() ) )
		{
			if( iCount == iIndex )
			{
				return &(*itEL);
			}

			++iCount;
		}
		else
		{
			pclsElement = SelectElement( itEL->GetElementList(), pszName, iIndex, iCount );
			if( pclsElement )
			{
				return pclsElement;
			}
		}
	}

	return NULL;
}

/**
 * @ingroup XmlParser
 * @brief XML �� ��� ���� element �߿��� �Էµ� �̸��� ��ġ�ϴ� ���� �˻��Ѵ�.
 * @param pclsList	element ����Ʈ
 * @param pszName		�̸�
 * @param iIndex		����
 * @param iCount		���� �˻��� �̸� ����
 * @returns �˻��Ǹ� �ش� ���� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
const char * CXmlSearch::SelectElementData( XML_ELEMENT_LIST * pclsList, const char * pszName, int iIndex, int & iCount )
{
	XML_ELEMENT_LIST::iterator	itEL;
	const char * pszData = NULL;

	for( itEL = pclsList->begin(); itEL != pclsList->end(); ++itEL )
	{
		if( !strcmp( pszName, itEL->GetName() ) )
		{
			if( iCount == iIndex )
			{
				return itEL->GetData();
			}

			++iCount;
		}
		else
		{
			pszData = SelectElementData( itEL->GetElementList(), pszName, iIndex, iCount );
			if( pszData )
			{
				return pszData;
			}
		}
	}

	return NULL;
}

/**
 * @ingroup XmlParser
 * @brief XML �� ��� ���� element �߿��� �Էµ� �̸��� ��ġ�ϴ� element �� �˻��Ѵ�.
 * @param pclsList	element ����Ʈ
 * @param pszName		�̸�
 * @param iIndex		����
 * @param iCount		���� �˻��� �̸� ����
 * @returns �˻��Ǹ� �ش� element �� �����͸� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CXmlElement * CXmlSearch::SelectElement( XML_ELEMENT_LIST * pclsList, const char * pszName, int iIndex, int & iCount )
{
	XML_ELEMENT_LIST::iterator	itEL;
	CXmlElement * pclsElement = NULL;

	for( itEL = pclsList->begin(); itEL != pclsList->end(); ++itEL )
	{
		if( !strcmp( pszName, itEL->GetName() ) )
		{
			if( iCount == iIndex )
			{
				return &(*itEL);
			}

			++iCount;
		}
		else
		{
			pclsElement = SelectElement( itEL->GetElementList(), pszName, iIndex, iCount );
			if( pclsElement )
			{
				return pclsElement;
			}
		}
	}

	return NULL;
}