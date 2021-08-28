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

static SSL_CTX	* gpsttClientCtx = NULL;
static const SSL_METHOD * gpsttClientMeth;

/**
 * @ingroup TestWebRtc
 * @brief RSA 키와 인증서 저장 클래스
 */
class CRSAKeyCert
{
public:
	CRSAKeyCert() : m_psttKey(NULL), m_psttCert(NULL)
	{}

	~CRSAKeyCert()
	{
		Clear();
	}

	void Clear()
	{
		if( m_psttKey )
		{
			EVP_PKEY_free( m_psttKey );
			m_psttKey = NULL;
		}

		if( m_psttCert )
		{
			X509_free( m_psttCert );
			m_psttCert = NULL;
		}
	}

	EVP_PKEY		* m_psttKey;
	X509				* m_psttCert;
	std::string	m_strFingerPrint;
};

CRSAKeyCert gclsKeyCert;

/**
 * @ingroup TestWebRtc
 * @brief RSA 키와 인증서를 생성한다.
 * @param clsKeyCert [out] RSA 키와 인증서 저장 객체
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CreateRSA( CRSAKeyCert & clsKeyCert )
{
	BIGNUM * psttBigNum = BN_new();
	if( psttBigNum == NULL )
	{
		return false;
	}

	if( BN_set_word( psttBigNum, RSA_F4 ) )
	{
		RSA * psttRSA = RSA_new();
		if( psttRSA )
		{
			if( RSA_generate_key_ex( psttRSA, 2048, psttBigNum, NULL ) )
			{
				clsKeyCert.m_psttKey = EVP_PKEY_new();
				if( clsKeyCert.m_psttKey )
				{
					if( EVP_PKEY_assign_RSA( clsKeyCert.m_psttKey, psttRSA ) )
					{
						clsKeyCert.m_psttCert = X509_new();
						if( clsKeyCert.m_psttCert )
						{
							X509_set_version( clsKeyCert.m_psttCert, 2 );
							ASN1_INTEGER_set( X509_get_serialNumber(clsKeyCert.m_psttCert), 1 );
							X509_gmtime_adj( X509_get_notBefore(clsKeyCert.m_psttCert), -31536000 );
							X509_gmtime_adj( X509_get_notAfter(clsKeyCert.m_psttCert), 31536000 );

							if( X509_set_pubkey( clsKeyCert.m_psttCert, clsKeyCert.m_psttKey ) )
							{
								X509_NAME * psttName = X509_get_subject_name( clsKeyCert.m_psttCert );
								if( psttName )
								{
									X509_NAME_add_entry_by_txt( psttName, "O", MBSTRING_ASC, (const unsigned char*)"TestWebRTC", -1, -1, 0 );
									X509_NAME_add_entry_by_txt( psttName, "CN", MBSTRING_ASC, (const unsigned char*)"TestWebRTC", -1, -1, 0 );

									if( X509_set_issuer_name( clsKeyCert.m_psttCert, psttName ) && 
											X509_sign( clsKeyCert.m_psttCert, clsKeyCert.m_psttKey, EVP_sha1() ) )
									{
										BN_free( psttBigNum );
										return true;
									}
								}
							}
						}
					}
				}
			}

			if( clsKeyCert.m_psttKey )
			{
				EVP_PKEY_free( clsKeyCert.m_psttKey );
			}
			else
			{
				RSA_free( psttRSA );
			}

			if( clsKeyCert.m_psttCert )
			{
				X509_free( clsKeyCert.m_psttCert );
			}
		}
	}

	BN_free( psttBigNum );

	return false;
}

/**
 * @ingroup TestWebRtc
 * @brief DTLS 사용을 위한 초기화 과정을 수행한다.
 */
void InitDtls()
{
	SSLStart();

	gpsttClientMeth = DTLSv1_2_method();
	if( (gpsttClientCtx = SSL_CTX_new( gpsttClientMeth )) == NULL )
	{
		printf( "SSL_CTX_new error - client" );
		return;
	}

	if( CreateRSA( gclsKeyCert ) == false )
	{
		printf( "CreateRSA error\n" );
		return;
	}

	if( !SSL_CTX_use_certificate( gpsttClientCtx, gclsKeyCert.m_psttCert ) )
	{
		printf( "SSL_CTX_use_certificate error\n" );
		return;
	}

	if( !SSL_CTX_use_PrivateKey( gpsttClientCtx, gclsKeyCert.m_psttKey ) )
	{
		printf( "SSL_CTX_use_PrivateKey error\n" );
		return;
	}

	unsigned int iSize;
	unsigned char szHash[EVP_MAX_MD_SIZE];
	char szFingerPrint[EVP_MAX_MD_SIZE*3+1];
	int iPos = 0;

	if( !X509_digest( gclsKeyCert.m_psttCert, EVP_sha256(), szHash, &iSize ) )
	{
		printf( "X509_digest error\n" );
		return;
	}

	memset( szFingerPrint, 0, sizeof(szFingerPrint) );
	for( unsigned int i = 0; i < iSize; ++i )
	{
		if( iPos == 0 )
		{
			sprintf( szFingerPrint + iPos, "%02X", szHash[i] );
			iPos += 2;
		}
		else
		{
			sprintf( szFingerPrint + iPos, ":%02X", szHash[i] );
			iPos += 3;
		}
	}

	gclsKeyCert.m_strFingerPrint = szFingerPrint;
}

/**
 * @ingroup TestWebRtc
 * @brief DTLS 를 위한 메모리를 해제한다.
 */
void FinalDtls()
{
	if( gpsttClientCtx )
	{
		SSL_CTX_free( gpsttClientCtx );
		gpsttClientCtx = NULL;
	}

	gclsKeyCert.Clear();
}
