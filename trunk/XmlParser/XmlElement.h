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

#ifndef _XML_ELEMENT_H_
#define _XML_ELEMENT_H_

#include "SipPlatformDefine.h"
#include <string>
#include <list>
#include <map>

class CXmlElement;

/**
 * @ingroup XmlParser
 * @brief XML attribute map. key �� attribute �̸��̰� value �� attribute ���̴�.
 */
typedef std::map< std::string, std::string > XML_ATTRIBUTE_MAP;

/**
 * @ingroup XmlParser
 * @brief CXmlElement ��ü ����Ʈ
 */
typedef std::list< CXmlElement > XML_ELEMENT_LIST;

/**
 * @defgroup XmlParser XmlParser
 * Xml �Ľ�/���� ���̺귯��
 * Xml �Ľ�/���� ���̺귯��
 */

/**
 * @ingroup XmlParser
 * @brief XML ���� ���� Ŭ����
 */
class CXmlElement
{
public:
	CXmlElement();
	~CXmlElement();

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	void Clear( );

	bool ParseFile( const char * pszFileName );

	const char * SelectAttribute( const char * pszName );
	bool SelectAttribute( const char * pszName, std::string & strValue );
	bool SelectAttributeTrim( const char * pszName, std::string & strValue );
	bool SelectAttribute( const char * pszName, int & iValue );
	bool SelectAttribute( const char * pszName, bool & bValue );

	CXmlElement * SelectElement( const char * pszName, const int iIndex = 0 );
	bool SelectElementList( const char * pszName, XML_ELEMENT_LIST & clsList );

	bool SelectElementData( const char * pszName, std::string & strData, const int iIndex = 0 );
	bool SelectElementTrimData( const char * pszName, std::string & strData, const int iIndex = 0 );
	bool SelectElementData( const char * pszName, int & iData, const int iIndex = 0 );
	bool SelectElementData( const char * pszName, int64_t & iData, const int iIndex = 0 );
	bool SelectElementData( const char * pszName, bool & bData, const int iIndex = 0 );

	const char * GetName();
	const char * GetData();

	bool IsDataEmpty();

	XML_ELEMENT_LIST * GetElementList();

protected:
	std::string	m_strName;
	std::string m_strData;

	XML_ATTRIBUTE_MAP	m_clsAttributeMap;
	XML_ELEMENT_LIST	m_clsElementList;

	bool GetBoolean( const char * pszData );
};

#endif
