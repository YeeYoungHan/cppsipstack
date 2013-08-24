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
 * @brief SIP stack ���� Ŭ����
 */
class CSipStackSetup
{
public:
	CSipStackSetup();
	~CSipStackSetup();

	bool Check( );
	int GetLocalPort( ESipTransport eTransport );

	/** SIP �޽����� ����Ǵ� ���� IP �ּ� */
	std::string m_strLocalIp;

	/** SIP �޽��� ����/���ſ� UDP ��Ʈ ��ȣ */
	int					m_iLocalUdpPort;

	/** SIP �޽��� ���ſ� UDP ������ ���� */
	int					m_iUdpThreadCount;

	/** SIP �޽��� ����/���ſ� TCP ��Ʈ ��ȣ */
	int					m_iLocalTcpPort;

	/** SIP �޽��� ����/���ſ� TLS ��Ʈ ��ȣ */
	int					m_iLocalTlsPort;

	/** SIP �޽��� ���ſ� TCP ������ ���� */
	int					m_iTcpThreadCount;

	/** SIP �޽��� ���ſ� TCP ������ �ϳ��� ���Ե� �� �ִ� �ִ� ���� ���� */
	int					m_iTcpMaxSocketPerThread;

	/** SIP �޽��� ���ſ� TCP ������ ���� ��� �ð� (�ʴ���) */
	int					m_iTcpRecvTimeout;

	/** TLS ���� handshake ��� �ð� (�ʴ���) */
	int					m_iTlsAcceptTimeout;

	/** TLS ������ ���� ���� ������ + ����Ű�� ������ PEM ���� */
	std::string	m_strCertFile;

	/** SIP UserAgent ����� ����� ���ڿ� */
	std::string	m_strUserAgent;

	/** SIP �޽����� ������ ���� compact form ���� �������� ���� */
	bool				m_bUseSipCompactForm;
};

#endif
