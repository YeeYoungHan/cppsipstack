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

	/** SIP 메시지 수신 callback 처리를 위한 TCP 쓰레드 개수. 
			본 개수가 0 이면 TCP 수신 쓰레드에서 callback 을 호출하고 0 보다 크면 tcp callback 쓰레드에서 callback 을 호출한다. */
	int					m_iTcpCallBackThreadCount;

	/** SIP 메시지 수신용 TCP 쓰레드 하나에 포함될 수 있는 최대 소켓 개수 */
	int					m_iTcpMaxSocketPerThread;

	/** SIP 메시지 수신용 TCP 소켓의 수신 대기 시간 (초단위) */
	int					m_iTcpRecvTimeout;

	/** TCP 세션 연결 timeout 시간 (초단위) */
	int					m_iTcpConnectTimeout;

	/** TLS 세션 handshake 대기 시간 (초단위) */
	int					m_iTlsAcceptTimeout;

	/** TLS 세션을 위한 서버 인증서 + 개인키를 포함한 PEM 파일 */
	std::string	m_strCertFile;

	/** TLS 세션으로 연결한 클라이언트 인증을 위한 인증 기관 인증서 PEM 파일 */
	std::string m_strCaCertFile;

	/** SIP UserAgent 헤더에 저장될 문자열 */
	std::string	m_strUserAgent;

	/** SIP 메시지를 생성할 때에 compact form 으로 생성할지 설정 */
	bool				m_bUseSipCompactForm;

	/** SIP stack 실행 주기 (ms 단위) */
	int					m_iStackExecutePeriod;

	/** timer D 만료시간 (ms 단위) */
	int					m_iTimerD;

	/** timer J 만료시간 (ms 단위) */
	int					m_iTimerJ;

	/** IPv6 사용 유무 */
	bool				m_bIpv6;

	/** Stateful SIP stack 인가? */
	bool				m_bStateful;

	/** TLS 클라이언트만 사용하는가? SIP 클라이언트에서 TLS 서버는 사용하지 않고 TLS 클라이언트만 사용하는 경우 true 로 설정한다. */
	bool				m_bTlsClient;

	/** SIP 요청 메시지를 전송할 때에 Contact 헤더에 수신 포트 번호를 사용하는 경우 true 로 설정한다. */
	bool				m_bUseContactListenPort;

	/** SIP REGISTER 를 전송한 후, 수신한 401 응답의 Authenticate 를 저장하여서 다음 주기의 SIP REGISTER 메시지를 생성할 때에 사용하는 경우 true 로 설정한다. */
	bool				m_bUseRegisterSession;

	/** UDP 전송/수신 버퍼 최대 크기 */
	int					m_iUdpBufSize;
};

#endif
