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

#include "StunDecode.h"

CStunDecode::CStunDecode() : m_pszText(NULL), m_iTextLen(0), m_iPos(0)
{
}

CStunDecode::~CStunDecode()
{
}

bool CStunDecode::SetPacket( const char * pszText, int iTextLen )
{
	m_pszText = pszText;
	m_iTextLen = iTextLen;
	m_iPos = 0;

	return true;
}

int CStunDecode::GetPos( )
{
	return m_iPos;
}

bool CStunDecode::Decode( uint16_t & sOutput )
{
	if( m_iTextLen < ( m_iPos + 2 ) ) return false;

	memcpy( &sOutput, m_pszText + m_iPos, 2 );
	sOutput = ntohs( sOutput );
	m_iPos += 2;

	return true;
}

bool CStunDecode::Decode( int iLen, std::string & strOutput )
{
	if( m_iTextLen < ( m_iPos + iLen ) ) return false;

	strOutput.clear();
	strOutput.append( m_pszText + m_iPos, iLen );
	m_iPos += iLen;

	return true;
}
