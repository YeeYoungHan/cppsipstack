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

#ifndef _XML_SIP_SERVER_H_
#define _XML_SIP_SERVER_H_

#include <string>
#include "SipServerInfo.h"

#define FLAG_NULL				0x00
#define FLAG_INSERT			0x01
#define FLAG_UPDATE			0x02
#define FLAG_DELETE			0x04
#define FLAG_NO_CHANGE	0x10

/**
 * @ingroup KSipServer
 * @brief ��ȭ��ȣ prefix �� �����ϴ� Ŭ����
 */
class CRoutePrefix
{
public:
	CRoutePrefix();

	std::string	m_strPrefix;
	bool				m_bDeletePrefix;
};

typedef std::list< CRoutePrefix > ROUTE_PREFIX_LIST;

/**
 * @ingroup KSipServer
 * @brief IP-PBX ���� ���ŵ� ��ȭ��ȣ�� KSipServer �� �α��ε� ���̵�� �����ϴ� Ŭ����
 */
class CIncomingRoute
{
public:
	bool IsEmpty();

	/** IP-PBX ���� ���۵� INVITE �޽����� To ���� ���̵� */
	std::string	m_strToId;

	/** KSipServer �� �α����� ���̵� */
	std::string	m_strDestId;
};

typedef std::list< CIncomingRoute > INCOMING_ROUTE_LIST;

/**
 * @ingroup KSipServer
 * @brief IP-PBX ���� ���� Ŭ����
 */
class CXmlSipServer : public CSipServerInfo
{
public:
	CXmlSipServer();
	~CXmlSipServer();

	/** IP-PBX �̸� */
	std::string		m_strName;

	/** �ڷᱸ�� ������ ���ؼ� ���Ǵ� Flag */
	int						m_iFlag;

	/** Call ������� ���� ��ȭ��ȣ prefix ����Ʈ */
	ROUTE_PREFIX_LIST	m_clsRoutePrefixList;

	/** IP-PBX ���� ���ŵ� ��ȭ��ȣ�� KSipServer �� �α��ε� ���̵�� �����ϴ� ����Ʈ */
	INCOMING_ROUTE_LIST m_clsIncomingRouteList;

	bool Parse( const char * pszFileName );
	void Clear();
};

#endif
