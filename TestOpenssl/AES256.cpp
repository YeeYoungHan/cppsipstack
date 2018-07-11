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

#include "TestOpenssl.h"

/**
 * @ingroup TestOpenssl
 * @brief AES256 암호화 테스트
 */
void AES256( )
{
	const char * pszKey = "12345678901234567890123456789012";
	const char * pszIv = "1234567890123456";
	const char * pszInput = "ABCD";
	char szOutput[255];
	int iOutputLen, iLen;

	OpenSSL_add_all_ciphers();

	const EVP_CIPHER * psttCipher = EVP_get_cipherbyname( "aes256" );
	EVP_CIPHER_CTX sttCtx;

	memset( szOutput, 0, sizeof(szOutput) );

	EVP_CIPHER_CTX_init( &sttCtx );
	EVP_EncryptInit( &sttCtx, psttCipher, (const unsigned char *)pszKey, (const unsigned char *)pszIv );

	EVP_EncryptUpdate( &sttCtx, (uint8_t *)szOutput, &iOutputLen, (uint8_t *)pszInput, strlen(pszInput) );
	EVP_EncryptFinal( &sttCtx, (uint8_t *)szOutput + iOutputLen, &iLen );
	EVP_CIPHER_CTX_cleanup( &sttCtx );

	iOutputLen += iLen;

	for( int i = 0; i < iOutputLen; ++i )
	{
		printf( "%02X", (uint8_t)szOutput[i] );
	}

	printf( "\n" );

	EVP_cleanup();
}
