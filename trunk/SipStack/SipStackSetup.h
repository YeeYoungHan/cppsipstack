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

#ifndef _SIP_STACK_SETUP_H_
#define _SIP_STACK_SETUP_H_

/**
 * @ingroup SipStack
 * @brief SIP stack 설정 클래스
 */
class CSipStackSetup
{
public:
	CSipStackSetup();
	~CSipStackSetup();

	bool Check( );
	int GetLocalPort( ESipTransport eTransport );

	/** SIP 메시지에 저장되는 로컬 IP 주소 */
	std::string m_strLocalIp;

	/** SIP 메시지 전송/수신용 UDP 포트 번호 */
	int					m_iLocalUdpPort;

	/** SIP 메시지 수신용 UDP 쓰레드 개수 */
	int					m_iUdpThreadCount;

	/** SIP 메시지 전송/수신용 TCP 포트 번호 */
	int					m_iLocalTcpPort;

	/** SIP 메시지 전송/수신용 TLS 포트 번호 */
	int					m_iLocalTlsPort;

	/** SIP 메시지 수신용 TCP 쓰레드 개수 */
	int					m_iTcpThreadCount;

	/** SIP 메시지 수신용 TCP 쓰레드 하나에 포함될 수 있는 최대 소켓 개수 */
	int					m_iTcpMaxSocketPerThread;

	/** SIP 메시지 수신용 TCP 소켓의 수신 대기 시간 (초단위) */
	int					m_iTcpRecvTimeout;

	/** TLS 세션 handshake 대기 시간 (초단위) */
	int					m_iTlsAcceptTimeout;

	/** TLS 세션을 위한 서버 인증서 + 개인키를 포함한 PEM 파일 */
	std::string	m_strCertFile;

	/** SIP UserAgent 헤더에 저장될 문자열 */
	std::string	m_strUserAgent;

	/** SIP 메시지를 생성할 때에 compact form 으로 생성할지 설정 */
	bool				m_bUseSipCompactForm;
};

#endif
