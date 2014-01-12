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
#include "SdpBandWidth.h"
#include "MemoryDebug.h"

/**
 * @ingroup SdpParser
 * @brief ������
 */
CSdpBandWidth::CSdpBandWidth()
{
}

/**
 * @ingroup SdpParser
 * @brief �Ҹ���
 */
CSdpBandWidth::~CSdpBandWidth()
{
}

/**
 * @ingroup SdpParser
 * @brief SDP bandwitdh �� value ���ڿ��� �Ľ��Ѵ�.
 * @param pszText		SDP bandwitdh �� value ���ڿ�
 * @param iTextLen	SDP bandwitdh �� value ���ڿ� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1�� �����Ѵ�.
 */
int CSdpBandWidth::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == ':' )
		{
			m_strType.append( pszText, iPos );
			m_strBandWidth.append( pszText + iPos + 1, iTextLen - ( iPos + 1 ) );
			break;
		}
	}

	if( m_strType.empty() )
	{
		m_strType.append( pszText, iTextLen );
	}

	return iTextLen;
}

/**
 * @ingroup SdpParser
 * @brief SDP bandwitdh �� value ���ڿ��� �����Ѵ�.
 * @param pszText		SDP bandwitdh �� value ���ڿ��� ������ ����
 * @param iTextSize pszText ������ ũ��
 * @returns �����ϸ� ����� ���ڿ� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSdpBandWidth::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;
	if( Empty() ) return -1;

	return snprintf( pszText, iTextSize, "%s:%s", m_strType.c_str(), m_strBandWidth.c_str() );
}

/**
 * @ingroup SdpParser
 * @brief ��� ������ �ʱ�ȭ��Ų��.
 */
void CSdpBandWidth::Clear()
{
	m_strType.clear();
	m_strBandWidth.clear();
}

/**
 * @ingroup SdpParser
 * @brief �����Ͱ� �����ϸ� false �� �����ϰ� �׷��� ������ true �� �����Ѵ�.
 * @return �����Ͱ� �����ϸ� false �� �����ϰ� �׷��� ������ true �� �����Ѵ�.
 */
bool CSdpBandWidth::Empty()
{
	if( m_strType.empty() || m_strBandWidth.empty() ) return true;

	return false;
}
