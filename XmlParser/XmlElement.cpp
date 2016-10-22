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

#include "SipPlatformDefine.h"
#include "XmlElement.h"
#include "StringUtility.h"
#include <stdlib.h>
#include "Log.h"
#include "MemoryDebug.h"

/**
 * @ingroup XmlParser
 * @brief 생성자
 */
CXmlElement::CXmlElement()
{
}

/**
 * @ingroup XmlParser
 * @brief 소멸자
 */
CXmlElement::~CXmlElement()
{
}

#define XML_ELEMENT_NULL				0
#define XML_ELEMENT_NAME				1
#define XML_ELEMENT_ATTR				2
#define XML_ELEMENT_DATA				3
#define XML_ELEMENT_NAME_END		4
#define XML_ELEMENT_DATA_PARSE	5
#define XML_ELEMENT_COMMENT			6
#define XML_ELEMENT_CDATA				7
#define XML_ELEMENT_DECLARATION	8

/**
 * @ingroup XmlParser
 * @brief XML 문자열을 파싱하여서 멤버 변수에 저장한다.
 * @param pszText		XML 문자열
 * @param iTextLen	XML 문자열 길이
 * @returns 성공하면 파싱한 XML 문자열의 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CXmlElement::Parse( const char * pszText, int iTextLen )
{
	int		iPos, iStartPos = -1, iLen;
	char	cType = XML_ELEMENT_NULL, cTypeOld = XML_ELEMENT_NULL;
	std::string	strName, strValue;

	Clear();

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( cType == XML_ELEMENT_COMMENT )
		{
			if( pszText[iPos] == '-' )
			{
				if( iPos + 3 < iTextLen && !strncmp( pszText + iPos + 1, "->", 2 ) )
				{
					iPos += 2;
					cType = cTypeOld;
				}
			}

			continue;
		}
		else if( cType == XML_ELEMENT_DECLARATION )
		{
			if( pszText[iPos] == '>' )
			{
				cType = XML_ELEMENT_NULL;
			}

			continue;
		}

		if( pszText[iPos] == ' ' || pszText[iPos] == '\t' || pszText[iPos] == '/' || pszText[iPos] == '\r' || pszText[iPos] == '\n' )
		{
			if( cType == XML_ELEMENT_NAME )
			{
				m_strName.append( pszText + iStartPos, iPos - iStartPos );
				cType = XML_ELEMENT_ATTR;
				iStartPos = -1;
				strName.clear();
				strValue.clear();
			}
		}
		else if( cType == XML_ELEMENT_CDATA )
		{
			if( pszText[iPos] == ']' )
			{
				if( iPos + 3 < iTextLen && !strncmp( pszText + iPos + 1, "]>", 2 ) )
				{
					m_strData.append( pszText + iStartPos, iPos - iStartPos );
					cType = XML_ELEMENT_DATA;
					iPos += 2;
					iStartPos = iPos + 1;
				}
			}
		}
		else if( pszText[iPos] == '<' )
		{
			if( iPos + 4 < iTextLen && !strncmp( pszText + iPos + 1, "!--", 3 ) )
			{
				cTypeOld = cType;
				cType = XML_ELEMENT_COMMENT;
			}
			else if( iPos + 9 < iTextLen && !strncmp( pszText + iPos + 1, "![CDATA[", 8 ) )
			{
				if( cType == XML_ELEMENT_DATA )
				{
					m_strData.append( pszText + iStartPos, iPos - iStartPos );
				}

				cTypeOld = cType;
				cType = XML_ELEMENT_CDATA;
				iStartPos = iPos + 9;
			}
			else if( iPos + 1 < iTextLen && pszText[iPos+1] == '?' )
			{
				cTypeOld = cType;
				cType = XML_ELEMENT_DECLARATION;
			}
			else if( cType == XML_ELEMENT_NULL )
			{
				cType = XML_ELEMENT_NAME;
				iStartPos = iPos + 1;
			}
			else if( pszText[iPos+1] == '/' )
			{
				if( cType == XML_ELEMENT_DATA )
				{
					m_strData.append( pszText + iStartPos, iPos - iStartPos );		
				}

				iLen = (int)m_strName.length();

				if( iTextLen - ( iPos + 2 ) < (int)( iLen + 1 ) ) 
				{
					CLog::Print( LOG_ERROR, "end tag is not correct for start tag(%s)", m_strName.c_str() );
					return -1;
				}

				if( !strncmp( pszText + iPos + 2, m_strName.c_str(), iLen ) )
				{
					cType = XML_ELEMENT_NAME_END;
					iPos += iLen + 1;
				}
				else
				{
					CLog::Print( LOG_ERROR, "end tag(%.*s) is not correct for start tag(%s)", iLen, pszText + iPos + 2, m_strName.c_str() );
					return -1;
				}
			}
			else
			{
				CXmlElement clsElement;

				iLen = clsElement.Parse( pszText + iPos, iTextLen - iPos );
				if( iLen == -1 ) return -1;

				m_clsElementList.push_back( clsElement );

				cType = XML_ELEMENT_DATA_PARSE;
				iPos += iLen - 1;
			}
		}
		else if( pszText[iPos] == '>' )
		{
			if( cType == XML_ELEMENT_NAME )
			{
				m_strName.append( pszText + iStartPos, iPos - iStartPos );
			}
			else if( cType == XML_ELEMENT_NAME_END )
			{
				++iPos;
				break;
			}
			else if( iPos < 2 )
			{
				CLog::Print( LOG_ERROR, "iPos(%d) < 2 : pszText[iPos](%s)", iPos, pszText + iPos );
				return -1;
			}
			else if( pszText[iPos-1] == '/' )
			{
				++iPos;
				break;
			}

			cType = XML_ELEMENT_DATA;
			iStartPos = iPos + 1;
		}
		else if( cType == XML_ELEMENT_ATTR )
		{
			if( iStartPos == -1 )
			{
				iStartPos = iPos;
			}
			else if( pszText[iPos] == '=' && strName.empty() )
			{
				if( pszText[iPos+1] != '"' )
				{
					CLog::Print( LOG_ERROR, "iPos(%d+1) != '\"' : pszText[iPos](%s)", iPos, pszText + iPos );
					return -1;
				}

				strName.append( pszText + iStartPos, iPos - iStartPos );
				++iPos;
				iStartPos = iPos + 1;
			}
			else if( pszText[iPos] == '"' )
			{
				strValue.append( pszText + iStartPos, iPos - iStartPos );

				m_clsAttributeMap.insert( XML_ATTRIBUTE_MAP::value_type( strName, strValue ) );

				strName.clear();
				strValue.clear();
				iStartPos = -1;
			}
		}
	}

	return iPos;
}

/**
 * @ingroup XmlParser
 * @brief XML 문자열을 파싱하여서 멤버 변수에 저장한다.
 * @param strText XML 문자열
 * @returns 성공하면 파싱한 XML 문자열의 길이를 리턴하고 그렇지 않으면 -1 를 리턴한다.
 */
