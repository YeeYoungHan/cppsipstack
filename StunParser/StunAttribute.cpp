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
#include "SipUdp.h"
#include "StunAttribute.h"
#include "StunDecode.h"
#include "StunEncode.h"
#include "StunHeader.h"
#include "StunDefine.h"
#include "MemoryDebug.h"

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

bool CStunAttribute::GetIpPort( std::string & strIp, uint16_t & sPort )
{
	if( m_sType == STUN_AT_XOR_MAPPED_ADDRESS )
	{
		if( m_strValue.length() < 8 ) return false;

		const char * pszValue = m_strValue.c_str();
		
		memcpy( &sPort, pszValue + 2, 2 );
		sPort = ntohs( sPort );

		uint16_t sMod = (((uint16_t)CStunHeader::m_arrCookie[0] << 8 ) & 0xFF00) | ((uint16_t)CStunHeader::m_arrCookie[1] & 0x00FF );
		sPort ^= sMod;

		if( pszValue[1] == STUN_FAMILY_IPV4 )
		{
			uint8_t arrIp[4];
			char szIp[21];

			memcpy( arrIp, pszValue + 4, 4 );

			for( int i = 0; i < 4; ++i )
			{
				arrIp[i] ^= CStunHeader::m_arrCookie[i];
			}

#ifdef WINXP
			snprintf( szIp, sizeof(szIp), "%s", inet_ntoa( *((struct in_addr *)&arrIp)) );
#else
			inet_ntop( AF_INET, &arrIp, szIp, sizeof(szIp) );
#endif

			strIp = szIp;
		}

		return true;
	}

	return false;
}

bool CStunAttribute::SetXorMappedAddress( const char * pszIp, uint16_t sPort )
{
	m_sType = STUN_AT_XOR_MAPPED_ADDRESS;
	m_sLength = 8;

	uint8_t szValue[8];

	szValue[0] = 0;
	szValue[1] = STUN_FAMILY_IPV4;

	uint16_t sMod = ((uint16_t)CStunHeader::m_arrCookie[0] << 8 ) & 0xFF00 | ((uint16_t)CStunHeader::m_arrCookie[1] & 0x00FF );
	sPort ^= sMod;
	sPort = htons( sPort );
	memcpy( szValue + 2, &sPort, 2 );

#ifdef WINXP
	*((uint32_t*)(szValue + 4)) = inet_addr( pszIp );
#else
	inet_pton( AF_INET, pszIp, szValue+4 );
#endif

	for( int i = 0; i < 4; ++i )
	{
		szValue[4+i] ^= CStunHeader::m_arrCookie[i];
	}

	m_strValue.clear();
	m_strValue.append( (char *)szValue, 8 );

	return true;
}
