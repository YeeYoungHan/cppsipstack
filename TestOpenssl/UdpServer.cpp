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
#include "FileUtility.h"
#include "Log.h"
#include "openssl/rand.h"

Socket UdpConnect( const char * pszIp, int iPort )
{
	char		szIp[INET6_ADDRSTRLEN];
	Socket	fd;

	memset( szIp, 0, sizeof(szIp) );
	if( isdigit(pszIp[0]) == 0 )
	{
		// if first character is not digit, suppose it is domain main.
		GetIpByName( pszIp, szIp, sizeof(szIp) );
	}
	else
	{
		snprintf( szIp, sizeof(szIp), "%s", pszIp );
	}

	struct	sockaddr_in	addr;		

	// connect server.
	if( ( fd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP )) == INVALID_SOCKET )
	{
		return INVALID_SOCKET;
	}

	addr.sin_family = AF_INET;
	addr.sin_port   = htons(iPort);

	inet_pton( AF_INET, szIp, &addr.sin_addr.s_addr );
	if( connect( fd, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR )
	{
		closesocket( fd );
		return INVALID_SOCKET;
	}

	return fd;
}

static SSL_CTX	* gpsttServerCtx = NULL;
static SSL_CTX	* gpsttClientCtx = NULL;

static const SSL_METHOD	* gpsttServerMeth;
static const SSL_METHOD * gpsttClientMeth;

int GenerateCookie( SSL *ssl, unsigned char *cookie, unsigned int *cookie_len )
{
	*cookie_len = sprintf( (char *)cookie, "1234567890" );

	return 1;
}

int VerifyCookie(SSL *ssl, unsigned char *cookie, unsigned int cookie_len)
{
	return 1;
}

bool DTLSServerStart( const char * szCertFile, const char * szCaCertFile )
{
	int	n;

	if( szCertFile == NULL ) return false;
	if( IsExistFile( szCertFile ) == false )
	{
		CLog::Print( LOG_ERROR, "cert file is not found" );
		return false;
	}

	gpsttServerMeth = DTLS_server_method();
	if( (gpsttServerCtx = SSL_CTX_new( gpsttServerMeth )) == NULL )
	{
		CLog::Print( LOG_ERROR, "SSL_CTX_new error - server" );
		return false;
	}

	gpsttClientMeth = DTLS_client_method();
	if( (gpsttClientCtx = SSL_CTX_new( gpsttClientMeth )) == NULL )
	{
		CLog::Print( LOG_ERROR, "SSL_CTX_new error - client" );
		return false;
	}

	if( SSL_CTX_use_certificate_file(gpsttServerCtx, szCertFile, SSL_FILETYPE_PEM) <= 0 )
	{
		CLog::Print( LOG_ERROR, "SSL_CTX_use_certificate_file error" );
		return false;
	}

	if( ( n = SSL_CTX_use_PrivateKey_file(gpsttServerCtx, szCertFile, SSL_FILETYPE_PEM)) <= 0 )
	{
		CLog::Print( LOG_ERROR, "SSL_CTX_use_PrivateKey_file error(%d)", n );
		return false;
	}
	
	if( !SSL_CTX_check_private_key( gpsttServerCtx ) )
	{
		CLog::Print( LOG_ERROR, "[SSL] Private key does not match the certificate public key");
		return false;
	}

	if( strlen( szCaCertFile ) > 0 )
	{
		if( SSL_CTX_load_verify_locations( gpsttServerCtx, szCaCertFile, NULL ) == 0 )
		{
			CLog::Print( LOG_ERROR, "[SSL] CaCertFile(%s) load error", szCaCertFile );
			return false;
		}

		SSL_CTX_set_verify( gpsttServerCtx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL );
		//SSL_CTX_set_verify_depth( gpsttServerCtx, 1 );
	}

	SSL_CTX_set_cipher_list(gpsttServerCtx, "ALL:NULL:eNULL:aNULL");
	//SSL_CTX_set_session_cache_mode(gpsttServerCtx, SSL_SESS_CACHE_OFF);

	//SSL_CTX_set_read_ahead(gpsttServerCtx, 1);
	SSL_CTX_set_cookie_generate_cb(gpsttServerCtx, GenerateCookie);
	//SSL_CTX_set_cookie_verify_cb(gpsttServerCtx, &VerifyCookie);

	SSL_CTX_set_cipher_list(gpsttClientCtx, "eNULL:!MD5");

	/*
	if( SSL_CTX_use_certificate_file(gpsttClientCtx, szCertFile, SSL_FILETYPE_PEM) <= 0 )
	{
		CLog::Print( LOG_ERROR, "SSL_CTX_use_certificate_file error" );
		return false;
	}

	if( ( n = SSL_CTX_use_PrivateKey_file(gpsttClientCtx, szCertFile, SSL_FILETYPE_PEM)) <= 0 )
	{
		CLog::Print( LOG_ERROR, "SSL_CTX_use_PrivateKey_file error(%d)", n );
		return false;
	}
	*/

	return true;
}

