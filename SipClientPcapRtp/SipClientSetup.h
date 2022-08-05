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

#ifndef _SIP_CLIENT_SETUP_H_
#define _SIP_CLIENT_SETUP_H_

#include <string>
#include "SipTransport.h"

class CSipClientSetup
{
public:
	CSipClientSetup();
	~CSipClientSetup();

	/** SIP ����� ���� UDP ��Ʈ ��ȣ */
	int					m_iUdpPort;

	/** SIP ����� ���� ���� IP �ּ� */
	std::string	m_strLocalIp;

	/** SIP ���� IP �ּ� */
	std::string	m_strSipServerIp;

	/** SIP ���� ��Ʈ ��ȣ */
	int					m_iSipServerPort;

	/** SIP ������ */
	std::string	m_strSipDomain;

	/** SIP ����� ���̵� */
	std::string	m_strSipUserId;

	/** SIP ��й�ȣ */
	std::string	m_strSipPassWord;

	/** SIP ��� �������� */
	ESipTransport	m_eSipTransport;

	/** TLS SIP ����� ���� �������� ����Ű�� ������ PEM ���� ��� */
	std::string		m_strPemFile;

	/** RTP �� �о ������ pcap ���� full path */
	std::string		m_strPcapFile;

	/** pcap ���Ͽ��� ���� RTP ������ IP �ּ� */
	std::string		m_strPcapRtpDestIp;

	/** pcap ���Ͽ��� ���� RTP ������ ��Ʈ ��ȣ */
	int						m_iPcapRtpDestPort;

	/** pcap ���Ͽ��� ���� SRTP �� ��ȣȭ�ϱ� ���� ��ȣȭ �˰��� (��: AES_CM_128_HMAC_SHA1_80 ) */
	std::string		m_strPcapSrtpCrypto;

	/** pcap ���Ͽ��� ���� SRTP �� ��ȣȭ�ϱ� ���� ��ȣȭ Ű */
	std::string		m_strPcapSrtpKey;

	bool Read( const char * pszFileName );
};

extern CSipClientSetup gclsSetupFile;

#endif
