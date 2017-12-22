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
#include "SipMd5.h"
#include <openssl/hmac.h>

static bool TestIntegrity( const char * pszPassWord )
{
	const char * pszPacket = "000100342112a4428e24a0e78f0a741a2ae42777000600096c4d52623a6c4d526b000000002400046efffaff802a0008000000000044d73a80540001330000008070000400000003";
	const char * pszMI = "0f2da18f9c1c439cd217ee85dfd8e15f25bfb283";
	
	std::string strPacket, strDigest;

	HexToString( pszPacket, strPacket );
	
	HMAC_CTX		sttCtx;
	uint8_t			szDigest[EVP_MAX_MD_SIZE];
	uint32_t		iDigestLen;

	memset( szDigest, 0, sizeof(szDigest) );

	char szMd5[16];

	SipMd5Byte( pszPassWord, (unsigned char *)szMd5 );

	HMAC_CTX_init( &sttCtx );
	HMAC_Init_ex( &sttCtx, szMd5, 16, EVP_sha1(), NULL );
	HMAC_Update( &sttCtx, (unsigned char *)strPacket.c_str(), strPacket.length() );
	HMAC_Final( &sttCtx, szDigest, &iDigestLen );

	/*
	//const char * pszPw = "nFNRfT4UabEOKa00ivn64MtQ";
	const char * pszPw = "FNPRfT4qUaVOKa0ivkn64mMY";

	SipMd5Byte( szPassWord, (unsigned char *)szMd5 );

	HMAC_CTX_init( &sttCtx );
	HMAC_Init_ex( &sttCtx, pszPw, strlen(pszPw), EVP_sha1(), NULL );
	HMAC_Update( &sttCtx, (unsigned char *)strPacket.c_str(), strPacket.length() );
	HMAC_Final( &sttCtx, szDigest, &iDigestLen );
	*/

	StringToHex( (char *)szDigest, iDigestLen, strDigest );

	if( !strcmp( strDigest.c_str(), pszMI ) )
	{
		printf( "success\n" );
		return true;
	}

	return false;
}

static bool TestIntegrity2( const char * pszPassWord )
{
	const char * pszPacket = "000100342112a4428e24a0e78f0a741a2ae42777000600096c4d52623a6c4d526b000000002400046efffaff802a0008000000000044d73a80540001330000008070000400000003";
	const char * pszMI = "0f2da18f9c1c439cd217ee85dfd8e15f25bfb283";
	
	std::string strPacket, strDigest;

	HexToString( pszPacket, strPacket );
	
	HMAC_CTX		sttCtx;
	uint8_t			szDigest[EVP_MAX_MD_SIZE];
	uint32_t		iDigestLen;

	memset( szDigest, 0, sizeof(szDigest) );

	HMAC_CTX_init( &sttCtx );
	HMAC_Init_ex( &sttCtx, pszPassWord, strlen(pszPassWord), EVP_sha1(), NULL );
	HMAC_Update( &sttCtx, (unsigned char *)strPacket.c_str(), strPacket.length() );
	HMAC_Final( &sttCtx, szDigest, &iDigestLen );

	StringToHex( (char *)szDigest, iDigestLen, strDigest );

	if( !strcmp( strDigest.c_str(), pszMI ) )
	{
		printf( "success\n" );
		return true;
	}

	return false;
}

bool TestStunMessageIntegrity( )
{
	char szPassWord[1024];

	snprintf( szPassWord, sizeof(szPassWord), "%s::%s", "lMRk", "nFNRfT4UabEOKa00ivn64MtQ" );
	TestIntegrity( szPassWord );

	snprintf( szPassWord, sizeof(szPassWord), "%s:%s:%s", "lMRk", "lMRb", "nFNRfT4UabEOKa00ivn64MtQ" );
	TestIntegrity( szPassWord );

	snprintf( szPassWord, sizeof(szPassWord), "%s:%s:%s", "lMRb", "lMRk", "nFNRfT4UabEOKa00ivn64MtQ" );
	TestIntegrity( szPassWord );

	snprintf( szPassWord, sizeof(szPassWord), "%s::%s", "lMRb", "nFNRfT4UabEOKa00ivn64MtQ" );
	TestIntegrity( szPassWord );

	snprintf( szPassWord, sizeof(szPassWord), "%s::%s", "lMRk:lMRb", "nFNRfT4UabEOKa00ivn64MtQ" );
	TestIntegrity( szPassWord );

	snprintf( szPassWord, sizeof(szPassWord), "%s::%s", "lMRb:lMRk", "nFNRfT4UabEOKa00ivn64MtQ" );
	TestIntegrity( szPassWord );

	snprintf( szPassWord, sizeof(szPassWord), "%s::%s", "lMRb", "FNPRfT4qUaVOKa0ivkn64mMY" );
	TestIntegrity( szPassWord );

	snprintf( szPassWord, sizeof(szPassWord), "%s:%s:%s", "lMRk", "lMRb", "FNPRfT4qUaVOKa0ivkn64mMY" );
	TestIntegrity( szPassWord );

	snprintf( szPassWord, sizeof(szPassWord), "%s:%s:%s", "lMRb", "lMRk", "FNPRfT4qUaVOKa0ivkn64mMY" );
	TestIntegrity( szPassWord );

	snprintf( szPassWord, sizeof(szPassWord), "%s::%s", "lMRb", "FNPRfT4qUaVOKa0ivkn64mMY" );
	TestIntegrity( szPassWord );

	snprintf( szPassWord, sizeof(szPassWord), "%s::%s", "lMRk:lMRb", "FNPRfT4qUaVOKa0ivkn64mMY" );
	TestIntegrity( szPassWord );

	snprintf( szPassWord, sizeof(szPassWord), "%s::%s", "lMRb:lMRk", "FNPRfT4qUaVOKa0ivkn64mMY" );
	TestIntegrity( szPassWord );

	TestIntegrity2( "nFNRfT4UabEOKa00ivn64MtQ" );
	TestIntegrity2( "FNPRfT4qUaVOKa0ivkn64mMY" );

	// https://gist.github.com/roxlu/6ae94d42591e2ae563d9
	// 

	return true;
}