int CXmlElement::Parse( std::string & strText )
{
	return Parse( strText.c_str(), strText.length() );
}

/**
 * @ingroup XmlParser
 * @brief 멤버 변수에 저장된 값을 이용하여서 XML 문자열을 생성한다.
 * @param pszText			XML 문자열을 저장할 변수
 * @param iTextSize		XML 문자열을 저장할 변수의 크기
 * @returns 생성된 XML 문자열의 길이를 리턴한다.
 */
int CXmlElement::ToString( char * pszText, int iTextSize )
{
	int iLen, n;

	iLen = snprintf( pszText, iTextSize, "<%s", m_strName.c_str() );

	if( m_clsAttributeMap.empty() == false )
	{
		XML_ATTRIBUTE_MAP::iterator	itAM;

		for( itAM = m_clsAttributeMap.begin(); itAM != m_clsAttributeMap.end(); ++itAM )
		{
			iLen += snprintf( pszText + iLen, iTextSize - iLen, " %s=\"%s\"", itAM->first.c_str(), itAM->second.c_str() );
		}
	}

	if( m_strData.empty() == false )
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, ">%s</%s>\n", m_strData.c_str(), m_strName.c_str() );
	}
	else if( m_clsElementList.empty() == false )
	{
		XML_ELEMENT_LIST::iterator	itEL;

		iLen += snprintf( pszText + iLen, iTextSize - iLen, ">\n" );

		for( itEL = m_clsElementList.begin(); itEL != m_clsElementList.end(); ++itEL )
		{
			n = itEL->ToString( pszText + iLen, iTextSize - iLen );
			if( n == -1 ) return -1;
			iLen += n;
		}

		iLen += snprintf( pszText + iLen, iTextSize - iLen, "</%s>\n", m_strName.c_str() );
	}
	else
	{
		iLen += snprintf( pszText + iLen, iTextSize - iLen, "/>\n" );
	}

	return iLen;
}

