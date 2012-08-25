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

#include <string>

enum EDbType
{
	E_DT_XML = 0,
	E_DT_MYSQL
};

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

	/** SIP ����� ���� realm */
	std::string	m_strRealm;

	/** �α��ε� ����ڿ��� OPTIONS �޽����� �����ϴ� �ֱ� (�ʴ���) */
	int					m_iSendOptionsPeriod;

	/** ����� ���� ���� ���� ���� */
	std::string	m_strUserXmlFolder;

	/** DB ���� IP �ּ� */
	std::string	m_strDbHost;

	/** DB ���� ���� ����� ���̵� */
	std::string m_strDbUserId;

	/** DB ���� ���� ��й�ȣ */
	std::string m_strDbPassWord;

	/** DB ���� �����ͺ��̽� �̸� */
	std::string m_strDbName;

	/** DB ���� ��Ʈ ��ȣ */
	int					m_iDbPort;

	/** �α� ���� */
	std::string	m_strLogFolder;

	/** �α� ���� */
	int					m_iLogLevel;

	/** �α� ������ �ִ� ũ�� */
	int					m_iLogMaxSize;

	/** SIP ����� ���� ���� ���� ��� */
	EDbType			m_eType;

	bool Read( const char * pszFileName );
};

extern CSipServerSetup gclsSetup;

#endif
