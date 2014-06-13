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

CXmlSearch::CXmlSearch()
{
}

CXmlSearch::~CXmlSearch()
{
}

/**
 * @ingroup XmlParser
 * @brief 모든 하위 Element 를 검색하여서 내용을 저장한다.
 * @param pszName		하위 Element 이름
 * @param strData		하위 Elemnet 의 내용을 저장할 변수
 * @param iIndex		하위 Element 인덱스. 0 을 입력하면 첫번째 검색된 하위 Element 를 리턴하고 1 을 입력하면 두번째 검색된 하위 Element 를 리턴한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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
 * @brief 모든 하위 Element 를 검색하여서 정수 내용을 가져온다.
 * @param pszName 하위 Element 이름
 * @param iData	하위 Element 의 값을 저장하는 변수
 * @param iIndex	하위 Element 인덱스. 0 을 입력하면 첫번째 검색된 하위 Element 를 리턴하고 1 을 입력하면 두번째 검색된 하위 Element 를 리턴한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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
 * @brief 모든 하위 Element 를 검색하여서 bool 내용을 가져온다.
 * @param pszName 하위 Element 이름
 * @param bData		하위 Element 의 값을 저장하는 변수
 * @param iIndex		하위 Element 인덱스. 0 을 입력하면 첫번째 검색된 하위 Element 를 리턴하고 1 을 입력하면 두번째 검색된 하위 Element 를 리턴한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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
 * @brief 모든 하위 Element 를 검색하여서 내용을 저장한다.
 * @param pszName		Element 이름
 * @param pszChildName 하위 element 이름
 * @param strData		하위 Elemnet 의 내용을 저장할 변수
 * @param iIndex		하위 Element 인덱스. 0 을 입력하면 첫번째 검색된 하위 Element 를 리턴하고 1 을 입력하면 두번째 검색된 하위 Element 를 리턴한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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
 * @brief 모든 하위 Element 를 검색하여서 정수 내용을 가져온다.
 * @param pszName Element 이름
 * @param pszChildName 하위 element 이름
 * @param iData		하위 Element 의 값을 저장하는 변수
 * @param iIndex		하위 Element 인덱스. 0 을 입력하면 첫번째 검색된 하위 Element 를 리턴하고 1 을 입력하면 두번째 검색된 하위 Element 를 리턴한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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
 * @brief 모든 하위 Element 를 검색하여서 bool 내용을 가져온다.
 * @param pszName Element 이름
 * @param pszChildName 하위 element 이름
 * @param bData		하위 Element 의 값을 저장하는 변수
 * @param iIndex		하위 Element 인덱스. 0 을 입력하면 첫번째 검색된 하위 Element 를 리턴하고 1 을 입력하면 두번째 검색된 하위 Element 를 리턴한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
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
 * @ingroup XmlParser
 * @brief XML 의 모든 하위 element 중에서 입력된 이름과 일치하는 element 를 검색한다.
 * @param pszName 이름
 * @param iIndex 순번. 0 을 입력하면 첫번째 검색된 element 를 검색한다. 2 를 입력하면 세번째 검색된 element 를 검색한다.
 * @returns 검색되면 해당 element 의 포인터를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CXmlElement * CXmlSearch::SelectElement( const char * pszName, const int iIndex )
{
	int iCount = 0;

	if( iIndex < 0 ) return NULL;

	return SelectElement( &m_clsElementList, pszName, iIndex, iCount );
}

/**
 * @ingroup XmlParser
 * @brief XML 의 모든 하위 element 중에서 입력된 이름과 일치하는 element 를 검색한다.
 * @param pszName 이름
 * @param pszChildName 하위 이름
 * @param iIndex 순번. 0 을 입력하면 첫번째 검색된 element 를 검색한다. 2 를 입력하면 세번째 검색된 element 를 검색한다.
 * @returns 검색되면 해당 element 의 포인터를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */

CXmlElement * CXmlSearch::SelectElement( const char * pszName, const char * pszChildName, const int iIndex )
{
	int iCount = 0;

	if( iIndex < 0 ) return NULL;

	return SelectElement( &m_clsElementList, pszName, pszChildName, iIndex, iCount );
}

/**
 * @brief N 개의 tag 와 일치하는 element 를 검색한다.
 * @param iIndex 순번. 0 을 입력하면 첫번째 검색된 element 를 검색한다. 2 를 입력하면 세번째 검색된 element 를 검색한다.
 * @param iNameCount 이름 개수
 * @param ... 이름
 * @returns 검색되면 해당 element 의 포인터를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CXmlElement * CXmlSearch::SelectElement( int iIndex, int iNameCount, ... )
{
	int iCount = 0;

	if( iIndex < 0 ) return NULL;
	if( iNameCount <= 0 ) return NULL;

	va_list		ap;
	char * pszTemp;
	XML_NAME_LIST clsNameList;

	va_start( ap, iNameCount );
	for( int i = 0; i < iNameCount ; ++i )
	{
		pszTemp = va_arg( ap, char * );
		if( pszTemp == NULL ) break;

		clsNameList.push_back( pszTemp );
	}
	va_end( ap );

	return SelectElement( &m_clsElementList, clsNameList, 0, iIndex, iCount );
}

/**
 * @ingroup XmlParser
 * @brief XML 의 모든 하위 element 중에서 입력된 이름과 일치하는 element 를 검색한다.
 * @param pclsList	element 리스트
 * @param pszName		이름
 * @param iIndex		순번
 * @param iCount		현재 검색된 이름 개수
 * @returns 검색되면 해당 element 의 포인터를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
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
 * @brief XML 의 모든 하위 element 중에서 입력된 이름과 일치하는 element 를 검색한다.
 * @param pclsList	element 리스트
 * @param pszName		이름
 * @param pszChildName 하위 이름
 * @param iIndex		순번
 * @param iCount		현재 검색된 이름 개수
 * @returns 검색되면 해당 element 의 포인터를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
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
 * @brief N 개의 tag 와 일치하는 element 를 검색한다.
 * @param pclsList		element 리스트
 * @param clsNameList 검색 이름 리스트. 상위 tag 이름부터 하위 tag 이름으로 순서대로 저장되어야 한다.
 * @param iNameIndex	검색 이름 리스트에서 현재 검색할 이름 인덱스
 * @param iIndex			순번
 * @param iCount			현재 검색된 이름 개수
 * @returns 검색되면 해당 element 의 포인터를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CXmlElement * CXmlSearch::SelectElement( XML_ELEMENT_LIST * pclsList, XML_NAME_LIST & clsNameList, int iNameIndex, const int iIndex, int & iCount )
{
	XML_ELEMENT_LIST::iterator	itEL;
	CXmlElement * pclsElement = NULL;

	int iNameSize = clsNameList.size();
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