/**
 * @ingroup XmlParser
 * @brief 멤버 변수에 저장된 값을 이용하여서 XML 문자열을 생성한다.
 * @param strText XML 문자열을 저장할 변수
 */
void CXmlElement::ToString( std::string & strText )
{
	strText.append( "<" );
	strText.append( m_strName );

	if( m_clsAttributeMap.empty() == false )
	{
		XML_ATTRIBUTE_MAP::iterator	itAM;

		for( itAM = m_clsAttributeMap.begin(); itAM != m_clsAttributeMap.end(); ++itAM )
		{
			strText.append( " " );
			strText.append( itAM->first );
			strText.append( "=\"" );
			strText.append( itAM->second );
			strText.append( "\"" );
		}
	}

	if( m_strData.empty() == false )
	{
		strText.append( ">" );
		strText.append( m_strData );
		strText.append( "</" );
		strText.append( m_strName );
		strText.append( ">\n" );
	}
	else if( m_clsElementList.empty() == false )
	{
		XML_ELEMENT_LIST::iterator	itEL;

		strText.append( ">\n" );

		for( itEL = m_clsElementList.begin(); itEL != m_clsElementList.end(); ++itEL )
		{
			itEL->ToString( strText );
		}

		strText.append( "</" );
		strText.append( m_strName );
		strText.append( ">\n" );
	}
	else
	{
		strText.append( "/>\n" );
	}
}

/**
 * @ingroup XmlParser
 * @brief 멤버 변수를 초기화시킨다.
 */
void CXmlElement::Clear( )
{
	m_strName.clear();
	m_strData.clear();
	m_clsAttributeMap.clear();
	m_clsElementList.clear();
}

