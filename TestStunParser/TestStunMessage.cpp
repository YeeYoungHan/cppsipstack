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

/**
 * @ingroup TestStunParser
 * @brief STUN 패킷을 파싱하고 다시 생성하여서 문제가 발생하는지 검사한다.
 * @param pszInput	HEX 문자열로 저장된 STUN 패킷
 * @param pszPw			STUN 수신자에서 전송한 ice-pwd
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
static bool TestStunMessage( const char * pszInput, const char * pszPw )
{
	CStunMessage clsMessage;
	std::string strPacket, strOutput;
	char szPacket[1500];
	int iPacketLen;

	memset( szPacket, 0, sizeof(szPacket) );

	HexToString( pszInput, strPacket );

	if( clsMessage.Parse( strPacket.c_str(), strPacket.length() ) == -1 ) return false;

	clsMessage.m_strPassword = pszPw;
	iPacketLen = clsMessage.ToString( szPacket, sizeof(szPacket) );
	if( iPacketLen == -1 ) return false;

	StringToHex( szPacket, iPacketLen, strOutput );

	if( strcmp( pszInput, strOutput.c_str() ) )
	{
		printf( "encode error\n" );
		printf( "input [%s]\n", pszInput );
		printf( "output[%s]\n", strOutput.c_str() );
		return false;
	}

	return true;
}

bool TestStunMessage()
{
	// A -> B 로 WebRTC 통화 요청을 전송하였을 때에 SDP 를 (A), (B) 라고 표시한다면
	// A -> B 로 전송한 STUN 요청 패킷과 B -> A 로 전송한 STUN 응답 패킷의 MESSAGE-INTEGRITY 계산을 위해서 (B) 의 ice-pwd 를 사용한다.
	// B -> A 로 전송한 STUN 요청 패킷과 A -> B 로 전송한 STUN 응답 패킷의 MESSAGE-INTEGRITY 계산을 위해서 (A) 의 ice-pwd 를 사용한다.

	// Binding Request User ( A -> B )
	if( TestStunMessage( "000100542112a4428e24a0e78f0a741a2ae42777000600096c4d52623a6c4d526b000000002400046efffaff802a0008000000000044d73a80540001330000008070000400000003000800140f2da18f9c1c439cd217ee85dfd8e15f25bfb283802800041e370fad"
		, "FNPRfT4qUaVOKa0ivkn64mMY" ) == false ) return false;

	// Binding Success Response ( B -> A )
	if( TestStunMessage( "010100342112a4428e24a0e78f0a741a2ae427770020000800010720e1baa46280700004000000030008001411d451d31710a08026478975871baf48e001ef428028000446cfd3d1"
		, "FNPRfT4qUaVOKa0ivkn64mMY" ) == false ) return false;

	// Binding Request User ( B -> A )
	if( TestStunMessage( "000100542112a442cec325aeb2bb95978bb36156000600096c4d526b3a6c4d5262000000002400046efffeff802900080000000038b4c8538054000131000000807000040000000300080014d7f156b39b8a01a3302cd4ae8bb1a874d3548d8880280004240f309b"
		, "nFNRfT4UabEOKa00ivn64MtQ" ) == false ) return false;

	// Binding Success Response ( A -> B )
	if( TestStunMessage( "010100342112a442cec325aeb2bb95978bb3615600200008000167dee1baa45d807000040000000300080014494fd190005dbecaa1dc9c169e97cca06a3bbc30802800049befef2b"
		, "nFNRfT4UabEOKa00ivn64MtQ" ) == false ) return false;

	return true;
}
