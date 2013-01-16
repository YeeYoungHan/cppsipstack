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
#include <map>

/**
 * @ingroup SimpleSipServer
 * @brief SIP Ŭ���̾�Ʈ ���� ���� Ŭ����
 */
class CUserInfo
{
public:
	std::string m_strIp;
	int					m_iPort;
};

typedef std::map< std::string, CUserInfo > USER_MAP;

/**
 * @ingroup SimpleSipServer
 * @brief �α����� ����ڵ��� ������ �����ϴ� Ŭ����
 */
class CUserMap
{
public:
	CUserMap();
	~CUserMap();

	bool Insert( CSipMessage * pclsMessage, CSipFrom * pclsContact );
	bool Select( const char * pszUserId, CUserInfo & clsInfo );

private:
	USER_MAP	m_clsMap;
	CSipMutex m_clsMutex;
};

extern CUserMap gclsUserMap;

#endif
