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

#include "StringUtility.h"
#include <string.h>
#include <stdlib.h>
#include "MemoryDebug.h"

/**
 * @ingroup SipPlatform
 * @brief ���ڿ��� ���Ե� ���ڿ��� �����Ѵ�.
 * @param strCallId	���ڿ�
 * @param pszBefore ���� ��� ���ڿ�
 * @param pszAfter	���� ��� ���ڿ��� ������ ���ڿ�
 */
void ReplaceString( std::string & strCallId, const char * pszBefore, const char * pszAfter )
{
	size_t iPos = strCallId.find( pszBefore );
	size_t iBeforeLen = strlen( pszBefore );
	size_t iAfterLen = strlen( pszAfter );

	while( iPos < std::string::npos )
	{
		strCallId.replace( iPos, iBeforeLen, pszAfter );
		iPos = strCallId.find( pszBefore, iPos + iAfterLen );
	}
}

/**
 * @ingroup SipPlatform
 * @brief ���ڿ��� ���Ե� Ű�� ���� �����Ѵ�. 
 *				"app=36;msg=36;hotline=46;presence=36; broadcast=46" ���ڿ����� 
 *				app �� ���� �����ϰ� ������ pszKey �� "app=" �� �Է��ϰ� cSep �� ';' �� �Է��ϸ� �ȴ�.
 * @param strText		���ڿ�
 * @param pszKey		Ű
 * @param cSep			������
 * @param strValue	Ű�� ���� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool SearchValue( std::string & strText, const char * pszKey, char cSep, std::string & strValue )
{
	strValue.clear();

	size_t iPos = strText.find( pszKey );
	if( iPos < std::string::npos )
	{
		size_t iKeyLen = strlen( pszKey );
		size_t iEndPos = strText.find( cSep, iPos + iKeyLen );

		if( iEndPos < std::string::npos )
		{
			strValue = strText.substr( iPos + iKeyLen, iEndPos - ( iPos + iKeyLen ) );
		}
		else
		{
			strValue = strText.substr( iPos + iKeyLen );
		}

		return true;
	}

	return false;
}

/**
 * @ingroup SipPlatform
 * @brief ���ڿ��� ���Ե� Ű�� ���� �����Ѵ�. 
 *				"app=36;msg=36;hotline=46;presence=36; broadcast=46" ���ڿ����� 
 *				app �� ���� �����ϰ� ������ pszKey �� "app=" �� �Է��ϰ� cSep �� ';' �� �Է��ϸ� �ȴ�.
 * @param strText		���ڿ�
 * @param pszKey		Ű
 * @param cSep			������
 * @param iValue		Ű�� ���� ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool SearchValue( std::string & strText, const char * pszKey, char cSep, int & iValue )
{
	std::string	strValue;

	if( SearchValue( strText, pszKey, cSep, strValue ) )
	{
		iValue = atoi( strValue.c_str() );

		return true;
	}

	return false;
}

/**
 * @ingroup SipPlatform
 * @brief ���ڿ��� ���� ������ �����Ѵ�.
 * @param strText ���ڿ�
 */
void LeftTrimString( std::string & strText )
{
	int iIndex;
	int iLen = (int)strText.length();
	for( iIndex = 0; iIndex < iLen; ++iIndex )
	{
		char c = strText.at(iIndex);
		if( c == ' ' || c == '\t' ) continue;

		strText.erase( 0, iIndex );
		break;
	}

	if( iIndex == iLen )
	{
		strText.clear();
	}
}

/**
 * @ingroup SipPlatform
 * @brief ���ڿ��� ������ ������ �����Ѵ�.
 * @param strText ���ڿ�
 */
void RightTrimString( std::string & strText )
{
	int iIndex;
	int iLen = (int)strText.length();
	for( iIndex = iLen - 1; iIndex >= 0; --iIndex )
	{
		char c = strText.at(iIndex);
		if( c == ' ' || c == '\t' ) continue;

		if( iIndex != ( iLen - 1 ) )
		{
			strText.erase( iIndex + 1 );
		}

		break;
	}

	if( iIndex == -1 )
	{
		strText.clear();
	}
}

/**
 * @ingroup SipPlatform
 * @brief ���ڿ��� ����, ������ ������ �����Ѵ�.
 * @param strText ���ڿ�
 */
void TrimString( std::string & strText )
{
	LeftTrimString( strText );
	RightTrimString( strText );
}

/**
 * @ingroup SipPlatform
 * @brief �Է� ���ڿ��� �����ڷ� �и��Ͽ��� ���ڿ� ����Ʈ�� �����Ѵ�.
 * @param pszText �Է� ���ڿ�
 * @param clsList ���ڿ� ����Ʈ
 * @param cSep		������
 */
void SplitString( const char * pszText, STRING_LIST & clsList, char cSep )
{
	int iStartPos = -1, i;

	clsList.clear();

	for( i = 0; pszText[i]; ++i )
	{
		if( pszText[i] == cSep )
		{
			if( iStartPos >= 0 && iStartPos != i )
			{
				std::string strTemp;

				strTemp.append( pszText + iStartPos, i - iStartPos );
				clsList.push_back( strTemp );
			}

			iStartPos = i + 1;
		}
		else if( i == 0 )
		{
			iStartPos = 0;
		}
	}

	if( iStartPos >= 0 && iStartPos != i )
	{
		std::string strTemp;

		strTemp.append( pszText + iStartPos, i - iStartPos );
		clsList.push_back( strTemp );
	}
}

/**
 * @ingroup SipPlatform
 * @brief ���ڿ��� unsigned int �� ��ȯ�Ѵ�.
 * @param pszText ���ڿ�
 * @returns unsigned int �� �����Ѵ�.
 */
uint32_t GetUInt32( const char * pszText )
{
	if( pszText == NULL ) return 0;

	return strtoul( pszText, NULL, 10 );
}

/**
 * @ingroup SipPlatform
 * @brief ���ڿ��� unsigned long long ���� ��ȯ�Ѵ�.
 * @param pszText ���ڿ�
 * @returns unsigned long long �� �����Ѵ�.
 */
uint64_t GetUInt64( const char * pszText )
{
	if( pszText == NULL ) return 0;

#ifdef WIN32
	return _strtoui64( pszText, NULL, 10 );
#else
	return strtoull( pszText, NULL, 10 );
#endif
}
