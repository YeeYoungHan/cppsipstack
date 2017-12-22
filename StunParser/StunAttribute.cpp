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

#include "StunAttribute.h"
#include "StunDecode.h"
#include "StunEncode.h"

CStunAttribute::CStunAttribute() : m_sType(0), m_sLength(0)
{
}

CStunAttribute::~CStunAttribute()
{
}

int CStunAttribute::Parse( const char * pszText, int iTextLen )
{
	CStunDecode clsDecode;

	if( clsDecode.SetPacket( pszText, iTextLen ) == false ) return -1;
	if( clsDecode.Decode( m_sType ) == false ) return -1;
	if( clsDecode.Decode( m_sLength ) == false ) return -1;
	if( clsDecode.Decode( m_sLength, m_strValue ) == false ) return -1;

	uint16_t sTemp = m_sLength % 4;
	if( sTemp )
	{
		std::string strTemp;

		sTemp = 4 - sTemp;
		if( clsDecode.Decode( sTemp, strTemp ) == false ) return -1;
	}

	return clsDecode.GetPos();
}

int CStunAttribute::ToString( char * pszText, int iTextSize )
{
	CStunEncode clsEncode;
	int iValueLen;

	m_sLength = m_strValue.length();
	uint16_t sTemp = m_sLength % 4;
	if( sTemp )
	{
		iValueLen = m_sLength + 4 - sTemp;
	}
	else
	{
		iValueLen = m_sLength;
	}

	if( clsEncode.SetPacket( pszText, iTextSize ) == false ) return -1;
	if( clsEncode.Encode( m_sType ) == false ) return -1;
	if( clsEncode.Encode( m_sLength ) == false ) return -1;
	if( clsEncode.Encode( m_strValue, iValueLen ) == false ) return -1;

	return clsEncode.GetPos();
}

void CStunAttribute::Clear()
{
	m_sType = 0;
	m_sLength = 0;
	m_strValue.clear();
}
