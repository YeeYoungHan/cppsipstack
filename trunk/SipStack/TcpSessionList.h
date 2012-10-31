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

#ifndef _TCP_SESSION_LIST_H_
#define _TCP_SESSION_LIST_H_

#include "SipTcp.h"
#include "SipBuffer.h"
#include "TcpThreadList.h"
#include <vector>
#include <string>

/**
 * @brief TCP �����忡 TCP ������ ������ ���� ���Ǵ� Ŭ����
 */
class CTcpComm
{
public:
	Socket	m_hSocket;
	char		m_szIp[16];
	int			m_iPort;
};

/**
 * @brief TCP ������ ���� �����ϴ� TCP ���� ����
 */
class CTcpSessionListInfo
{
public:
	CTcpSessionListInfo();
	void Clear();

	std::string		m_strIp;
	int						m_iPort;
	//SSL						* m_psttSsl;

	CSipBuffer		m_clsSipBuf;

	time_t				m_iConnectTime;
	time_t				m_iRecvTime;
};

typedef std::vector< CTcpSessionListInfo > SESSION_LIST;

/**
 * @brief TCP ������ ���� �����ϴ� TCP ���� ������ �����ϴ� Ŭ����
 */
class CTcpSessionList
{
public:
	CTcpSessionList(void);
	~CTcpSessionList(void);

	bool Init( int iPollFdMax );
	bool Insert( Socket hSocket );
	bool Insert( CTcpComm & clsTcpComm );
	bool Delete( int iIndex, CThreadListEntry * pclsEntry );
	void DeleteAll( CThreadListEntry * pclsEntry );
	void DeleteTimeout( int iTimeout, CThreadListEntry * pclsEntry );

	struct pollfd * m_psttPollFd;
	SESSION_LIST	m_clsList;

	int	m_iPollFdMax;
	int m_iPoolFdCount;
};

#endif
