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
#include "SdpOrigin.h"
#include "MemoryDebug.h"

/**
 * @ingroup SdpParser
 * @brief ������
 */
CSdpOrigin::CSdpOrigin()
{
}

/**
 * @ingroup SdpParser
 * @brief �Ҹ���
 */
CSdpOrigin::~CSdpOrigin()
{
}

/**
 * @ingroup SdpParser
 * @brief SDP origin �� value ���ڿ��� �Ľ��Ѵ�.
 * @param pszText		SDP origin �� value ���ڿ�
 * @param iTextLen	SDP origin �� value ���ڿ� ����
 * @returns �����ϸ� �Ľ��� ���̸� �����ϰ� �׷��� ������ -1�� �����Ѵ�.
 */
int CSdpOrigin::Parse( const char * pszText, int iTextLen )
{
	Clear();
	if( pszText == NULL || iTextLen <= 0 ) return -1;

	int		iPos, iStartPos = 0, iType = 0;

	for( iPos = 0; iPos < iTextLen; ++iPos )
	{
		if( pszText[iPos] == ' ' )
		{
			SetData( pszText + iStartPos, iPos - iStartPos, iType );
			++iType;
			iStartPos = iPos + 1;
		}
	}

	if( iStartPos < iTextLen )
	{
		SetData( pszText + iStartPos, iPos - iStartPos, iType );
		++iType;
	}

	if( iType != 6 ) return -1;

	return iTextLen;
}

/**
 * @ingroup SdpParser
 * @brief SDP origin �� value ���ڿ��� �����Ѵ�.
 * @param pszText		SDP origin �� value ���ڿ��� ������ ����
 * @param iTextSize pszText ������ ũ��
 * @returns �����ϸ� ����� ���ڿ� ���̸� �����ϰ� �׷��� ������ -1 �� �����Ѵ�.
 */
int CSdpOrigin::ToString( char * pszText, int iTextSize )
{
	if( pszText == NULL || iTextSize <= 0 ) return -1;
	if( Empty() ) return -1;

	int iLen;

	iLen = snprintf( pszText, iTextSize, "%s %s %s %s %s %s", m_strUserName.c_str(), m_strSessId.c_str(), m_strSessVersion.c_str()
		, m_strNetType.c_str(), m_strAddrType.c_str(), m_strUnicastAddress.c_str() );

	return iLen;
}

/**
 * @ingroup SdpParser
 * @brief ��� ������ �ʱ�ȭ��Ų��.
 */
void CSdpOrigin::Clear()
{
	m_strUserName.clear();
	m_strSessId.clear();
	m_strSessVersion.clear();
	m_strNetType.clear();
	m_strAddrType.clear();
	m_strUnicastAddress.clear();
}

/**
 * @ingroup SdpParser
 * @brief �����Ͱ� �����ϸ� false �� �����ϰ� �׷��� ������ true �� �����Ѵ�.
 * @return �����Ͱ� �����ϸ� false �� �����ϰ� �׷��� ������ true �� �����Ѵ�.
 */
bool CSdpOrigin::Empty()
{
	if( m_strUserName.empty() || m_strSessId.empty() || m_strSessVersion.empty() ) return true;
	if( m_strNetType.empty() || m_strAddrType.empty() || m_strUnicastAddress.empty() ) return true;

	return false;
}

void CSdpOrigin::SetData( const char * pszData, int iLen, int iType )
{
	switch( iType )
	{
	case 0:
		m_strUserName.append( pszData, iLen );
		break;
	case 1:
		m_strSessId.append( pszData, iLen );
		break;
	case 2:
		m_strSessVersion.append( pszData, iLen );
		break;
	case 3:
		m_strNetType.append( pszData, iLen );
		break;
	case 4:
		m_strAddrType.append( pszData, iLen );
		break;
	case 5:
		m_strUnicastAddress.append( pszData, iLen );
		break;
	}
}
