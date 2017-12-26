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
#include "TestStunParser.h"

/**
 * @ingroup TestStunParser
 * @brief 입력된 STUN 패킷에서 XOR_MAPPED_ADDRESS 애트리뷰트를 찾은 후, XOR_MAPPED_ADDRESS 의 IP/Port 와 입력된 IP/Port 가 일치하는지 검사한다.
 * @param pszInput		HEX steam 으로 저장된 STUN 패킷
 * @param pszInputIp	IP 주소
 * @param sInputPort	Port 번호
 * @returns XOR_MAPPED_ADDRESS 의 IP/Port 와 입력된 IP/Port 가 일치하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
static bool TestStunXorMappedAddress( const char * pszInput, const char * pszInputIp, uint16_t sInputPort )
{
	CStunMessage clsMessage;
	std::string strPacket;

	HexToString( pszInput, strPacket );

	if( clsMessage.Parse( strPacket.c_str(), strPacket.length() ) == -1 )
	{
		printf( "%s clsMessage.Parse(%s) error\n", __FUNCTION__, pszInput );
		return false;
	}

	STUN_ATTRIBUTE_LIST::iterator itAL;

	for( itAL = clsMessage.m_clsAttributeList.begin(); itAL != clsMessage.m_clsAttributeList.end(); ++itAL )
	{
		if( itAL->m_sType == STUN_AT_XOR_MAPPED_ADDRESS )
		{
			std::string strIp;
			uint16_t sPort;

			itAL->GetIpPort( strIp, sPort );

			if( strcmp( strIp.c_str(), pszInputIp ) )
			{
				printf( "%s ip(%s) != input ip(%s) => packet(%s)\n", __FUNCTION__, strIp.c_str(), pszInputIp, pszInput );
				return false;
			}

			if( sPort != sInputPort )
			{
				printf( "%s port(%d) != input port(%d) => packet(%s)\n", __FUNCTION__, sPort, sInputPort, pszInput );
				return false;
			}

			break;
		}
	}

	return true;

}

bool TestStunXorMappedAddress( )
{
	if( TestStunXorMappedAddress( "010100342112a4428e24a0e78f0a741a2ae427770020000800010720e1baa46280700004000000030008001411d451d31710a08026478975871baf48e001ef428028000446cfd3d1"
				, "192.168.0.32", 9778 ) == false ) return false;
	
	if( TestStunXorMappedAddress( "010100342112a442cec325aeb2bb95978bb3615600200008000167dee1baa45d807000040000000300080014494fd190005dbecaa1dc9c169e97cca06a3bbc30802800049befef2b"
				, "192.168.0.31", 18124 ) == false ) return false;

	return true;
}
