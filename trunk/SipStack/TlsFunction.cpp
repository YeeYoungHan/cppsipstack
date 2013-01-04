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

#include "SipStackDefine.h"
#include "TlsFunction.h"
#include "SipMutex.h"
#include "Log.h"
#include "ServerUtility.h"

static SSL_CTX	* ctx;
static SSL_METHOD	* meth;

static bool gbStartSslServer = false;
static CSipMutex * garrMutex;

/** SSL 라이브러리를 multi-thread 에서 사용할 수 있기 위한 Lock/Unlock function */
static void SSLLockingFunction( int mode, int n, const char * file, int line )
{
	if( mode & CRYPTO_LOCK )
	{
		garrMutex[n].acquire();
	}
	else
	{
		garrMutex[n].release();
	}
}

/** SSL 라이브러리를 multi-thread 에서 사용할 수 있기 위한 ID function */
static unsigned long SSLIdFunction( )
{
#ifdef WIN32
	return GetCurrentThreadId();
#else
	return pthread_self();
#endif
}

static bool SSLStart( )
{
	garrMutex = new CSipMutex[ CRYPTO_num_locks() ];
	if( garrMutex == NULL )
	{
		CLog::Print( LOG_ERROR, "%s CMutex[] new error", __FUNCTION__ );
		return false;
	}

	CRYPTO_set_id_callback( SSLIdFunction );
	CRYPTO_set_locking_callback( SSLLockingFunction );

	if( !SSL_library_init() )
	{
		CLog::Print( LOG_ERROR, "SSL_init_library error" );
		return false;
	}

	return true;
}

static bool SSLStop( )
{
	CRYPTO_set_id_callback(NULL);
	CRYPTO_set_locking_callback(NULL);

	if( garrMutex )
	{
		delete [] garrMutex;
		garrMutex = NULL;
	}

	return true;
}

bool SSLServerStart( const char * szCertFile, const char * szCaCertFile )
{
	int	n;

	if( szCertFile == NULL ) return false;
	if( IsExistFile( szCertFile ) == false )
	{
		CLog::Print( LOG_ERROR, "cert file is not found" );
		return false;
	}

	if( SSLStart() == false ) return false;

	if( !SSL_library_init() )
	{
		CLog::Print( LOG_ERROR, "SSL_init_library error");
		return false;
	}

	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();

	//meth = SSLv3_server_method();
	meth = TLSv1_server_method();
	if( (ctx = SSL_CTX_new( meth )) == NULL )
	{
		CLog::Print( LOG_ERROR, "SSL_CTX_new error" );
		return false;
	}

	if( SSL_CTX_use_certificate_file(ctx, szCertFile, SSL_FILETYPE_PEM) <= 0 )
	{
		CLog::Print( LOG_ERROR, "SSL_CTX_use_certificate_file error" );
		return false;
	}

	if( ( n = SSL_CTX_use_PrivateKey_file(ctx, szCertFile, SSL_FILETYPE_PEM)) <= 0 )
	{
		CLog::Print( LOG_ERROR, "SSL_CTX_use_PrivateKey_file error(%d)", n );
		return false;
	}
	
	if( !SSL_CTX_check_private_key( ctx ) )
	{
		CLog::Print( LOG_ERROR, "[SSL] Private key does not match the certificate public key");
		return false;
	}

	if( strlen( szCaCertFile ) > 0 )
	{
		if( SSL_CTX_load_verify_locations( ctx, szCaCertFile, NULL ) == 0 )
		{
			CLog::Print( LOG_ERROR, "[SSL] CaCertFile(%s) load error", szCaCertFile );
			return false;
		}
	}

	gbStartSslServer = true;

	return true;
}

bool SSLServerStop( )
{
	if( gbStartSslServer )
	{
		SSLStop();
		SSL_CTX_free( ctx );

		gbStartSslServer = false;
	}

	return true;
}

bool SSLAccept( Socket iFd, SSL ** ppsttSsl, bool bCheckClientCert, int iVerifyDepth, int iAcceptTimeout )
{
	SSL * psttSsl;

	if( (psttSsl = SSL_new( ctx )) == NULL )
	{
		CLog::Print( LOG_ERROR, "SSL_new() error" );
	  return false;
	}

	SSL_set_fd( psttSsl, (int)iFd );

#ifdef WIN32
	int		iTimeout = iAcceptTimeout;
	setsockopt( iFd, SOL_SOCKET, SO_RCVTIMEO, (char *)&iTimeout, sizeof(iTimeout) );
#else
	struct timeval	sttTime;

	sttTime.tv_sec = iAcceptTimeout / 1000;
	sttTime.tv_usec = ( iAcceptTimeout % 1000 ) * 1000;

	CLog::Print( LOG_DEBUG, "SO_RCVTIMEO(%d.%d)", sttTime.tv_sec, sttTime.tv_usec );
	if( setsockopt( iFd, SOL_SOCKET, SO_RCVTIMEO, &sttTime, sizeof(sttTime) ) == -1 )
	{
		CLog::Print( LOG_ERROR, "setsockopt(SO_RCVTIMEO:%d.%d) error(%d)", sttTime.tv_sec, sttTime.tv_usec, GetError() );
	}
#endif

	// QQQ : SSL 프로토콜이 아닌 경우에 메모리 에러가 발생하므로 아래와 같이
	//     : 막아 놓았음. 더 좋은 방법을 모색하여야 함.
	try
	{
		if( bCheckClientCert )
		{
			SSL_set_verify( psttSsl, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT | SSL_VERIFY_CLIENT_ONCE, NULL );
			SSL_set_verify_depth( psttSsl, iVerifyDepth );
		}

		if( SSL_accept( psttSsl ) == -1 )
		{
			CLog::Print( LOG_ERROR, "SSL_accept() error" );
			SSL_free( psttSsl );
			return false;
		}
	}
	catch( ... )
	{
		CLog::Print( LOG_ERROR, "[SSL] SSL_accept() undefined error" );
		SSL_free( psttSsl );
		return false;
	}


#ifdef WIN32
	iTimeout = 0;	
	setsockopt( iFd, SOL_SOCKET, SO_RCVTIMEO, (char *)&iTimeout, sizeof(iTimeout) );
#else
	sttTime.tv_sec = 0;
	sttTime.tv_usec = 0;
	if( setsockopt( iFd, SOL_SOCKET, SO_RCVTIMEO, &sttTime, sizeof(sttTime) ) == -1 )
	{
		CLog::Print( LOG_ERROR, "setsockopt(SO_RCVTIMEO:%d.%d) error(%d)", sttTime.tv_sec, sttTime.tv_usec, GetError() );
	}
#endif

	*ppsttSsl = psttSsl;

	return true;
}

int SSLSend( SSL * ssl, const char * szBuf, int iBufLen )
{
	int		n;	
	int		iSendLen = 0;
	
	try
	{
		while( 1 )
		{
			n = SSL_write( ssl, szBuf + iSendLen, iBufLen - iSendLen );
			if( n <= 0 ) return -1;
		
			iSendLen += n;
			if( iSendLen == iBufLen ) break;	
		}
	}
	catch( ... )
	{
		CLog::Print( LOG_ERROR, "[SSL] SSLSend() undefined error" );
	}
	
	return iBufLen;
}

int SSLRecv( SSL * ssl, char * szBuf, int iBufLen )
{
	return SSL_read( ssl, szBuf, iBufLen );
}

bool SSLClose( SSL * ssl )
{
	if( ssl ) 
	{
		SSL_free( ssl );
	}

	return true;
}
