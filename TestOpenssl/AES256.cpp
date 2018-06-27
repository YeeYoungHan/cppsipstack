#include "TestOpenssl.h"

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