/**
 * @ingroup XmlParser
 * @brief XML 파일을 읽어서 멤버 변수에 저장한다.
 * @param pszFileName		XML 파일 이름
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CXmlElement::ParseFile( const char * pszFileName )
{
	FILE * fd;
	char	szBuf[1024];
	std::string	strBuf;

	fd = fopen( pszFileName, "r" );
	if( fd == NULL ) return false;

	memset( szBuf, 0, sizeof(szBuf) );
	while( fgets( szBuf, sizeof(szBuf)-1, fd ) )
	{
		strBuf.append( szBuf );
	}
	fclose( fd );

	if( Parse( strBuf.c_str(), (int)strBuf.length() ) == -1 ) return false;

	return true;
}

/**
 * @ingroup XmlParser
 * @brief 애트리뷰트에 해당하는 값을 검색한다.
 * @param pszName 애트리뷰트 이름
 * @returns 성공하면 애트리뷰트의 값을 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
const char * CXmlElement::SelectAttribute( const char * pszName )
{
	XML_ATTRIBUTE_MAP::iterator	itAM;

	itAM = m_clsAttributeMap.find( pszName );
	if( itAM != m_clsAttributeMap.end() )
	{
		return itAM->second.c_str();
	}

	return NULL;
}

/**
 * @ingroup XmlParser
 * @brief 애트리뷰트에 해당하는 값을 검색한다.
 * @param pszName		애트리뷰트 이름
 * @param strValue	애트리뷰트 값
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CXmlElement::SelectAttribute( const char * pszName, std::string & strValue )
{
	XML_ATTRIBUTE_MAP::iterator	itAM;

	strValue.clear();

	itAM = m_clsAttributeMap.find( pszName );
	if( itAM != m_clsAttributeMap.end() )
	{
		strValue = itAM->second;
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief 애트리뷰트에 해당하는 값을 검색한다. 검색에 성공하면 값의 왼쪽, 오른쪽 공백을 제거한다.
 * @param pszName		애트리뷰트 이름
 * @param strValue	애트리뷰트 값
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CXmlElement::SelectAttributeTrim( const char * pszName, std::string & strValue )
{
	XML_ATTRIBUTE_MAP::iterator	itAM;

	strValue.clear();

	itAM = m_clsAttributeMap.find( pszName );
	if( itAM != m_clsAttributeMap.end() )
	{
		strValue = itAM->second;
		TrimString( strValue );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief 애트리뷰트에 해당하는 값을 검색하여 int 변수에 저장한다.
 * @param pszName		애트리뷰트 이름
 * @param iValue		애트리뷰트 값
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CXmlElement::SelectAttribute( const char * pszName, int & iValue )
{
	XML_ATTRIBUTE_MAP::iterator	itAM;

	iValue = -1;

	itAM = m_clsAttributeMap.find( pszName );
	if( itAM != m_clsAttributeMap.end() )
	{
		TrimString( itAM->second );
		iValue = atoi( itAM->second.c_str() );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief 애트리뷰트에 해당하는 값을 검색하여 bool 변수에 저장한다.
 * @param pszName		애트리뷰트 이름
 * @param bValue		애트리뷰트 값
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CXmlElement::SelectAttribute( const char * pszName, bool & bValue )
{
	XML_ATTRIBUTE_MAP::iterator	itAM;

	bValue = false;

	itAM = m_clsAttributeMap.find( pszName );
	if( itAM != m_clsAttributeMap.end() )
	{
		TrimString( itAM->second );
		bValue = GetBoolean( itAM->second.c_str() );

		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief 하위 Element 를 검색한다.
 * @param pszName		하위 Element 이름
 * @param iIndex		하위 Element 인덱스. 0 을 입력하면 첫번째 검색된 하위 Element 를 리턴하고 1 을 입력하면 두번째 검색된 하위 Element 를 리턴한다.
 * @returns 성공하면 하위 Element 객체의 포인터를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CXmlElement * CXmlElement::SelectElement( const char * pszName, const int iIndex )
{
	XML_ELEMENT_LIST::iterator	itEL;
	int iCount = 0;

	if( iIndex < 0 ) return NULL;

	for( itEL = m_clsElementList.begin(); itEL != m_clsElementList.end(); ++itEL )
	{
		if( !strcmp( pszName, itEL->m_strName.c_str() ) )
		{
			if( iCount == iIndex )
			{
				return &(*itEL);
			}

			++iCount;
		}
	}

	return NULL;
}

/**
 * @ingroup XmlParser
 * @brief 하위 Element 를 검색한다.
 * @param iIndex 하위 Element 인덱스. 0 을 입력하면 첫번째 검색된 하위 Element 를 리턴하고 1 을 입력하면 두번째 검색된 하위 Element 를 리턴한다.
 * @returns 성공하면 하위 Element 객체의 포인터를 리턴하고 그렇지 않으면 NULL 을 리턴한다.
 */
CXmlElement * CXmlElement::SelectElement( const int iIndex )
{
	XML_ELEMENT_LIST::iterator	itEL;
	int iCount = 0;

	if( iIndex < 0 ) return NULL;

	for( itEL = m_clsElementList.begin(); itEL != m_clsElementList.end(); ++itEL )
	{
		if( iCount == iIndex )
		{
			return &(*itEL);
		}

		++iCount;
	}

	return NULL;
}

