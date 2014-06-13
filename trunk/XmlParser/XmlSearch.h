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

#ifndef _XML_SEARCH_H_
#define _XML_SEARCH_H_

#include "XmlElement.h"

/**
 * @ingroup XmlParser
 * @brief XML 의 모든 하위 element 까지 재귀적으로 검색하는 기능을 수행하는 클래스
 */
class CXmlSearch : public CXmlElement
{
public:
	CXmlSearch();
	~CXmlSearch();

	const char * SelectElementData( const char * pszName, int iIndex = 0 );
	bool SelectElementData( const char * pszName, std::string & strData, int iIndex = 0 );
	bool SelectElementData( const char * pszName, int & iData, int iIndex = 0 );
	bool SelectElementData( const char * pszName, bool & bData, int iIndex = 0 );

	const char * SelectElementData( const char * pszName, const char * pszChildName, int iIndex = 0 );
	bool SelectElementData( const char * pszName, const char * pszChildName, std::string & strData, int iIndex = 0 );
	bool SelectElementData( const char * pszName, const char * pszChildName, int & iData, int iIndex = 0 );
	bool SelectElementData( const char * pszName, const char * pszChildName, bool & bData, int iIndex = 0 );

	CXmlElement * SelectElement( const char * pszName, int iIndex = 0 );
	CXmlElement * SelectElement( const char * pszName, const char * pszChildName, int iIndex = 0 );

private:
	CXmlElement * SelectElement( XML_ELEMENT_LIST * pclsList, const char * pszName, int iIndex, int & iCount );
	CXmlElement * SelectElement( XML_ELEMENT_LIST * pclsList, const char * pszName, const char * pszChildName, int iIndex, int & iCount );
};

#endif
