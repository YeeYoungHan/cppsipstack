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

#ifndef _SIP_SERVER_SETUP_H_
#define _SIP_SERVER_SETUP_H_

#include "StringMap.h"
#include "XmlElement.h"

/**
 * @ingroup SipLoadBalancer
 * @brief SipLoadBalancer ���� ������ ������ �����ϴ� Ŭ����
 */
class CSipServerSetup
{
public:
	CSipServerSetup();
	~CSipServerSetup();

	/** SIP ����� ���� UDP ��Ʈ ��ȣ */
	int					m_iUdpPort;

	/** SIP ����� ���� ���� IP �ּ� */
	std::string	m_strLocalIp;

	/** SIP ����� ���� UDP ���� ������ ���� */
	int					m_iUdpThreadCount;

	/** SIP ����� ���� TCP ��Ʈ ��ȣ */
	int					m_iTcpPort;

	/** SIP ����� ���� TCP ���� ������ ���� */
	int					m_iTcpThreadCount;

	/** SIP ����� ���� TCP ���� �ִ� ��� �ð� ( �ʴ��� ) */
	int					m_iTcpRecvTimeout;

	/** SIP ����� ���� TLS ��Ʈ ��ȣ */
	int					m_iTlsPort;

	/** SIP ����� ���� TLS handshake timeout �ð� (�ʴ���) */
	int					m_iTlsAcceptTimeout;

	/** TLS ���������� ���� ���� ������ + Ű�� ������ PEM ���� full path */ 
	std::string	m_strCertFile;

	/** �α� ���� */
	std::string	m_strLogFolder;

	/** �α� ���� */
	int					m_iLogLevel;

	/** �α� ������ �ִ� ũ�� */
	int					m_iLogMaxSize;

	/** ����͸� TCP ��Ʈ ��ȣ */
	int					m_iMonitorPort;

	/** ����͸� TCP ��Ʈ�� ���� ����� IP �ּ� ����Ʈ */
	CStringMap		 m_clsMonitorIpMap;

	/** SIP transaction list �� �������� ���� SIP User Agent �� */
	CStringMap		 m_clsDenySipUserAgentMap;

	bool Read( const char * pszFileName );
	bool Read( );
	bool IsMonitorIp( const char * pszIp );
	bool IsChange();

private:
	bool Read( CXmlElement & clsXml );
	bool ReadSipServer( CXmlElement & clsXml );
	void SetFileSizeTime( );

	std::string	m_strFileName;	// ���� ���� �̸�
	time_t			m_iFileTime;		// ���� ���� ���� �ð�
	int					m_iFileSize;		// ���� ���� ũ��
};

extern CSipServerSetup gclsSetup;

#endif
