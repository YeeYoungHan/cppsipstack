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

#include "XmlElement.h"
#include "StringMap.h"
#include <string>
#include <list>

/**
 * @ingroup KSipServer
 * @brief KSipServer ���� ������ ������ �����ϴ� Ŭ����
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

	/** Call Pickup �� ���� ���̵� ( ��ȭ��ȣ ) */
	std::string	m_strCallPickupId;

	/** SIP stack ���� �ֱ� (ms ����) */
	int					m_iStackExecutePeriod;

	/** timer D ����ð� (ms ����) */
	int					m_iTimerD;

	/** timer J ����ð� (ms ����) */
	int					m_iTimerJ;

	/** SIP ����� ���� realm */
	std::string	m_strRealm;

	/** SIP REGISTER timeout �ּ� �ð� */
	int					m_iMinRegisterTimeout;

	/** RTP relay ��� ��� ���� */
	bool				m_bUseRtpRelay;

	/** RTP relay �� ����� ���� ��Ʈ ��ȣ */
	int					m_iBeginRtpPort;

	/** RTP relay �� ����� ���� ��Ʈ ��ȣ */
	int					m_iEndRtpPort;

	/** �α��ε� ����ڿ��� OPTIONS �޽����� �����ϴ� �ֱ� (�ʴ���) */
	int					m_iSendOptionsPeriod;

	/** ����� ���� ���� ���� ���� - �� �׸��� �����Ǿ� ���� ������ MySQL DB �� ����Ѵ�. */
	std::string	m_strUserXmlFolder;

	/** IP-PBX ���� ���� ���� - m_strUserXmlFolder �� �����Ǿ� ���� ������ �� �׸��� ������ �ʴ´�. */
	std::string	m_strSipServerXmlFolder;

	/** CDR ���� ���� ���� - m_strUserXmlFolder �� �����Ǿ� ���� ������ �� �׸��� ������ �ʴ´�. */
	std::string	m_strCdrFolder;

	// ================================================================
	// �α� ���

	/** �α� ���� */
	std::string	m_strLogFolder;

	/** �α� ���� */
	int					m_iLogLevel;

	/** �α� ������ �ִ� ũ�� */
	int					m_iLogMaxSize;

	// ================================================================
	// ����͸� ���

	/** ����͸� TCP ��Ʈ ��ȣ */
	int					m_iMonitorPort;

	/** ����͸� TCP ��Ʈ�� ���� ����� IP �ּ� �� */
	CStringMap	m_clsMonitorIpMap;

	// ================================================================
	// ���� ���

	/** SIP transaction list �� �������� ���� SIP User Agent �� */
	CStringMap	m_clsDenySipUserAgentMap;

	/** SIP transaction list �� ������ SIP User Agent �� */
	CStringMap	m_clsAllowSipUserAgentMap;

	/** �α����� ����ϴ� Ŭ���̾�Ʈ IP �ּ� �� */
	CStringMap	m_clsAllowClientIpMap;

	bool Read( const char * pszFileName );
	bool Read( );

	bool IsCallPickupId( const char * pszId );

	bool IsMonitorIp( const char * pszIp );

	bool IsAllowUserAgent( const char * pszSipUserAgent );
	bool IsDenyUserAgent( const char * pszSipUserAgent );

	bool IsAllowClientIp( const char * pszClientIp );
	
	bool IsChange();

private:
	bool Read( CXmlElement & clsXml );
	void SetFileSizeTime( );

	std::string	m_strFileName;	// ���� ���� �̸�
	time_t			m_iFileTime;		// ���� ���� ���� �ð�
	int					m_iFileSize;		// ���� ���� ũ��
};

extern CSipServerSetup gclsSetup;

#endif
