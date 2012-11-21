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
 * @ingroup KSipServer
 * @brief SIP 클라이언트 정보 저장 클래스
 */
class CUserInfo
{
public:
	CUserInfo();

	std::string m_strIp;
	int					m_iPort;

	time_t			m_iLoginTime;
	int					m_iLoginTimeout;

	int					m_iOptionsSeq;
	time_t			m_iSendOptionsTime;
};

typedef std::map< std::string, CUserInfo > USER_MAP;

/**
 * @ingroup KSipServer
 * @brief 로그인한 사용자들의 정보를 저장하는 클래스
 */
class CUserMap
{
public:
	CUserMap();
	~CUserMap();

	bool Insert( CSipMessage * pclsMessage, CSipFrom * pclsContact );
	bool Select( const char * pszUserId, CUserInfo & clsInfo );
	bool Select( const char * pszUserId );
	bool Delete( const char * pszUserId );

	bool SetIpPort( const char * pszUserId, const char * pszIp, int iPort );

	void DeleteTimeout( int iTimeout );
	void SendOptions( );

	void GetString( std::string & strBuf );

private:
	USER_MAP	m_clsMap;
	CSipMutex m_clsMutex;
};

extern CUserMap gclsUserMap;

#endif
