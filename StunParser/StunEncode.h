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

#ifndef _STUN_ENCODE_H_
#define _STUN_ENCODE_H_

#include "SipPlatformDefine.h"
#include <string>

/**
 * @ingroup StunParser
 * @brief STUN 패킷 인코딩 클래스
 *				숫자, 문자열 등을 STUN 패킷에 저장한다.
 */
class CStunEncode
{
public:
	CStunEncode();
	~CStunEncode();

	bool SetPacket( char * pszText, int iTextSize );
	int GetPos( );

	bool Encode( uint16_t sInput );
	bool Encode( const char * pszInput, int iInputLen, int iOutputLen = 0 );
	bool Encode( std::string & strInput, int iOutputLen = 0 );

private:
	char * m_pszText;
	int		m_iTextSize;
	int		m_iPos;
};

#endif
