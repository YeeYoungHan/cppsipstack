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

#include "StunMessage.h"
#include "StunDefine.h"
#include <openssl/hmac.h>
#include "MemoryDebug.h"

#define CRC_POLYNOMIAL				0xEDB88320
#define STUN_FINGERPRINT_XOR	0x5354554E

bool CStunMessage::m_bInit = false;
uint32_t CStunMessage::m_arrCrc32[256];

CStunMessage::CStunMessage()
{
	if( m_bInit == false )
	{
		OpenSSL_add_all_digests();

		for( int i = 0; i < 256; ++i )
		{ 
			uint32_t iValue = (uint32_t)i;

			for( int j = 0; j < 8; ++j )
			{ 
				if( iValue & 1 )
				{ 
					iValue = CRC_POLYNOMIAL ^ (iValue >> 1); 
				}
				else
				{ 
					iValue >>= 1; 
				} 
			}

			m_arrCrc32[i] = iValue; 
		}

		m_bInit = true;
	}
}

CStunMessage::~CStunMessage()
{
}

int CStunMessage::Parse( const char * pszText, int iTextLen )
{
	int iPos = 0, n;

	n = m_clsHeader.Parse( pszText, iTextLen );
	if( n <= 0 ) return -1;
	iPos += n;

	if( m_clsHeader.m_sMessageLength > ( iTextLen - iPos ) ) return -1;
	iTextLen = iPos + m_clsHeader.m_sMessageLength;

	while( iPos < iTextLen )
	{
		CStunAttribute clsAttribute;

		n = clsAttribute.Parse( pszText + iPos, iTextLen - iPos );
		if( n <= 0 ) return -1;
		iPos += n;

		m_clsAttributeList.push_back( clsAttribute );
	}

	return iPos;
}

int CStunMessage::ToString( char * pszText, int iTextSize )
{
	int iLen = 0, n;
	STUN_ATTRIBUTE_LIST::iterator itSAL, itUN;

	iLen += STUN_HEADER_SIZE;

	for( itSAL = m_clsAttributeList.begin(); itSAL != m_clsAttributeList.end(); ++itSAL )
	{
		if( itSAL->m_sType == STUN_AT_USERNAME )
		{
			itUN = itSAL;
		}
		else if( itSAL->m_sType == STUN_AT_MESSAGE_INTEGRITY )
		{
			m_clsHeader.m_sMessageLength = iLen - STUN_HEADER_SIZE + 24;
			m_clsHeader.ToString( pszText, iTextSize );

			HMAC_CTX		sttCtx;
			uint8_t			szDigest[EVP_MAX_MD_SIZE];
			uint32_t		iDigestLen;

			memset( szDigest, 0, sizeof(szDigest) );

			HMAC_CTX_init( &sttCtx );
			HMAC_Init_ex( &sttCtx, m_strPassword.c_str(), m_strPassword.length(), EVP_sha1(), NULL );
			HMAC_Update( &sttCtx, (unsigned char *)pszText, iLen );
			HMAC_Final( &sttCtx, szDigest, &iDigestLen );

			itSAL->m_strValue.clear();
			itSAL->m_strValue.append( (char *)szDigest, iDigestLen );
		}
		else if( itSAL->m_sType == STUN_AT_FINGER_PRINT )
		{
			m_clsHeader.m_sMessageLength = iLen - STUN_HEADER_SIZE + 8;
			m_clsHeader.ToString( pszText, iTextSize );

			uint32_t iCRC = 0xFFFFFFFF;

			for( int i = 0; i < iLen; ++i )
			{ 
				iCRC = ( iCRC >> 8 ) ^ m_arrCrc32[(iCRC ^ (uint8_t)pszText[i]) & 0xFF]; 
			}

			iCRC = iCRC ^ 0xFFFFFFFF;
			iCRC = iCRC ^ STUN_FINGERPRINT_XOR;
			iCRC = htonl( iCRC );
			
			itSAL->m_sLength = 4;
			itSAL->m_strValue.clear();
			itSAL->m_strValue.append( (char *)&iCRC, 4 );
		}

		n = itSAL->ToString( pszText + iLen, iTextSize - iLen );
		if( n <= 0 ) return -1;
		iLen += n;
	}

	m_clsHeader.m_sMessageLength = iLen - STUN_HEADER_SIZE;
	n = m_clsHeader.ToString( pszText, iTextSize );
	if( n <= 0 ) return -1;

	return iLen;
}

void CStunMessage::Clear()
{
	m_clsAttributeList.clear();
}
