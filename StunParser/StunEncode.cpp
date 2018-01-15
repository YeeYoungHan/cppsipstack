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

#include "SipUdp.h"
#include "StunEncode.h"

CStunEncode::CStunEncode() : m_pszText(NULL), m_iTextSize(0), m_iPos(0)
{
}

CStunEncode::~CStunEncode()
{
}

bool CStunEncode::SetPacket( char * pszText, int iTextSize )
{
	m_pszText = pszText;
	m_iTextSize = iTextSize;
	m_iPos = 0;

	return true;
}

int CStunEncode::GetPos( )
{
	return m_iPos;
}

bool CStunEncode::Encode( uint16_t sInput )
{
	if( m_iTextSize < ( m_iPos + 2 ) ) return false;

	uint16_t sTemp = htons( sInput );

	memcpy( m_pszText + m_iPos, &sTemp, 2 );
	m_iPos += 2;

	return true;
}

bool CStunEncode::Encode( const char * pszInput, int iInputLen, int iOutputLen )
{
	if( iOutputLen == 0 ) iOutputLen = iInputLen;

	if( m_iTextSize < ( m_iPos + iOutputLen ) ) return false;

	memcpy( m_pszText + m_iPos, pszInput, iInputLen );
	m_iPos += iInputLen;

	int iTemp = iOutputLen - iInputLen;

	for( int i = 0; i < iTemp; ++i )
	{
		m_pszText[m_iPos++] = 0;
	}

	return true;
}

bool CStunEncode::Encode( std::string & strInput, int iOutputLen )
{
	return Encode( strInput.c_str(), strInput.length(), iOutputLen );
}
