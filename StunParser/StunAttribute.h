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

#ifndef _STUN_ATTRIBUTE_H_
#define _STUN_ATTRIBUTE_H_

#include "SipPlatformDefine.h"
#include <string>

/**
 * @ingroup StunParser
 * @brief STUN 애트리뷰트 저장 클래스
 */
class CStunAttribute
{
public:
	CStunAttribute();
	~CStunAttribute();

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	void Clear();

	bool GetIpPort( std::string & strIp, uint16_t & sPort );

	uint16_t		m_sType;
	uint16_t		m_sLength;
	std::string	m_strValue;
};

#endif
