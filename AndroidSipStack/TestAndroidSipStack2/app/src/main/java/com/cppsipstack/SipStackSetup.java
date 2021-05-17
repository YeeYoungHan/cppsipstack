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

package com.cppsipstack;

/** SIP stack 설정 클래스
 * @ingroup SipUserAgentAndroid
 * @author websearch (websearch@naver.com)
 */
public class SipStackSetup
{
	/** SIP 메시지에 저장되는 로컬 IP 주소 */
	public String m_strLocalIp = "";

	/** SIP 메시지 전송/수신용 UDP 포트 번호 */
	public int	m_iLocalUdpPort = 5080;

	/** SIP 메시지 수신용 UDP 쓰레드 개수 */
	public int	m_iUdpThreadCount = 1;

	/** SIP 메시지 전송/수신용 TCP 포트 번호 */
	public int	m_iLocalTcpPort = 0;

	/** SIP 메시지 전송/수신용 TLS 포트 번호 */
	public int	m_iLocalTlsPort = 0;

	/** SIP 메시지 수신용 TCP 쓰레드 개수 */
	public int	m_iTcpThreadCount = 1;

	/** SIP 메시지 수신용 TCP 쓰레드 하나에 포함될 수 있는 최대 소켓 개수 */
	public int	m_iTcpMaxSocketPerThread = 10;

	/** SIP 메시지 수신용 TCP 소켓의 수신 대기 시간 (초단위) */
	public int	m_iTcpRecvTimeout = 300;

	/** TLS 세션 handshake 대기 시간 (초단위) */
	public int	m_iTlsAcceptTimeout = 0;

	/** TLS 세션을 위한 서버 인증서 + 개인키를 포함한 PEM 파일 */
	public String	m_strCertFile = "";

	/** SIP UserAgent 헤더에 저장될 문자열 */
	public String	m_strUserAgent = "";

	/** SIP 메시지를 생성할 때에 compact form 으로 생성할지 설정 */
	public boolean	m_bUseSipCompactForm = false;

	/** SIP 메시지를 TLS 로 전송하고 싶으면 true 로 저장하고 그렇지 않으면 false 로 저장한다. */
	public boolean m_bTlsClient = false;
}
