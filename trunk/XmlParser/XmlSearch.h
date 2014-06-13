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
#include <vector>

typedef std::vector< std::string > XML_NAME_LIST;

/**
 * @ingroup XmlParser
 * @brief XML �� ��� ���� element ���� ��������� �˻��ϴ� ����� �����ϴ� Ŭ����
 */
class CXmlSearch : public CXmlElement
{
public:
	CXmlSearch();
	~CXmlSearch();

	const char * SelectAttribute( const char * pszName, const char * pszAttrName, const int iIndex = 0 );
	bool SelectAttribute( const char * pszName, const char * pszAttrName, std::string & strValue, const int iIndex = 0 );
	bool SelectAttribute( const char * pszName, const char * pszAttrName, int & iValue, const int iIndex = 0 );
	bool SelectAttribute( const char * pszName, const char * pszAttrName, bool & bValue, const int iIndex = 0 );

	bool SelectElementData( const char * pszName, std::string & strData, const int iIndex = 0 );
	bool SelectElementData( const char * pszName, int & iData, const int iIndex = 0 );
	bool SelectElementData( const char * pszName, bool & bData, const int iIndex = 0 );

	bool SelectElementData( const char * pszName, const char * pszChildName, std::string & strData, const int iIndex = 0 );
	bool SelectElementData( const char * pszName, const char * pszChildName, int & iData, const int iIndex = 0 );
	bool SelectElementData( const char * pszName, const char * pszChildName, bool & bData, const int iIndex = 0 );

	bool SelectElementData( const int iIndex, std::string & strData, int iNameCount, ... );
	bool SelectElementData( const int iIndex, int & iData, int iNameCount, ... );
	bool SelectElementData( const int iIndex, bool & bData, int iNameCount, ... );

	CXmlElement * SelectElement( const char * pszName, const int iIndex = 0 );
	CXmlElement * SelectElement( const char * pszName, const char * pszChildName, const int iIndex = 0 );
	CXmlElement * SelectElement( const int iIndex, int iNameCount, ... );

private:
	CXmlElement * SelectElement( XML_ELEMENT_LIST * pclsList, const char * pszName, const int iIndex, int & iCount );
	CXmlElement * SelectElement( XML_ELEMENT_LIST * pclsList, const char * pszName, const char * pszChildName, const int iIndex, int & iCount );
	CXmlElement * SelectElement( XML_ELEMENT_LIST * pclsList, XML_NAME_LIST & clsNameList, int iNameIndex, const int iIndex, int & iCount );
};

#endif
