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
#include "WebSocketPacket.h"
#include "MemoryDebug.h"

CWebSocketPacket::CWebSocketPacket()
{
}

CWebSocketPacket::~CWebSocketPacket()
{
}

/**
 * @ingroup HttpStack
 * @brief WebSocket 패킷을 저장한다.
 * @param pszPacket		패킷
 * @param iPacketLen	패킷 길이
 * @returns true 를 리턴한다.
 */
bool CWebSocketPacket::AddPacket( const char * pszPacket, int iPacketLen )
{
	m_strBuf.append( pszPacket, iPacketLen );

	return true;
}

/**
 * @ingroup HttpStack
 * @brief WebSocket 수신 데이터를 가져온다.
 * @param clsHeader WebSocket 헤더 정보 저장 객체
 * @param strData		WebSocket 수신 데이터
 * @returns WebSocket 수신 데이터가 존재하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CWebSocketPacket::GetData( CWebSocketPacketHeader & clsHeader, std::string & strData )
{
	strData.clear();

	int iPacketLen = (int)m_strBuf.length();
	if( iPacketLen < 2 )
	{
		return false;
	}

	const char * pszPacket = m_strBuf.c_str();
	bool bMask = false;
	
	if( (uint8_t)pszPacket[1] & 0x80 ) bMask = true;

	clsHeader.m_iOpCode = (uint8_t)pszPacket[0] & 0x0F;

	int iPayLoadLen = (uint8_t)pszPacket[1] & 0x7F;
	int iWantLen = 0;
	int iPayLoadPos = 0;

	if( iPayLoadLen == 126 )
	{
		if( iPacketLen < 4 )
		{
			return false;
		}

		uint16_t sPayLoadLen;

		memcpy( &sPayLoadLen, pszPacket + 2, 2 );
		iPayLoadLen = ntohs( sPayLoadLen );
		iWantLen = 2 + 2 + iPayLoadLen;
		iPayLoadPos = 2 + 2;
	}
	else if( iPayLoadLen == 127 )
	{
		if( iPacketLen < 10 )
		{
			return false;
		}

		uint64_t lPayLoadLen;

		memcpy( &lPayLoadLen, pszPacket + 2, 8 );
		iPayLoadLen = ntohll( lPayLoadLen );
		iWantLen = 2 + 8 + iPayLoadLen;
		iPayLoadPos = 2 + 8;
	}
	else
	{
		iWantLen = 2 + iPayLoadLen;
		iPayLoadPos = 2;
	}

	if( bMask )
	{
		iWantLen += 4;
	}

	if( iPacketLen < iWantLen )
	{
		return false;
	}

	if( bMask )
	{
		int iMaskPos = iPayLoadPos;
		iPayLoadPos += 4;

		char * pszData = (char *)malloc(iPayLoadLen);
		if( pszData == NULL )
		{
			return false;
		}

		for( int i = 0; i < iPayLoadLen; ++i )
		{
			pszData[i] = pszPacket[iPayLoadPos+i] ^ pszPacket[iMaskPos+i%4];
		}

		strData.append( pszData, iPayLoadLen );

		free( pszData );
	}
	else
	{
		strData.append( pszPacket + iPayLoadPos, iPayLoadLen );
	}

	m_strBuf.erase( 0, iWantLen );

	return true;
}
