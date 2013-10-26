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

/** SIP stack ���� Ŭ����
 * @ingroup SipUserAgentAndroid
 * @author websearch (websearch@naver.com)
 */
public class SipStackSetup
{
	/** SIP �޽����� ����Ǵ� ���� IP �ּ� */
	public String m_strLocalIp;

	/** SIP �޽��� ����/���ſ� UDP ��Ʈ ��ȣ */
	public int	m_iLocalUdpPort = 5060;

	/** SIP �޽��� ���ſ� UDP ������ ���� */
	public int	m_iUdpThreadCount = 1;

	/** SIP �޽��� ����/���ſ� TCP ��Ʈ ��ȣ */
	public int	m_iLocalTcpPort = 0;

	/** SIP �޽��� ����/���ſ� TLS ��Ʈ ��ȣ */
	public int	m_iLocalTlsPort = 0;

	/** SIP �޽��� ���ſ� TCP ������ ���� */
	public int	m_iTcpThreadCount = 0;

	/** SIP �޽��� ���ſ� TCP ������ �ϳ��� ���Ե� �� �ִ� �ִ� ���� ���� */
	public int	m_iTcpMaxSocketPerThread = 0;

	/** SIP �޽��� ���ſ� TCP ������ ���� ��� �ð� (�ʴ���) */
	public int	m_iTcpRecvTimeout = 0;

	/** TLS ���� handshake ��� �ð� (�ʴ���) */
	public int	m_iTlsAcceptTimeout = 0;

	/** TLS ������ ���� ���� ������ + ����Ű�� ������ PEM ���� */
	public String	m_strCertFile = null;

	/** SIP UserAgent ����� ����� ���ڿ� */
	public String	m_strUserAgent = "";

	/** SIP �޽����� ������ ���� compact form ���� �������� ���� */
	public boolean	m_bUseSipCompactForm = false;
}
