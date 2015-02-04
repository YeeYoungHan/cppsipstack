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

#ifndef _TLS_FUNCTION_H_
#define _TLS_FUNCTION_H_

#ifdef USE_TLS

#include "SipTcp.h"
#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

bool SSLServerStart( const char * szCertFile, const char * szCaCertFile );
bool SSLServerStop( );
void SSLFinal();

bool SSLConnect( Socket iFd, SSL ** ppsttSsl );
bool SSLAccept( Socket iFd, SSL ** ppsttSsl, bool bCheckClientCert, int iVerifyDepth, int iAcceptTimeout );
int SSLSend( SSL * ssl, const char * szBuf, int iBufLen );
int SSLRecv( SSL * ssl, char * szBuf, int iBufLen );
bool SSLClose( SSL * ssl );

void SSLPrintLogServerCipherList( );
void SSLPrintLogClientCipherList( );

#else

#define SSL void

#endif

#endif
