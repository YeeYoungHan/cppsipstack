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

bool TestStunXorMappedAddress( )
{
	const char * pszInput = "010100342112a4428e24a0e78f0a741a2ae427770020000800010720e1baa46280700004000000030008001411d451d31710a08026478975871baf48e001ef428028000446cfd3d1";
	
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

			if( strcmp( strIp.c_str(), "192.168.0.32" ) )
			{
				printf( "%s ip error\n", __FUNCTION__ );
				return false;
			}

			if( sPort != 9778 )
			{
				printf( "%s port error\n", __FUNCTION__ );
				return false;
			}

			break;
		}
	}

	return true;
}
