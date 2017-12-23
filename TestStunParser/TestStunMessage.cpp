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

#include "TestStunParser.h"

bool TestStunMessage()
{
	CStunMessage clsMessage;
	std::string strPacket, strOutput;
	char szPacket[1500];
	int iPacketLen;

	memset( szPacket, 0, sizeof(szPacket) );
	const char * pszInput = "000100542112a4428e24a0e78f0a741a2ae42777000600096c4d52623a6c4d526b000000002400046efffaff802a0008000000000044d73a80540001330000008070000400000003000800140f2da18f9c1c439cd217ee85dfd8e15f25bfb283802800041e370fad";

	HexToString( pszInput, strPacket );

	if( clsMessage.Parse( strPacket.c_str(), strPacket.length() ) == -1 ) return false;

	clsMessage.m_strPassword = "FNPRfT4qUaVOKa0ivkn64mMY";
	iPacketLen = clsMessage.ToString( szPacket, sizeof(szPacket) );
	if( iPacketLen == -1 ) return false;

	StringToHex( szPacket, iPacketLen, strOutput );

	if( strcmp( pszInput, strOutput.c_str() ) )
	{
		printf( "encode error\n" );
		printf( "input [%s]\n", pszInput );
		printf( "output[%s]\n", strOutput.c_str() );
	}

	return true;
}
