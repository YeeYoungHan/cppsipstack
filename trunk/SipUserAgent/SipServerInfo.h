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

#ifndef _SIP_REGISTER_INFO_H_
#define _SIP_REGISTER_INFO_H_

#include "SipMessage.h"
#include "SipStack.h"

/**
 * @ingroup SipUserAgent
 * @brief SIP �α��� ������ �����ϴ� Ŭ����
 */
class CSipServerInfo
{
public:
	CSipServerInfo();
	~CSipServerInfo();

	bool Equal( CSipServerInfo & clsInfo );
	bool Equal( const char * pszIp, int iPort, ESipTransport eTransport );
	void Update( CSipServerInfo & clsInfo );
	void ClearLogin();

	/** SIP ������ IP �ּ� */
	std::string		m_strIp;

	/** SIP ������ ��Ʈ ��ȣ */
	int						m_iPort;

	/** SIP ������ ������ */
	std::string		m_strDomain;

	/** �α��� ���̵� */
	std::string		m_strUserId;

	/** �α��� ��й�ȣ */
	std::string		m_strPassWord;

	/** �α��� ���� �ð� (�ʴ���) */
	int						m_iLoginTimeout;

	/** transport */
	ESipTransport	m_eTransport;

	/** NAT ���� �ð� (�ʴ���) */
	int						m_iNatTimeout;

	bool					m_bLogin;
	time_t				m_iLoginTime;
	time_t				m_iSendTime;
	time_t				m_iResponseTime;
	time_t				m_iNextSendTime;
	CSipCallId		m_clsCallId;
	int						m_iSeqNo;
	bool					m_bAuth;
	
	bool					m_bDelete;

	CSipMessage * CreateRegister( CSipStack * pclsSipStack, CSipMessage * pclsResponse );
	bool AddAuth( CSipMessage * pclsRequest, CSipMessage * pclsResponse );
};

typedef std::list< CSipServerInfo > SIP_SERVER_INFO_LIST;

#endif
