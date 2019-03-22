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
 * @brief XML attribute map. key 는 attribute 이름이고 value 는 attribute 값이다.
 */
typedef std::map< std::string, std::string > XML_ATTRIBUTE_MAP;

/**
 * @ingroup XmlParser
 * @brief CXmlElement 객체 리스트
 */
typedef std::list< CXmlElement > XML_ELEMENT_LIST;

/**
 * @defgroup XmlParser XmlParser
 * Xml 파싱/생성 라이브러리
 * Xml 파싱/생성 라이브러리
 */

/**
 * @ingroup XmlParser
 * @brief XML 정보 저장 클래스
 */
class CXmlElement
{
public:
	CXmlElement( const char * pszName = NULL );
	~CXmlElement();

	int Parse( const char * pszText, int iTextLen );
	int Parse( std::string & strText );

	int ToString( char * pszText, int iTextSize, bool bUseTab = false, int iDepth = 0 );
	void ToString( std::string & strText, bool bUseTab = false, int iDepth = 0 );
	void Clear( );

	bool ParseFile( const char * pszFileName );
	bool WriteFile( const char * pszFileName, bool bUseTab );

	const char * SelectAttribute( const char * pszName );
	bool SelectAttribute( const char * pszName, std::string & strValue );
	bool SelectAttributeTrim( const char * pszName, std::string & strValue );
	bool SelectAttribute( const char * pszName, int & iValue );
	bool SelectAttribute( const char * pszName, int64_t & iValue );
	bool SelectAttribute( const char * pszName, bool & bValue );
	bool SelectAttribute( const char * pszName, double & dbValue );

	CXmlElement * SelectElement( const char * pszName, const int iIndex = 0 );
	CXmlElement * SelectElement( const int iIndex );
	bool SelectElementList( const char * pszName, XML_ELEMENT_LIST & clsList );

	bool SelectElementData( const char * pszName, std::string & strData, const int iIndex = 0 );
	bool SelectElementTrimData( const char * pszName, std::string & strData, const int iIndex = 0 );
	bool SelectElementData( const char * pszName, int & iData, const int iIndex = 0 );
	bool SelectElementData( const char * pszName, int64_t & iData, const int iIndex = 0 );
	bool SelectElementData( const char * pszName, bool & bData, const int iIndex = 0 );
	bool SelectElementData( const char * pszName, double & dbData, const int iIndex = 0 );

	const char * GetName();
	const char * GetData();

	bool IsDataEmpty();

	void SetName( const char * pszName );
	void SetData( const char * pszData );

	void InsertElementData( const char * pszName, CXmlElement * pclsElement );
	void InsertElementData( const char * pszName, const char * pszData );
	void InsertElementData( const char * pszName, std::string & strData );
	void InsertElementData( const char * pszName, int iData );
	void InsertElementData( const char * pszName, int64_t iData );
	void InsertElementData( const char * pszName, bool bData );
	void InsertElementData( const char * pszName, double dbData );
	void InsertElement( CXmlElement * pclsElement );

	bool UpdateElementData( const char * pszName, const char * pszData, const int iIndex = 0 );
	bool UpdateElementData( const char * pszName, std::string & strData, const int iIndex = 0 );
	bool UpdateElementData( const char * pszName, int iData, const int iIndex = 0 );
	bool UpdateElementData( const char * pszName, int64_t iData, const int iIndex = 0 );
	bool UpdateElementData( const char * pszName, bool bData, const int iIndex = 0 );
	bool UpdateElementData( const char * pszName, double dbData, const int iIndex = 0 );

	void InsertAttribute( const char * pszName, const char * pszValue );
	void InsertAttribute( const char * pszName, std::string & strValue );
	void InsertAttribute( const char * pszName, int iValue );
	void InsertAttribute( const char * pszName, int64_t iValue );
	void InsertAttribute( const char * pszName, bool bValue );
	void InsertAttribute( const char * pszName, double dbValue );

	XML_ELEMENT_LIST * GetElementList();

protected:
	std::string	m_strName;
	std::string m_strData;

	XML_ATTRIBUTE_MAP	m_clsAttributeMap;
	XML_ELEMENT_LIST	m_clsElementList;

	bool GetBoolean( const char * pszData );
};

#endif
