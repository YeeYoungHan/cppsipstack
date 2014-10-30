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

#ifdef USE_TLS

#include "TlsFunction.h"
#include "SipMutex.h"
#include "Log.h"
#include "FileUtility.h"
#include "MemoryDebug.h"

static SSL_CTX	* gpsttServerCtx;
static SSL_CTX	* gpsttClientCtx;

#if OPENSSL_VERSION_NUMBER >= 0x10000003L
static const SSL_METHOD	* gpsttServerMeth;
static const SSL_METHOD * gpsttClientMeth;
#else
static SSL_METHOD	* gpsttServerMeth;
static SSL_METHOD * gpsttClientMeth;
#endif

static bool gbStartSslServer = false;
static CSipMutex * garrMutex;

/**
 * @ingroup SipStack
 * @brief SSL ���̺귯���� multi-thread ���� ����� �� �ֱ� ���� Lock/Unlock function
 * @param mode	CRYPTO_LOCK / CRYPTO_UNLOCK
 * @param n			������ ���̵�
 * @param file 
 * @param line 
 */
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

/**
 * @ingroup SipStack
 * @brief SSL ���̺귯���� multi-thread ���� ����� �� �ֱ� ���� ID function
 * @returns ���� ������ ID �� �����Ѵ�.
 */
static unsigned long SSLIdFunction( )
{
#ifdef WIN32
	return GetCurrentThreadId();
#else
	return pthread_self();
#endif
}

/**
 * @ingroup SipStack
 * @brief SSL ���̺귯���� multi-thread ������� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
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

/**
 * @ingroup SipStack
 * @brief SSL ���̺귯���� ������Ų��.
 * @returns true �� �����Ѵ�.
 */
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

static void SSLPrintError( )
{
	CLog::Print( ERR_print_errors_fp );
}

/**
 * @ingroup SipStack
 * @brief SSL ���� ���̺귯���� �����Ѵ�.
 * @param szCertFile		���� ������ �� ����Ű ����
 * @param szCaCertFile	CA ������ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
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

	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();

	gpsttServerMeth = TLSv1_server_method();
	if( (gpsttServerCtx = SSL_CTX_new( gpsttServerMeth )) == NULL )
	{
		CLog::Print( LOG_ERROR, "SSL_CTX_new error - server" );
		return false;
	}

	gpsttClientMeth = TLSv1_client_method();
	if( (gpsttClientCtx = SSL_CTX_new( gpsttClientMeth )) == NULL )
	{
		CLog::Print( LOG_ERROR, "SSL_CTX_new error - client" );
		return false;
	}

	if( SSL_CTX_use_certificate_file(gpsttServerCtx, szCertFile, SSL_FILETYPE_PEM) <= 0 )
	{
		CLog::Print( LOG_ERROR, "SSL_CTX_use_certificate_file error" );
		SSLPrintError( );
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
	}

	gbStartSslServer = true;

	return true;
}

/**
 * @ingroup SipStack
 * @brief SSL ���� ���̺귯���� �����Ѵ�.
 * @returns true �� �����Ѵ�.
 */
bool SSLServerStop( )
{
	if( gbStartSslServer )
	{
		SSLStop();
		SSL_CTX_free( gpsttServerCtx );

		gbStartSslServer = false;
	}

	return true;
}

/**
 * @ingroup SipStack
 * @brief ���μ����� ����� ���� ���������� �����Ͽ��� openssl �޸� ������ ������� �ʴ´�. 
 */
void SSLFinal()
{
	ERR_free_strings();

#ifdef USE_TLS_FREE
	// http://clseto.mysinablog.com/index.php?op=ViewArticle&articleId=3304652
	ERR_remove_state(0);
	COMP_zlib_cleanup();
	OBJ_NAME_cleanup(-1);
	CRYPTO_cleanup_all_ex_data();
	EVP_cleanup();
	sk_SSL_COMP_free( SSL_COMP_get_compression_methods() );
#endif
}

