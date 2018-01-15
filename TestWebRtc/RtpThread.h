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

#ifndef _RTP_THREAD_H_
#define _RTP_THREAD_H_

#include "SipUdp.h"
#include "SipCallBack.h"

/**
 * @ingroup TestWebRtc
 * @brief RTP 쓰레드 정보 저장 클래스
 */
class CRtpThreadArg
{
public:
	CRtpThreadArg();
	~CRtpThreadArg();

	bool CreateSocket();
	void Close();

	Socket	m_hWebRtcUdp;
	Socket	m_hPbxUdp;

	int			m_iWebRtcUdpPort;
	int			m_iPbxUdpPort;

	bool		m_bStop;
	bool		m_bStartCall;

	std::string	m_strUserId;
	std::string m_strToId;
	std::string m_strSdp;

	std::string m_strIcePwd;
	std::string m_strCallId;
};

/**
 * @ingroup TestWebRtc
 * @brief RSA 키와 인증서 저장 클래스
 */
class CRSAKeyCert
{
public:
	CRSAKeyCert() : m_psttKey(NULL), m_psttCert(NULL)
	{}

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

void InitDtls();
void FinalDtls();

bool StartRtpThread( CRtpThreadArg * pclsArg );

extern CRSAKeyCert gclsKeyCert;

#endif
