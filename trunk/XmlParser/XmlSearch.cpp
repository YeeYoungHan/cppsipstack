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
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define MAKE_NAME_LIST va_list ap; \
	char * pszTemp; \
	XML_NAME_LIST clsNameList; \
	va_start( ap, iNameCount ); \
	for( int i = 0; i < iNameCount ; ++i ) \
	{ \
		pszTemp = va_arg( ap, char * ); \
		if( pszTemp == NULL ) break; \
		clsNameList.push_back( pszTemp ); \
	} \
	va_end( ap );

CXmlSearch::CXmlSearch()
{
}

CXmlSearch::~CXmlSearch()
{
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ѵ�.
 * @param pszName Element �̸�
 * @param pszAttrName ��Ʈ����Ʈ �̸�
 * @param iIndex		Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� ��Ʈ����Ʈ�� ���� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
const char * CXmlSearch::SelectAttribute( const char * pszName, const char * pszAttrName, const int iIndex )
{
	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		return pclsElement->SelectAttribute( pszAttrName );
	}

	return NULL;
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ѵ�.
 * @param pszName Element �̸�
 * @param pszAttrName ��Ʈ����Ʈ �̸�
 * @param strValue	��Ʈ����Ʈ ��
 * @param iIndex		Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlSearch::SelectAttribute( const char * pszName, const char * pszAttrName, std::string & strValue, const int iIndex )
{
	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		return pclsElement->SelectAttribute( pszAttrName, strValue );
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ͽ� int ������ �����Ѵ�.
 * @param pszName Element �̸�
 * @param pszAttrName ��Ʈ����Ʈ �̸�
 * @param iValue		��Ʈ����Ʈ ��
 * @param iIndex		Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlSearch::SelectAttribute( const char * pszName, const char * pszAttrName, int & iValue, const int iIndex )
{
	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		return pclsElement->SelectAttribute( pszAttrName, iValue );
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ͽ� bool ������ �����Ѵ�.
 * @param pszName Element �̸�
 * @param pszAttrName ��Ʈ����Ʈ �̸�
 * @param bValue		��Ʈ����Ʈ ��
 * @param iIndex		Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlSearch::SelectAttribute( const char * pszName, const char * pszAttrName, bool & bValue, const int iIndex )
{
	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		return pclsElement->SelectAttribute( pszAttrName, bValue );
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ��� ���� Element �� �˻��Ͽ��� ������ �����Ѵ�.
 * @param pszName		Element �̸�
 * @param strData		Elemnet �� ������ ������ ����
 * @param iIndex		Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlSearch::SelectElementData( const char * pszName, std::string & strData, const int iIndex )
{
	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		strData = pclsElement->GetData();
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ��� ���� Element �� �˻��Ͽ��� ���� ������ �����´�.
 * @param pszName Element �̸�
 * @param iData	Element �� ���� �����ϴ� ����
 * @param iIndex	Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlSearch::SelectElementData( const char * pszName, int & iData, const int iIndex )
{
	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		iData = atoi( pclsElement->GetData() );

		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ��� ���� Element �� �˻��Ͽ��� bool ������ �����´�.
 * @param pszName Element �̸�
 * @param bData		Element �� ���� �����ϴ� ����
 * @param iIndex	Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlSearch::SelectElementData( const char * pszName, bool & bData, const int iIndex )
{
	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		bData = GetBoolean( pclsElement->GetData() );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ��� ���� Element �� �˻��Ͽ��� ������ �����Ѵ�.
 * @param pszName		Element �̸�
 * @param pszChildName ���� Element �̸�
 * @param strData		Elemnet �� ������ ������ ����
 * @param iIndex		Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlSearch::SelectElementData( const char * pszName, const char * pszChildName, std::string & strData, const int iIndex )
{
	CXmlElement * pclsElement = SelectElement( pszName, pszChildName, iIndex );
	if( pclsElement )
	{
		strData = pclsElement->GetData();
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ��� ���� Element �� �˻��Ͽ��� ���� ������ �����´�.
 * @param pszName Element �̸�
 * @param pszChildName ���� element �̸�
 * @param iData		���� Element �� ���� �����ϴ� ����
 * @param iIndex		���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlSearch::SelectElementData( const char * pszName, const char * pszChildName, int & iData, const int iIndex )
{
	CXmlElement * pclsElement = SelectElement( pszName, pszChildName, iIndex );
	if( pclsElement )
	{
		iData = atoi( pclsElement->GetData() );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ��� ���� Element �� �˻��Ͽ��� bool ������ �����´�.
 * @param pszName Element �̸�
 * @param pszChildName ���� element �̸�
 * @param bData		���� Element �� ���� �����ϴ� ����
 * @param iIndex		���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlSearch::SelectElementData( const char * pszName, const char * pszChildName, bool & bData, const int iIndex )
{
	CXmlElement * pclsElement = SelectElement( pszName, pszChildName, iIndex );
	if( pclsElement )
	{
		bData = GetBoolean( pclsElement->GetData() );
		return true;
	}

	return false;
}

/**
 * @brief N ���� tag �� ��ġ�ϴ� element �� ���� �˻��Ѵ�.
 * @param iIndex ����. 0 �� �Է��ϸ� ù��° �˻��� element �� �˻��Ѵ�. 2 �� �Է��ϸ� ����° �˻��� element �� �˻��Ѵ�.
 * @param strData �˻��� ������ ���� ����
 * @param iNameCount �̸� ����
 * @param ... �̸�
 * @returns �˻��Ǹ� �ش� element �� �����͸� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
bool CXmlSearch::SelectElementData( const int iIndex, std::string & strData, int iNameCount, ... )
{
	if( iIndex < 0 ) return false;
	if( iNameCount <= 0 ) return false;

	int iCount = 0;
	MAKE_NAME_LIST

	CXmlElement * pclsElement = SelectElement( &m_clsElementList, clsNameList, 0, iIndex, iCount );
	if( pclsElement )
	{
		strData = pclsElement->GetData();
		return true;
	}

	return false;
}

/**
 * @brief N ���� tag �� ��ġ�ϴ� element �� ���� �˻��Ѵ�.
 * @param iIndex ����. 0 �� �Է��ϸ� ù��° �˻��� element �� �˻��Ѵ�. 2 �� �Է��ϸ� ����° �˻��� element �� �˻��Ѵ�.
 * @param iData �˻��� ������ ���� ����
 * @param iNameCount �̸� ����
 * @param ... �̸�
 * @returns �˻��Ǹ� �ش� element �� �����͸� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
bool CXmlSearch::SelectElementData( const int iIndex, int & iData, int iNameCount, ... )
{
	if( iIndex < 0 ) return false;
	if( iNameCount <= 0 ) return false;

	int iCount = 0;
	MAKE_NAME_LIST

	CXmlElement * pclsElement = SelectElement( &m_clsElementList, clsNameList, 0, iIndex, iCount );
	if( pclsElement )
	{
		iData = atoi( pclsElement->GetData() );
		return true;
	}

	return false;
}

/**
 * @brief N ���� tag �� ��ġ�ϴ� element �� ���� �˻��Ѵ�.
 * @param iIndex ����. 0 �� �Է��ϸ� ù��° �˻��� element �� �˻��Ѵ�. 2 �� �Է��ϸ� ����° �˻��� element �� �˻��Ѵ�.
 * @param bData �˻��� ������ ���� ����
 * @param iNameCount �̸� ����
 * @param ... �̸�
 * @returns �˻��Ǹ� �ش� element �� �����͸� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
bool CXmlSearch::SelectElementData( const int iIndex, bool & bData, int iNameCount, ... )
{
	if( iIndex < 0 ) return false;
	if( iNameCount <= 0 ) return false;

	int iCount = 0;
	MAKE_NAME_LIST

	CXmlElement * pclsElement = SelectElement( &m_clsElementList, clsNameList, 0, iIndex, iCount );
	if( pclsElement )
	{
		bData = GetBoolean( pclsElement->GetData() );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief XML �� ��� ���� element �߿��� �Էµ� �̸��� ��ġ�ϴ� element �� �˻��Ѵ�.
 * @param pszName �̸�
 * @param iIndex ����. 0 �� �Է��ϸ� ù��° �˻��� element �� �˻��Ѵ�. 2 �� �Է��ϸ� ����° �˻��� element �� �˻��Ѵ�.
 * @returns �˻��Ǹ� �ش� element �� �����͸� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CXmlElement * CXmlSearch::SelectElement( const char * pszName, const int iIndex )
{
	int iCount = 0;

	if( iIndex < 0 ) return NULL;

	return SelectElement( &m_clsElementList, pszName, iIndex, iCount );
}

/**
 * @ingroup XmlParser
 * @brief XML �� ��� ���� element �߿��� �Էµ� �̸��� ��ġ�ϴ� element �� �˻��Ѵ�.
 * @param pszName Element �̸�
 * @param pszChildName ���� Element �̸�
 * @param iIndex ����. 0 �� �Է��ϸ� ù��° �˻��� element �� �˻��Ѵ�. 2 �� �Է��ϸ� ����° �˻��� element �� �˻��Ѵ�.
 * @returns �˻��Ǹ� �ش� element �� �����͸� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */

CXmlElement * CXmlSearch::SelectElement( const char * pszName, const char * pszChildName, const int iIndex )
{
	int iCount = 0;

	if( iIndex < 0 ) return NULL;

	return SelectElement( &m_clsElementList, pszName, pszChildName, iIndex, iCount );
}

/**
 * @brief N ���� tag �� ��ġ�ϴ� element �� �˻��Ѵ�.
 * @param iIndex ����. 0 �� �Է��ϸ� ù��° �˻��� element �� �˻��Ѵ�. 2 �� �Է��ϸ� ����° �˻��� element �� �˻��Ѵ�.
 * @param iNameCount �̸� ����
 * @param ... �̸�
 * @returns �˻��Ǹ� �ش� element �� �����͸� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CXmlElement * CXmlSearch::SelectElement( const int iIndex, int iNameCount, ... )
{
	if( iIndex < 0 ) return NULL;
	if( iNameCount <= 0 ) return NULL;

	int iCount = 0;
	MAKE_NAME_LIST

	return SelectElement( &m_clsElementList, clsNameList, 0, iIndex, iCount );
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
CXmlElement * CXmlSearch::SelectElement( XML_ELEMENT_LIST * pclsList, const char * pszName, const int iIndex, int & iCount )
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

/**
 * @ingroup XmlParser
 * @brief XML �� ��� ���� element �߿��� �Էµ� �̸��� ��ġ�ϴ� element �� �˻��Ѵ�.
 * @param pclsList	element ����Ʈ
 * @param pszName		�̸�
 * @param pszChildName ���� �̸�
 * @param iIndex		����
 * @param iCount		���� �˻��� �̸� ����
 * @returns �˻��Ǹ� �ش� element �� �����͸� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CXmlElement * CXmlSearch::SelectElement( XML_ELEMENT_LIST * pclsList, const char * pszName, const char * pszChildName, const int iIndex, int & iCount )
{
	XML_ELEMENT_LIST::iterator	itEL;
	CXmlElement * pclsElement = NULL;

	for( itEL = pclsList->begin(); itEL != pclsList->end(); ++itEL )
	{
		if( !strcmp( pszName, itEL->GetName() ) )
		{
			pclsElement = SelectElement( itEL->GetElementList(), pszChildName, iIndex, iCount );
			if( pclsElement )
			{
				return pclsElement;
			}
		}
		else
		{
			pclsElement = SelectElement( itEL->GetElementList(), pszName, pszChildName, iIndex, iCount );
			if( pclsElement )
			{
				return pclsElement;
			}
		}
	}

	return NULL;
}

/**
 * @brief N ���� tag �� ��ġ�ϴ� element �� �˻��Ѵ�.
 * @param pclsList		element ����Ʈ
 * @param clsNameList �˻� �̸� ����Ʈ. ���� tag �̸����� ���� tag �̸����� ������� ����Ǿ�� �Ѵ�.
 * @param iNameIndex	�˻� �̸� ����Ʈ���� ���� �˻��� �̸� �ε���
 * @param iIndex			����
 * @param iCount			���� �˻��� �̸� ����
 * @returns �˻��Ǹ� �ش� element �� �����͸� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CXmlElement * CXmlSearch::SelectElement( XML_ELEMENT_LIST * pclsList, XML_NAME_LIST & clsNameList, int iNameIndex, const int iIndex, int & iCount )
{
	XML_ELEMENT_LIST::iterator	itEL;
	CXmlElement * pclsElement = NULL;

	int iNameSize = (int)clsNameList.size();
	const char * pszName = clsNameList[iNameIndex].c_str();

	for( itEL = pclsList->begin(); itEL != pclsList->end(); ++itEL )
	{
		if( !strcmp( pszName, itEL->GetName() ) )
		{
			if( iNameSize == ( iNameIndex + 1 ) )
			{
				if( iCount == iIndex )
				{
					return &(*itEL);
				}

				++iCount;
			}
			else
			{
				pclsElement = SelectElement( itEL->GetElementList(), clsNameList, iNameIndex + 1, iIndex, iCount );
				if( pclsElement )
				{
					return pclsElement;
				}
			}
		}
		else
		{
			pclsElement = SelectElement( itEL->GetElementList(), clsNameList, iNameIndex, iIndex, iCount );
			if( pclsElement )
			{
				return pclsElement;
			}
		}
	}

	return NULL;
}
