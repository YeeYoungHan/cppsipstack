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
#include "XmlElement.h"

/**
 * @ingroup XmlParser
 * @brief ������
 */
CXmlElement::CXmlElement()
{
}

/**
 * @ingroup XmlParser
 * @brief �Ҹ���
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

/**
 * @ingroup XmlParser
 * @brief XML ���ڿ��� �Ľ��Ͽ��� ��� ������ �����Ѵ�.
 * @param pszText		XML ���ڿ�
 * @param iTextLen	XML ���ڿ� ����
 * @returns �����ϸ� �Ľ��� XML ���ڿ��� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
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
		else if( pszText[iPos] == '<' )
		{
			if( iPos + 4 < iTextLen && !strncmp( pszText + iPos + 1, "!--", 3 ) )
			{
				cTypeOld = cType;
				cType = XML_ELEMENT_COMMENT;
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

				iLen = m_strName.length();

				if( iTextLen - ( iPos + 2 ) < (int)( iLen + 1 ) ) return -1;

				if( !strncmp( pszText + iPos + 2, m_strName.c_str(), iLen ) )
				{
					cType = XML_ELEMENT_NAME_END;
					iPos += iLen + 1;
				}
				else
				{
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
			else if( pszText[iPos] == '=' )
			{
				if( pszText[iPos+1] != '"' ) return -1;

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
 * @brief ��� ������ ����� ���� �̿��Ͽ��� XML ���ڿ��� �����Ѵ�.
 * @param pszText			XML ���ڿ��� ������ ����
 * @param iTextSize		XML ���ڿ��� ������ ������ ũ��
 * @returns ������ XML ���ڿ��� ���̸� �����Ѵ�.
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
 * @brief ��� ������ �ʱ�ȭ��Ų��.
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
 * @brief XML ������ �о ��� ������ �����Ѵ�.
 * @param pszFileName		XML ���� �̸�
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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

	if( Parse( strBuf.c_str(), strBuf.length() ) == -1 ) return false;

	return true;
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ѵ�.
 * @param pszName ��Ʈ����Ʈ �̸�
 * @returns �����ϸ� ��Ʈ����Ʈ�� ���� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
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
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ѵ�.
 * @param pszName		��Ʈ����Ʈ �̸�
 * @param strValue	��Ʈ����Ʈ ��
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ͽ� int ������ �����Ѵ�.
 * @param pszName		��Ʈ����Ʈ �̸�
 * @param iValue		��Ʈ����Ʈ ��
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectAttribute( const char * pszName, int & iValue )
{
	XML_ATTRIBUTE_MAP::iterator	itAM;

	iValue = -1;

	itAM = m_clsAttributeMap.find( pszName );
	if( itAM != m_clsAttributeMap.end() )
	{
		iValue = atoi( itAM->second.c_str() );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ��Ʈ����Ʈ�� �ش��ϴ� ���� �˻��Ͽ� bool ������ �����Ѵ�.
 * @param pszName		��Ʈ����Ʈ �̸�
 * @param bValue		��Ʈ����Ʈ ��
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectAttribute( const char * pszName, bool & bValue )
{
	XML_ATTRIBUTE_MAP::iterator	itAM;

	bValue = false;

	itAM = m_clsAttributeMap.find( pszName );
	if( itAM != m_clsAttributeMap.end() )
	{
		if( !strcasecmp( itAM->second.c_str(), "true" ) )
		{
			bValue = true;
		}

		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �˻��Ѵ�.
 * @param pszName		���� Element �̸�
 * @param iIndex		���� Element �ε���. 0 �� �Է��ϸ� ù��° �˻��� ���� Element �� �����ϰ� 1 �� �Է��ϸ� �ι�° �˻��� ���� Element �� �����Ѵ�.
 * @returns �����ϸ� ���� Element ��ü�� �����͸� �����ϰ� �׷��� ������ NULL �� �����Ѵ�.
 */
CXmlElement * CXmlElement::SelectElement( const char * pszName, int iIndex )
{
	XML_ELEMENT_LIST::iterator	itEL;
	int iCount = 0;

	if( iIndex < 0 ) iIndex = 0;

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
 * @brief ���� Element �� �˻��Ͽ��� Element ����Ʈ�� �����Ѵ�.
 * @param pszName		���� Element �̸�
 * @param clsList		���� Element �� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief ���� Element �� �˻��Ͽ��� ������ �����Ѵ�.
 * @param pszName		���� Element �̸�
 * @param strData		���� Elemnet �� ������ ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectElementData( const char * pszName, std::string & strData )
{
	strData.clear();

	CXmlElement * pclsElement = SelectElement( pszName );
	if( pclsElement )
	{
		strData = pclsElement->m_strData;
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief ���� Element �� �˻��Ͽ��� ���� ������ �����´�.
 * @param pszName ���� Element �̸�
 * @param iData		���� Element �� ���� �����ϴ� ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CXmlElement::SelectElementData( const char * pszName, int & iData )
{
	iData = -1;

	CXmlElement * pclsElement = SelectElement( pszName );
	if( pclsElement )
	{
		iData = atoi( pclsElement->m_strData.c_str() );
		return true;
	}

	return false;
}

/**
 * @ingroup XmlParser
 * @brief Element �̸��� �����Ѵ�.
 * @returns Element �̸��� �����Ѵ�.
 */
const char * CXmlElement::GetName()
{
	return m_strName.c_str();
}

/**
 * @ingroup XmlParser
 * @brief Element ������ �����Ѵ�.
 * @returns Element ������ �����Ѵ�.
 */
const char * CXmlElement::GetData()
{
	return m_strData.c_str();
}

bool CXmlElement::IsDataEmpty()
{
	return m_strData.empty();
}
