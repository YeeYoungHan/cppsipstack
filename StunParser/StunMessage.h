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

#ifndef _STUN_MESSAGE_H_
#define _STUN_MESSAGE_H_

#include "StunHeader.h"
#include "StunAttribute.h"
#include <list>

typedef std::list< CStunAttribute > STUN_ATTRIBUTE_LIST;

/**
 * @ingroup StunParser
 * @brief STUN 패킷 저장 클래스
 *				본 클래스에는 STUN 헤더와 STUN 애트리뷰트 리스트가 포함되어 있다.
 */
class CStunMessage
{
public:
	CStunMessage();
	~CStunMessage();

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	void Clear();

	CStunHeader					m_clsHeader;
	STUN_ATTRIBUTE_LIST m_clsAttributeList;
	std::string					m_strPassword;

private:
	static bool			m_bInit;
	static uint32_t m_arrCrc32[256];
};

#endif