bool DTLSConnect( const char * pszIp, int iPort, SSL ** ppsttSsl )
{
	SSL * psttSsl;

	char		szIp[INET6_ADDRSTRLEN];
	Socket	fd;

	memset( szIp, 0, sizeof(szIp) );
	if( isdigit(pszIp[0]) == 0 )
	{
		// if first character is not digit, suppose it is domain main.
		GetIpByName( pszIp, szIp, sizeof(szIp) );
	}
	else
	{
		snprintf( szIp, sizeof(szIp), "%s", pszIp );
	}

	struct	sockaddr_in	addr;

	// connect server.
	if( ( fd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP )) == INVALID_SOCKET )
	{
		return false;
	}

	addr.sin_family = AF_INET;
	addr.sin_port   = htons(iPort);

	inet_pton( AF_INET, szIp, &addr.sin_addr.s_addr );
	if( connect( fd, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR )
	{
		closesocket( fd );
		return false;
	}

	if( (psttSsl = SSL_new(gpsttClientCtx)) == NULL )
	{
		return false;
	}

	SSL_set_fd( psttSsl, (int)fd );

	if( SSL_connect( psttSsl ) == -1 )
	{
		SSL_free( psttSsl );
		return false;
	}

	*ppsttSsl = psttSsl;

	return true;
}


THREAD_API UdpClientThread( LPVOID lpParameter )
{
	SSL * psttSsl;
	char	szPacket[8192];
	int		n;

	if( DTLSConnect( "127.0.0.1", 3333, &psttSsl ) == false )
	{
		printf( "SSLConnect() error\n" );
		return 0;
	}

	while( 1 )
	{
		memset( szPacket, 0, sizeof(szPacket) );
		n = SSLRecv( psttSsl, szPacket, sizeof(szPacket) );
		if( n <= 0 )
		{
			printf( "SSLRecv error\n" );
			break;
		}

		printf( "recv[%s]\n", szPacket );
	}

	SSLClose( psttSsl );

	return 0;
}

void UdpServer( const char * pszCertFile )
{
	if( SSLServerStart( pszCertFile, "" ) == false )
	{
		printf( "SSLServerStart() error\n" );
		return;
	}

	if( DTLSServerStart( pszCertFile, "" ) == false )
	{
		printf( "DTLSServerStart() error\n" );
		return;
	}

	Socket hListen = UdpListen( 3333, NULL );
	if( hListen == INVALID_SOCKET )
	{
		printf( "UdpListen() error\n" );
		return;
	}

	StartThread( "UdpClientThread", UdpClientThread, NULL );

	BIO * psttBio;
	SSL * psttSsl;
	struct timeval sttTimeout;
	struct	sockaddr_in	addr;
	char	szPacket[8192];
	int		iLen;

	psttBio = BIO_new_dgram( hListen, BIO_NOCLOSE );
	sttTimeout.tv_sec = 5;
	sttTimeout.tv_usec = 0;
	BIO_ctrl( psttBio, BIO_CTRL_DGRAM_SET_RECV_TIMEOUT, 0, &sttTimeout );

	psttSsl = SSL_new(gpsttServerCtx);
	SSL_set_bio(psttSsl, psttBio, psttBio);

	while( DTLSv1_listen( psttSsl, &addr ) <= 0 )
	{
		
	}

	printf( "connected\n" );

	for( int i = 0; i < 10; ++i )
	{
		iLen = snprintf( szPacket, sizeof(szPacket), "i(%d)", i );
		SSLSend( psttSsl, szPacket, iLen );
		sleep(1);
	}

	SSLClose( psttSsl );
	closesocket( hListen );
}