/**
 * @ingroup XmlParser
 * @brief 하위 Element 를 검색하여서 Element 리스트에 저장한다.
 * @param pszName		하위 Element 이름
 * @param clsList		하위 Element 를 저장할 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CXmlElement::SelectElementList( const char * pszName, XML_ELEMENT_LIST & clsList )
{
	XML_ELEMENT_LIST::iterator	itEL;

	clsList.clear();

	for( itEL = m_clsElementList.begin(); itEL != m_clsElementList.end(); ++itEL )
	{
		if( !strcmp( pszName, itEL->m_strName.c_str() ) )
		{
			clsList.push_back( *itEL );
		}
	}

	if( clsList.empty() == false ) return true;

	return false;
}

/**
 * @ingroup XmlParser
 * @brief 하위 Element 를 검색하여서 내용을 저장한다.
 * @param pszName		하위 Element 이름
 * @param strData		하위 Elemnet 의 내용을 저장할 변수
 * @param iIndex		하위 Element 인덱스. 0 을 입력하면 첫번째 검색된 하위 Element 를 리턴하고 1 을 입력하면 두번째 검색된 하위 Element 를 리턴한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CXmlElement::SelectElementData( const char * pszName, std::string & strData, const int iIndex )
{
	strData.clear();

	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		strData = pclsElement->m_strData;
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief 하위 Element 를 검색하여서 내용을 저장한다. 검색에 성공하면 내용의 왼쪽, 오른쪽 공백을 제거한다.
 * @param pszName		하위 Element 이름
 * @param strData		하위 Elemnet 의 내용을 저장할 변수
 * @param iIndex		하위 Element 인덱스. 0 을 입력하면 첫번째 검색된 하위 Element 를 리턴하고 1 을 입력하면 두번째 검색된 하위 Element 를 리턴한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CXmlElement::SelectElementTrimData( const char * pszName, std::string & strData, const int iIndex )
{
	strData.clear();

	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		strData = pclsElement->m_strData;
		TrimString( strData );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief 하위 Element 를 검색하여서 정수 내용을 가져온다.
 * @param pszName 하위 Element 이름
 * @param piData	하위 Element 의 값을 저장하는 변수
 * @param iIndex	하위 Element 인덱스. 0 을 입력하면 첫번째 검색된 하위 Element 를 리턴하고 1 을 입력하면 두번째 검색된 하위 Element 를 리턴한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CXmlElement::SelectElementData( const char * pszName, int & iData, const int iIndex )
{
	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		TrimString( pclsElement->m_strData );
		iData = atoi( pclsElement->m_strData.c_str() );

		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief 하위 Element 를 검색하여서 64bit 정수 내용을 가져온다.
 * @param pszName 하위 Element 이름
 * @param iData		하위 Element 의 값을 저장하는 변수
 * @param iIndex	하위 Element 인덱스. 0 을 입력하면 첫번째 검색된 하위 Element 를 리턴하고 1 을 입력하면 두번째 검색된 하위 Element 를 리턴한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CXmlElement::SelectElementData( const char * pszName, int64_t & iData, const int iIndex )
{
	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		TrimString( pclsElement->m_strData );
		iData = atoll( pclsElement->m_strData.c_str() );

		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief 하위 Element 를 검색하여서 bool 내용을 가져온다.
 * @param pszName 하위 Element 이름
 * @param bData		하위 Element 의 값을 저장하는 변수
 * @param iIndex		하위 Element 인덱스. 0 을 입력하면 첫번째 검색된 하위 Element 를 리턴하고 1 을 입력하면 두번째 검색된 하위 Element 를 리턴한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CXmlElement::SelectElementData( const char * pszName, bool & bData, const int iIndex )
{
	bData = false;

	CXmlElement * pclsElement = SelectElement( pszName, iIndex );
	if( pclsElement )
	{
		TrimString( pclsElement->m_strData );
		bData = GetBoolean( pclsElement->m_strData.c_str() );

		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief Element 이름을 리턴한다.
 * @returns Element 이름을 리턴한다.
 */
const char * CXmlElement::GetName()
{
	return m_strName.c_str();
}

/**
 * @ingroup XmlParser
 * @brief Element 내용을 리턴한다.
 * @returns Element 내용을 리턴한다.
 */
