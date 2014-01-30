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
#include "SdpAttribute.h"
#include <stdlib.h>
#include "MemoryDebug.h"

/**
 * @ingroup SdpParser
 * @brief ������
 */
CSdpAttribute::CSdpAttribute()
{
}

/**
 * @ingroup SdpParser
 * @brief ������
 * @param pszName		��Ʈ����Ʈ �̸�
 * @param pszValue	��Ʈ����Ʈ ��
 */
CSdpAttribute::CSdpAttribute( const char * pszName, const char * pszValue )
{
	if( pszName )
	{
		m_strName = pszName;
	}

	if( pszValue )
	{
		m_strValue = pszValue;
	}
}

/**
 * @ingroup SdpParser
 * @brief �Ҹ���
 */
CSdpAttribute::~CSdpAttribute()
{
}

/**
 * @ingroup SdpParser
 * @brief SDP attribute �� value ���ڿ��� �Ľ��Ѵ�.
 * @param pszText		SDP attribute �� value ���ڿ�
 * @param iTextLen	SDP attribute �� value ���ڿ� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1�� �����Ѵ�.
 */
int CSdpAttribute::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == ':' )
		{
			m_strName.append( pszText, iPos );
			m_strValue.append( pszText + iPos + 1, iTextLen - ( iPos + 1 ) );
			break;
		}
	}

	if( m_strName.empty() )
	{
		m_strName.append( pszText, iTextLen );
	}

	return iTextLen;
}

/**
 * @ingroup SdpParser
 * @brief SDP attribute �� value ���ڿ��� �����Ѵ�.
 * @param pszText		SDP attribute �� value ���ڿ��� ������ ����
 * @param iTextSize pszText ������ ũ��
 * @returns �����ϸ� ����� ���ڿ� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSdpAttribute::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;
	if( Empty() ) return -1;

	if( m_strValue.empty() )
	{
		return snprintf( pszText, iTextSize, "%s", m_strName.c_str() );
	}
	else
	{
		return snprintf( pszText, iTextSize, "%s:%s", m_strName.c_str(), m_strValue.c_str() );
	}
}

/**
 * @ingroup SdpParser
 * @brief ��� ������ �ʱ�ȭ��Ų��.
 */
void CSdpAttribute::Clear()
{
	m_strName.clear();
	m_strValue.clear();
}

/**
 * @ingroup SdpParser
 * @brief �����Ͱ� �����ϸ� false �� �����ϰ� �׷��� ������ true �� �����Ѵ�.
 */
bool CSdpAttribute::Empty()
{
	if( m_strName.empty() ) return true;

	return false;
}

/**
 * @ingroup SdpParser
 * @brief payload type �� �����Ѵ�.
 * @returns payload type �� �����Ѵ�.
 */
int CSdpAttribute::GetPayLoadType()
{
	if( strcmp( m_strName.c_str(), "rtpmap" ) && strcmp( m_strName.c_str(), "fmtp" ) ) return -1;

	int iLen = (int)m_strValue.length();
	const char * pszValue = m_strValue.c_str();

	for( int i = 0; i < iLen; ++i )
	{
		if( pszValue[i] == ' ' )
		{
			if( i > 0 )
			{
				std::string strValue;

				strValue.append( pszValue, i );

				return atoi( strValue.c_str() );
			}

			break;
		}
	}

	return -1;
}

/**
 * @ingroup SdpParser
 * @brief encoding name �� �����´�.
 * @param strName encoding name �� ������ ����
 * @returns ���������� encoding name �� �������� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSdpAttribute::GetEncodingName( std::string & strName )
{
	strName.clear();

	if( strcmp( m_strName.c_str(), "rtpmap" ) ) return false;

	int iLen = (int)m_strValue.length();
	const char * pszValue = m_strValue.c_str();
	int iStartPos = -1;

	for( int i = 0; i < iLen; ++i )
	{
		if( pszValue[i] == ' ' )
		{
			iStartPos = i + 1;
		}
		else if( pszValue[i] == '/' )
		{
			strName.append( pszValue + iStartPos, i - iStartPos );
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SdpParser
 * @brief SDP attribute �� value �� �Ľ��Ͽ��� clsParamList ��ü�� �����Ѵ�.
 * @param clsParamList parameter list ���� ��ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CSdpAttribute::GetParameterList( CSipParameterList & clsParamList )
{
	clsParamList.ClearParam();

	if( strcmp( m_strName.c_str(), "fmtp" ) ) return false;

	int iLen = (int)m_strValue.length();
	const char * pszValue = m_strValue.c_str();
	int iStartPos = -1;

	for( int i = 0; i < iLen; ++i )
	{
		if( pszValue[i] == ' ' )
		{
			iStartPos = i + 1;
			break;
		}
	}

	if( clsParamList.HeaderListParamParse( pszValue + iStartPos, iLen - iStartPos ) > 0 )
	{
		return true;
	}

	return false;
}
