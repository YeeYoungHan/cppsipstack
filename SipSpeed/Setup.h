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

#pragma once

#include <string>
#include <map>

#define SETUP_FILENAME	"SipSpeed.ini"

#define ST_SIP_SERVER_IP		"sip_server_ip"
#define ST_SIP_SERVER_PORT	"sip_server_port"
#define ST_SIP_DOMAIN				"sip_domain"
#define ST_CALLER_ID				"caller_id"
#define ST_CALLER_PW				"caller_pw"
#define ST_CALLEE_ID				"callee_id"
#define ST_CALLEE_PW				"callee_pw"
#define ST_CALL_TOTAL_COUNT	"call_total_count"
#define ST_CALL_CONCURRENT_COUNT	"call_concurrent_count"

typedef std::map< std::string, std::string > SETUP_MAP;

/**
 * @ingroup SipClientMFC
 * @brief 설정 파일 저장/읽기 클래스
 */
class CSetup
{
public:
	CSetup();
	~CSetup();

	bool Get();
	bool Put();

	std::string m_strSipServerIp;
	int         m_iSipServerPort;
	std::string m_strSipDomain;
	std::string m_strCallerId;
	std::string m_strCallerPassWord;
	std::string m_strCalleeId;
	std::string m_strCalleePassWord;
	int					m_iCallTotalCount;
	int					m_iCallConcurrentCount;

private:
	bool GetFile();
	bool PutFile();

	int GetInt( const char * pszName, int iIndex, int iDefaultValue );
	int GetInt( const char * pszName, int iDefaultValue );
	bool GetString( const char * pszName, std::string & strValue );

	bool PutInt( const char * pszName, int iIndex, int iValue );
	bool PutInt( const char * pszName, int iValue );
	bool PutString( const char * pszName, const char * pszValue );

	SETUP_MAP	m_clsMap;
};

extern CSetup gclsSetup;
