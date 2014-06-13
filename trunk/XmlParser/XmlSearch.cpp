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
 * @brief XML ���� element �� �̸��� ���� ���� �˻��Ѵ�.
 * @param pszName element �̸�
 * @param iIndex	����. 0 �� �Է��ϸ� ù��° �˻��� element ���� �˻��Ѵ�. 2 �� �Է��ϸ� ����° �˻��� element ���� �˻��Ѵ�.
 * @returns �˻��Ǹ� �ش� ���� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
const char * CXmlSearch::SelectData( const char * pszName, int iIndex )
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
			pszData = SelectData( itEL->GetElementList(), pszName, iIndex, iCount );
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
 * @brief XML ���� element �� �̸��� ���� ���� �˻��Ѵ�.
 * @param pclsList	element ����Ʈ
 * @param pszName		�̸�
 * @param iIndex		����
 * @param iCount		���� �˻��� �̸� ����
 * @returns �˻��Ǹ� �ش� ���� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
const char * CXmlSearch::SelectData( XML_ELEMENT_LIST * pclsList, const char * pszName, int iIndex, int & iCount )
{
	XML_ELEMENT_LIST::iterator	itEL;

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
	}

	return NULL;
}
