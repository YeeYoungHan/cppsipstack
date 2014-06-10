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

#ifndef _USER_MAP_H_
#define _USER_MAP_H_

#include "SipMutex.h"
#include "SipMessage.h"
#include "SipUserAgent.h"
#include "XmlUser.h"
#include <map>

typedef std::list< std::string > USER_ID_LIST;

/**
 * @ingroup KSipServer
 * @brief SIP Ŭ���̾�Ʈ ���� ���� Ŭ����
 */
class CUserInfo
{
public:
	CUserInfo();
	void GetCallRoute( CSipCallRoute & clsRoute );

	/** Ŭ���̾�Ʈ IP �ּ� */
	std::string m_strIp;

	/** Ŭ���̾�Ʈ ��Ʈ ��ȣ */
	int					m_iPort;

	/** Ŭ���̾�Ʈ ���� Ʈ������Ʈ */
	ESipTransport	m_eTransport;

	/** �α��� �ð� */
	time_t			m_iLoginTime;

	/** �α��� timeout �ð� (�ʴ���) */
	int					m_iLoginTimeout;

	/** OPTIONS �޽��� ���� SEQ ��ȣ */
	int					m_iOptionsSeq;

	/** OPTIONS �޽��� ���� �ð� */
	time_t			m_iSendOptionsTime;

	/** Call Pickup �� ���� �׷� ���̵� */
	std::string	m_strGroupId;
};

typedef std::map< std::string, CUserInfo > USER_MAP;

/**
 * @ingroup KSipServer
 * @brief �α����� ����ڵ��� ������ �����ϴ� Ŭ����
 */
class CUserMap
{
public:
	CUserMap();
	~CUserMap();

	bool Insert( CSipMessage * pclsMessage, CSipFrom * pclsContact, CXmlUser * pclsXmlUser );
	bool Select( const char * pszUserId, CUserInfo & clsInfo );
	bool Select( const char * pszUserId );
	bool SelectGroup( const char * pszGroupId, USER_ID_LIST & clsList );
	bool Delete( const char * pszUserId );

	bool SetIpPort( const char * pszUserId, const char * pszIp, int iPort );

	void DeleteTimeout( int iTimeout );
	void SendOptions( );

	void GetString( CMonitorString & strBuf );

private:
	USER_MAP	m_clsMap;
	CSipMutex m_clsMutex;
};

extern CUserMap gclsUserMap;

#endif
