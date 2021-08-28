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
#include "ServerUtility.h"
#include "Log.h"

#include "Dtls.hpp"

void PrintHex( const char * pszName, const uint8_t * pszData, int iDataLen )
{
	printf( "%s [", pszName );

	for( int i = 0; i < iDataLen; ++i )
	{
		printf( "%02x", pszData[i] );
	}

	printf( "]\n" );
}

bool gbDtlClientRun = false;
int giUdpPort = 2000;

THREAD_API DtlsClient( LPVOID lpParameter )
{
	gbDtlClientRun = true;

	Socket hSocket = UdpListen( giUdpPort, NULL );
	if( hSocket == INVALID_SOCKET )
	{
		printf( "%s UdpListen(%d) error(%d)", __FUNCTION__, giUdpPort, GetError() );
		gbDtlClientRun = false;
		return 0;
	}

	SSL * psttSsl;
	struct	sockaddr_in	addr;

	addr.sin_family = AF_INET;
	addr.sin_port   = htons(giUdpPort+1);

	inet_pton( AF_INET, "127.0.0.1", &addr.sin_addr.s_addr );
	if( connect( hSocket, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR )
	{
		printf( "connect error\n" );
	}
	else if( (psttSsl = SSL_new( gpsttClientCtx ) ) == NULL )
	{
		printf( "ssl_new error\n" );
	}
	else
	{
		SSL_set_fd( psttSsl, (int)hSocket );
		SSL_set_tlsext_use_srtp( psttSsl, "SRTP_AES128_CM_SHA1_80" );

		if( SSL_connect( psttSsl ) == -1 )
		{
			printf( "SSL_connect error\n" );
		}
		else
		{
			uint8_t szMaterial[60];
			uint8_t * pszLocalKey, * pszLocalSalt, * pszRemoteKey, * pszRemoteSalt;

			SSL_export_keying_material( psttSsl, szMaterial, sizeof(szMaterial), "EXTRACTOR-dtls_srtp", 19, NULL, 0, 0 );

			pszLocalKey = szMaterial;
			pszRemoteKey = pszLocalKey + 16;
			pszLocalSalt = pszRemoteKey + 16;
			pszRemoteSalt = pszLocalSalt + 14;

			printf( "%s\n", __FUNCTION__ );
			PrintHex( "LocalKey", pszLocalKey, 16 );
			PrintHex( "RemoteKey", pszRemoteKey, 16 );
			PrintHex( "LocalSalt", pszLocalSalt, 14 );
			PrintHex( "RemoteSalt", pszRemoteSalt, 14 );
		}

		SSL_free( psttSsl );
	}

	closesocket( hSocket );
	gbDtlClientRun = false;

#if OPENSSL_VERSION_NUMBER < 0x10100000L
	ERR_remove_thread_state( NULL );
#endif

	return 0;
}

THREAD_API DtlsServer( LPVOID lpParameter )
{
	Socket hSocket = UdpListen( giUdpPort + 1, NULL );
	if( hSocket == INVALID_SOCKET )
	{
		printf( "%s UdpListen(%d) error(%d)", __FUNCTION__, giUdpPort + 1, GetError() );
		return 0;
	}

	SSL * psttSsl;
	struct	sockaddr_in	addr;

	addr.sin_family = AF_INET;
	addr.sin_port   = htons(giUdpPort);

	inet_pton( AF_INET, "127.0.0.1", &addr.sin_addr.s_addr );
	if( connect( hSocket, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR )
	{
		printf( "connect error\n" );
	}
	else if( (psttSsl = SSL_new( gpsttClientCtx ) ) == NULL )
	{
		printf( "ssl_new error\n" );
	}
	else
	{
		SSL_set_fd( psttSsl, (int)hSocket );
		SSL_set_tlsext_use_srtp( psttSsl, "SRTP_AES128_CM_SHA1_80" );

		if( SSL_accept( psttSsl ) == -1 )
		{
			printf( "SSL_accept error\n" );
		}
		else
		{
			uint8_t szMaterial[60];
			uint8_t * pszLocalKey, * pszLocalSalt, * pszRemoteKey, * pszRemoteSalt;

			SSL_export_keying_material( psttSsl, szMaterial, sizeof(szMaterial), "EXTRACTOR-dtls_srtp", 19, NULL, 0, 0 );

			pszLocalKey = szMaterial;
			pszRemoteKey = pszLocalKey + 16;
			pszLocalSalt = pszRemoteKey + 16;
			pszRemoteSalt = pszLocalSalt + 14;

			printf( "%s\n", __FUNCTION__ );
			PrintHex( "LocalKey", pszLocalKey, 16 );
			PrintHex( "RemoteKey", pszRemoteKey, 16 );
			PrintHex( "LocalSalt", pszLocalSalt, 14 );
			PrintHex( "RemoteSalt", pszRemoteSalt, 14 );
		}

		SSL_free( psttSsl );
	}

	closesocket( hSocket );

#if OPENSSL_VERSION_NUMBER < 0x10100000L
	ERR_remove_thread_state( NULL );
#endif

	return 0;
}

int main( int argc, char * argv[] )
{
#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );
#endif

	InitNetwork();
	InitDtls();

	for( int i = 0; i < 1; ++i )
	{
		giUdpPort += i * 2;

		StartThread( "DtlsClient", DtlsClient, NULL );
		DtlsServer( NULL );

		while( gbDtlClientRun )
		{
			sleep(1);
		}
	}
	
	FinalDtls();
	SSLFinal();
	CLog::Release();

	return 0;
}
