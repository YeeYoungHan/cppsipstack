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

	/** SIP 통신을 위한 UDP 포트 번호 */
	int					m_iUdpPort;

	/** SIP 통신을 위한 로컬 IP 주소 */
	std::string	m_strLocalIp;

	/** SIP 서버 IP 주소 */
	std::string	m_strSipServerIp;

	/** SIP 서버 포트 번호 */
	int					m_iSipServerPort;

	/** SIP 도메인 */
	std::string	m_strSipDomain;

	/** SIP 사용자 아이디 */
	std::string	m_strSipUserId;

	/** SIP 비밀번호 */
	std::string	m_strSipPassWord;

	/** SIP 통신 프로토콜 */
	ESipTransport	m_eSipTransport;

	/** TLS SIP 통신을 위한 인증서와 개인키를 저장한 PEM 파일 경로 */
	std::string		m_strPemFile;

	/** RTP 를 읽어서 전송할 pcap 파일 full path */
	std::string		m_strPcapFile;

	/** pcap 파일에서 읽을 RTP 목적지 IP 주소 */
	std::string		m_strPcapRtpDestIp;

	/** pcap 파일에서 읽을 RTP 목적지 포트 번호 */
	int						m_iPcapRtpDestPort;

	/** pcap 파일에서 읽은 SRTP 를 복호화하기 위한 암호화 알고리즘 (예: AES_CM_128_HMAC_SHA1_80 ) */
	std::string		m_strPcapSrtpCrypto;

	/** pcap 파일에서 읽은 SRTP 를 복호화하기 위한 암호화 키 */
	std::string		m_strPcapSrtpKey;

	bool Read( const char * pszFileName );
};

extern CSipClientSetup gclsSetupFile;

#endif
