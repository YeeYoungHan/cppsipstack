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

#include "StunHeader.h"
#include "StunDecode.h"
#include "StunEncode.h"

uint8_t CStunHeader::m_arrCookie[4] = { 0x21, 0x12, 0xa4, 0x42 };

CStunHeader::CStunHeader() : m_sMessageType(0), m_sMessageLength(0)
{
}

CStunHeader::~CStunHeader()
{
}

int CStunHeader::Parse( const char * pszText, int iTextLen )
{
	CStunDecode clsDecode;
	std::string strTemp;

	if( clsDecode.SetPacket( pszText, iTextLen ) == false ) return -1;
	if( clsDecode.Decode( m_sMessageType ) == false ) return -1;
	if( clsDecode.Decode( m_sMessageLength ) == false ) return -1;
	if( clsDecode.Decode( 4, strTemp ) == false ) return -1;
	if( clsDecode.Decode( 12, m_strTransactionId ) == false ) return -1;

	return clsDecode.GetPos();
}

int CStunHeader::ToString( char * pszText, int iTextSize )
{
	CStunEncode clsEncode;

	if( clsEncode.SetPacket( pszText, iTextSize ) == false ) return -1;
	if( clsEncode.Encode( m_sMessageType ) == false ) return -1;
	if( clsEncode.Encode( m_sMessageLength ) == false ) return -1;
	if( clsEncode.Encode( (char *)m_arrCookie, 4 ) == false ) return -1;
	if( clsEncode.Encode( m_strTransactionId, 12 ) == false ) return -1;

	return clsEncode.GetPos();
}

void CStunHeader::Clear()
{
	m_sMessageType = 0;
	m_sMessageLength = 0;
	m_strTransactionId.clear();
}
