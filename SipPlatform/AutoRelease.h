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

#ifndef _AUTO_RELEASE_H_
#define _AUTO_RELEASE_H_

#include <stdio.h>

/**
 * @brief 객체가 삭제될 때에 Release 메소드를 호출하는 클래스
 * @param T_MAP		맵 자료구조 클래스
 * @param T_DATA	맵 자료구조의 데이터 클래스
 */
template< class T_MAP, class T_DATA >
class CAutoRelease
{
	T_MAP		* m_pclsMap;

public:
	T_DATA	* m_pclsData;

	CAutoRelease( T_MAP & clsMap ) : m_pclsData(NULL)
	{
		m_pclsMap = &clsMap;
	}

	~CAutoRelease()
	{
		if( m_pclsData )
		{
			m_pclsMap->Release( m_pclsData );
		}
	}
};

#endif
