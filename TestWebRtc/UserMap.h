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

#include "RtpThread.h"
#include "SipMutex.h"
#include <string>
#include <map>

/**
 * @ingroup TestWebRtc
 * @brief 사용자 정보 저장 클래스
 */
class CUserInfo
{
public:
	CUserInfo();

	std::string	m_strPassWord;
	std::string	m_strSipServerIp;

	std::string m_strIp;
	int					m_iPort;

	std::string m_strUserAgent;

	CRtpThreadArg * m_pclsRtpArg;
};

// key = user id
typedef std::map< std::string, CUserInfo > USER_MAP;

// key = client ip + port
typedef std::map< std::string, std::string > USER_KEY_MAP;

/**
 * @ingroup TestWebRtc
 * @brief 사용자 정보 저장 자료구조 클래스
 */
class CUserMap
{
public:
	CUserMap();
	~CUserMap();

	bool Insert( const char * pszUserId, const char * pszPassWord, const char * pszSipServerIp, const char * pszIp, int iPort, const char * pszUserAgent );
	bool Update( const char * pszUserId, CRtpThreadArg * pclsRtpArg, bool bSetNull );
	bool Delete( const char * pszUserId );
	
	bool Select( const char * pszUserId, CUserInfo & clsUserInfo );
	bool SelectUserId( const char * pszIp, int iPort, std::string & strUserId );

private:
	USER_MAP			m_clsMap;
	USER_KEY_MAP	m_clsKeyMap;
	CSipMutex			m_clsMutex;

	void GetKey( const char * pszIp, int iPort, std::string & strKey );
};

extern CUserMap gclsUserMap;

#endif
