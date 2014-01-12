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

#include "StringMap.h"
#include "MemoryDebug.h"

CStringMap::CStringMap()
{
}

CStringMap::~CStringMap()
{
}

/**
 * @ingroup SipPlatform
 * @brief ���ڿ� �� �ڷᱸ���� ���ڿ� Ű, ���� �����Ѵ�.
 * @param pszKey		���ڿ� Ű
 * @param pszValue	���ڿ� ��
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CStringMap::Insert( const char * pszKey, const char * pszValue )
{
	STRING_MAP::iterator	itMap;
	bool	bRes = false;

	if( pszKey == NULL ) return false;
	if( pszValue == NULL ) pszValue = "";

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszKey );
	if( itMap == m_clsMap.end() )
	{
		m_clsMap.insert( STRING_MAP::value_type( pszKey, pszValue ) );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipPlatform
 * @brief ���ڿ� �� �ڷᱸ���� ���ڿ� Ű�� �����ϴ��� �˻��Ѵ�.
 * @param pszKey	���ڿ� Ű
 * @returns ���ڿ� �� �ڷᱸ���� ���ڿ� Ű�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CStringMap::Select( const char * pszKey )
{
	STRING_MAP::iterator	itMap;
	bool	bRes = false;

	if( pszKey == NULL ) return false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszKey );
	if( itMap != m_clsMap.end() )
	{
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipPlatform
 * @brief ���ڿ� �� �ڷᱸ���� ����� ���ڿ� ������ �����Ѵ�.
 * @returns ���ڿ� �� �ڷᱸ���� ����� ���ڿ� ������ �����Ѵ�.
 */
int CStringMap::GetCount( )
{
	int iCount;

	m_clsMutex.acquire();
	iCount = (int)m_clsMap.size();
	m_clsMutex.release();

	return iCount;
}

/**
 * @ingroup SipPlatform
 * @brief �ڷᱸ���� ����� �����͸� ��� �����Ѵ�.
 */
void CStringMap::DeleteAll( )
{
	m_clsMutex.acquire();
	m_clsMap.clear();
	m_clsMutex.release();
}
