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

#ifndef _CALL_MAP_H_
#define _CALL_MAP_H_

#include <string>
#include <map>
#include "SipMutex.h"

/**
 * @ingroup KSipServer
 * @brief 통화 정보 저장 클래스
 */
class CCallInfo
{
public:
	CCallInfo();

	/** 상대 SIP 클라이언트와 연결된 통화 SIP Call-ID */
	std::string	m_strPeerCallId;

	/** 최소 INVITE 를 수신하였는가? */
	bool				m_bRecv;

	/** 상대 SIP 클라이언트와 연동하는 RTP relay 포트 번호. RTP relay 기능을 사용하지 않으면 -1 이 저장된다. */
	int					m_iPeerRtpPort;
};

/**
 * @ingroup KSipServer
 * @brief 연결된 통화 정보를 저장하는 자료구조. key 와 value 는 SIP Call-ID 이다.
 */
typedef std::map< std::string, CCallInfo > CALL_MAP;

/**
 * @ingroup KSipServer
 * @brief 연결된 통화 정보를 저장하는 자료구조 클래스
 */
class CCallMap
{
public:
	CCallMap();
	~CCallMap();

	bool Insert( const char * pszRecvCallId, const char * pszSendCallId, int iStartRtpPort );
	bool Insert( const char * pszCallId, CCallInfo & clsCallInfo );
	bool Update( const char * pszCallId, const char * pszPeerCallId );
	bool Select( const char * pszCallId, std::string & strCallId );
	bool Select( const char * pszCallId, CCallInfo & clsCallInfo );
	bool SelectToRing( const char * pszTo, std::string & strCallId );
	bool Delete( const char * pszCallId, bool bStopPort = true );
	bool DeleteOne( const char * pszCallId );

	void StopCallAll();
	int GetCount();

	void GetString( std::string & strBuf );

private:
	CALL_MAP	m_clsMap;
	CSipMutex	m_clsMutex;
};

extern CCallMap gclsCallMap;
extern CCallMap gclsTransCallMap;

#endif