/**
 * @brief SSL ������ �����Ѵ�.
 * @param iFd				Ŭ���̾�Ʈ TCP ���� �ڵ�
 * @param ppsttSsl	SSL ����ü
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool SSLConnect( Socket iFd, SSL ** ppsttSsl )
{
	SSL * psttSsl;

	if( (psttSsl = SSL_new(gpsttClientCtx)) == NULL )
	{
		return false;
	}
	
	try
	{
		SSL_set_fd( psttSsl, (int)iFd );
		if( SSL_connect( psttSsl ) == -1 )
		{
			SSL_free( psttSsl );
			return false;
		}
	}
	catch( ... )
	{
		CLog::Print( LOG_ERROR, "[SSL] SSLConnect() undefined error" );
		SSL_free( psttSsl );
		return false;
	}

	*ppsttSsl = psttSsl;

	return true;
}

/**
 * @ingroup SipStack
 * @brief Ŭ���̾�Ʈ SSL ���� ��û�� ����Ѵ�.
 * @param iFd								Ŭ���̾�Ʈ TCP ���� �ڵ�
 * @param ppsttSsl					SSL ����ü
 * @param bCheckClientCert	Ŭ���̾�Ʈ �������� Ȯ���� ���ΰ�?
 * @param iVerifyDepth			the maximum depth for the certificate chain verification that shall be allowed for ssl
 * @param iAcceptTimeout		SSL ���� ��û ó�� �ִ� �ð� ( ms ���� )
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool SSLAccept( Socket iFd, SSL ** ppsttSsl, bool bCheckClientCert, int iVerifyDepth, int iAcceptTimeout )
{
	SSL * psttSsl;

	if( (psttSsl = SSL_new( gpsttServerCtx )) == NULL )
	{
		CLog::Print( LOG_ERROR, "SSL_new() error" );
	  return false;
	}

	SSL_set_fd( psttSsl, (int)iFd );

	if( iAcceptTimeout > 0 )
	{
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
	}

	// QQQ : SSL ���������� �ƴ� ��쿡 �޸� ������ �߻��ϹǷ� �Ʒ��� ����
	//     : ���� ������. �� ���� ����� ����Ͽ��� ��.
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

	if( iAcceptTimeout > 0 )
	{
#ifdef WIN32
		int iTimeout = 0;	
		setsockopt( iFd, SOL_SOCKET, SO_RCVTIMEO, (char *)&iTimeout, sizeof(iTimeout) );
#else
		struct timeval	sttTime;

		sttTime.tv_sec = 0;
		sttTime.tv_usec = 0;
		if( setsockopt( iFd, SOL_SOCKET, SO_RCVTIMEO, &sttTime, sizeof(sttTime) ) == -1 )
		{
			CLog::Print( LOG_ERROR, "setsockopt(SO_RCVTIMEO:%d.%d) error(%d)", sttTime.tv_sec, sttTime.tv_usec, GetError() );
		}
#endif
	}

	*ppsttSsl = psttSsl;

	return true;
}

/**
 * @ingroup SipStack
 * @brief SSL �������ݷ� ��Ŷ�� �����Ѵ�.
 * @param ssl			SSL ����ü
 * @param szBuf		���� ��Ŷ
 * @param iBufLen ���� ��Ŷ ũ��
 * @returns ���� ��Ŷ ũ�⸦ �����Ѵ�.
 */
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

/**
 * @ingroup SipStack
 * @brief SSL �������ݷ� ���ŵ� ��Ŷ�� �д´�.
 * @param ssl			SSL ����ü
 * @param szBuf		���� ��Ŷ ���� ����
 * @param iBufLen ���� ��Ŷ ���� ���� ũ��
 * @returns �����ϸ� ����� �����ϰ� �����ϸ� 0 �Ǵ� ������ �����Ѵ�.
 */
int SSLRecv( SSL * ssl, char * szBuf, int iBufLen )
{
	return SSL_read( ssl, szBuf, iBufLen );
}

/**
 * @ingroup SipStack
 * @brief SSL ������ �����Ѵ�.
 * @param ssl	SSL ����ü
 * @returns true �� �����Ѵ�.
 */
bool SSLClose( SSL * ssl )
{
	if( ssl ) 
	{
		SSL_free( ssl );
	}

	return true;
}

#endif
