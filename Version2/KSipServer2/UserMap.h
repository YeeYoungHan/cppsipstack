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
 * @brief SIP 클라이언트 정보 저장 클래스
 */
class CUserInfo
{
public:
	CUserInfo();
	void GetCallRoute( CSipCallRoute & clsRoute );

	/** 클라이언트 IP 주소 */
	std::string m_strIp;

	/** 클라이언트 포트 번호 */
	int					m_iPort;

	/** 클라이언트 연결 트랜스포트 */
	ESipTransport	m_eTransport;

	/** 로그인 시간 */
	time_t			m_iLoginTime;

	/** 로그인 timeout 시간 (초단위) */
	int					m_iLoginTimeout;

	/** OPTIONS 메시지 전송 SEQ 번호 */
	int					m_iOptionsSeq;

	/** OPTIONS 메시지 전송 시간 */
	time_t			m_iSendOptionsTime;

	/** Call Pickup 을 위한 그룹 아이디 */
	std::string	m_strGroupId;
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