const char * CXmlElement::GetData()
{
	return m_strData.c_str();
}

/**
 * @ingroup XmlParser
 * @brief Element 내용이 존재하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 * @returns 
 */
bool CXmlElement::IsDataEmpty()
{
	return m_strData.empty();
}

/**
 * @ingroup XmlParser
 * @brief Element 이름을 설정한다.
 * @param pszName Element 이름
 */
void CXmlElement::SetName( const char * pszName )
{
	m_strName = pszName;
}

/**
 * @ingroup XmlParser
 * @brief 하위 Element 를 추가한다.
 * @param pszName 하위 Element 이름
 * @param pszData 하위 Element 의 data 값
 */
void CXmlElement::InsertElementData( const char * pszName, const char * pszData )
{
	CXmlElement clsElement;

	clsElement.m_strName = pszName;

	if( pszData )
	{
		if( strstr( pszData, "<" ) || strstr( pszData, ">" ) )
		{
			clsElement.m_strData = "<![CDATA[";
			clsElement.m_strData.append( pszData );
			clsElement.m_strData.append( "]]>" );
		}
		else
		{
			clsElement.m_strData = pszData;
		}
	}

	m_clsElementList.push_back( clsElement );
}

/**
 * @ingroup XmlParser
 * @brief 하위 Element 를 추가한다.
 * @param pszName 하위 Element 이름
 * @param strData 하위 Element 의 data 값
 */
void CXmlElement::InsertElementData( const char * pszName, std::string & strData )
{
	InsertElementData( pszName, strData.c_str() );
}

/**
 * @ingroup XmlParser
 * @brief 하위 Element 를 추가한다.
 * @param pszName 하위 Element 이름
 * @param iData		하위 Element 의 data 값
 */
void CXmlElement::InsertElementData( const char * pszName, int iData )
{
	char szData[11];

	snprintf( szData, sizeof(szData), "%d", iData );

	InsertElementData( pszName, szData );
}

/**
 * @ingroup XmlParser
 * @brief 하위 Element 를 추가한다.
 * @param pszName 하위 Element 이름
 * @param iData		하위 Element 의 data 값
 */
void CXmlElement::InsertElementData( const char * pszName, int64_t iData )
{
	char szData[21];

	snprintf( szData, sizeof(szData), LONG_LONG_FORMAT, iData );

	InsertElementData( pszName, szData );
}

/**
 * @ingroup XmlParser
 * @brief 하위 Element 를 추가한다.
 * @param pszName 하위 Element 이름
 * @param bData		하위 Element 의 data 값
 */
void CXmlElement::InsertElementData( const char * pszName, bool bData )
{
	InsertElementData( pszName, bData ? "true" : "false" );
}

/**
 * @ingroup XmlParser
 * @brief 하위 Element 를 추가한다.
 * @param pclsElement 하위 Element
 */
void CXmlElement::InsertElement( CXmlElement * pclsElement )
{
	m_clsElementList.push_back( *pclsElement );
}

/**
 * @ingroup XmlParser
 * @brief 애트리뷰트를 추가한다.
 * @param pszName		애트리뷰트 이름
 * @param pszValue	애트리뷰트 값
 */
void CXmlElement::InsertAttribute( const char * pszName, const char * pszValue )
{
	m_clsAttributeMap.insert( XML_ATTRIBUTE_MAP::value_type( pszName, pszValue ) );
}

/**
 * @ingroup XmlParser
 * @brief element list 의 포인터를 리턴한다.
 * @returns element list 의 포인터를 리턴한다.
 */
XML_ELEMENT_LIST * CXmlElement::GetElementList()
{
	return &m_clsElementList;
}

/**
 * @brief 입력된 문자열을 bool 로 변환하여서 리턴한다.
 * @param pszData 문자열
 * @returns 입력된 문자열이 true 이거나 yes 이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CXmlElement::GetBoolean( const char * pszData )
{
	if( !strcasecmp( pszData, "true" ) || !strcasecmp( pszData, "yes" ) )
	{
		return true;
	}

	return false;
}
