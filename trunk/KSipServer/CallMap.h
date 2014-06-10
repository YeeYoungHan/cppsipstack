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
 * @brief ��ȭ ���� ���� Ŭ����
 */
class CCallInfo
{
public:
	CCallInfo();

	/** ��� SIP Ŭ���̾�Ʈ�� ����� ��ȭ SIP Call-ID */
	std::string	m_strPeerCallId;

	/** �ּ� INVITE �� �����Ͽ��°�? */
	bool				m_bRecv;

	/** ��� SIP Ŭ���̾�Ʈ�� �����ϴ� RTP relay ��Ʈ ��ȣ. RTP relay ����� ������� ������ -1 �� ����ȴ�. */
	int					m_iPeerRtpPort;
};

/**
 * @ingroup KSipServer
 * @brief ����� ��ȭ ������ �����ϴ� �ڷᱸ��. key �� value �� SIP Call-ID �̴�.
 */
typedef std::map< std::string, CCallInfo > CALL_MAP;

/**
 * @ingroup KSipServer
 * @brief ����� ��ȭ ������ �����ϴ� �ڷᱸ�� Ŭ����
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
	bool Select( const char * pszCallId );
	bool SelectToRing( const char * pszTo, std::string & strCallId );
	bool Delete( const char * pszCallId, bool bStopPort = true );
	bool DeleteOne( const char * pszCallId );

	void StopCallAll();
	int GetCount();

	void GetString( CMonitorString & strBuf );

private:
	CALL_MAP	m_clsMap;
	CSipMutex	m_clsMutex;
};

extern CCallMap gclsCallMap;
extern CCallMap gclsTransCallMap;

#endif
