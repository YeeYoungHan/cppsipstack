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

#include "SipPlatformDefine.h"
#include "UserMap.h"

CUserMap gclsUserMap;

CUserMap::CUserMap()
{
}

CUserMap::~CUserMap()
{
}

/**
 * @ingroup TestWebRtc
 * @brief 사용자 정보를 저장한다.
 * @param pszUserId 사용자 아이디
 * @param pszPassWord			SIP 서버 로그인 비밀번호
 * @param pszSipServerIp	SIP 서버 IP 주소
 * @param pszIp			WebSocket 클라이언트 IP 주소
 * @param iPort			WebSocket 클라이언트 포트 번호
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CUserMap::Insert( const char * pszUserId, const char * pszPassWord, const char * pszSipServerIp, const char * pszIp, int iPort )
{
	bool bRes = false;
	std::string strKey;
	USER_MAP::iterator itMap;
	USER_KEY_MAP::iterator itKeyMap;

	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	itKeyMap = m_clsKeyMap.find( strKey );
	if( itKeyMap == m_clsKeyMap.end() )
	{
		itMap = m_clsMap.find( pszUserId );
		if( itMap == m_clsMap.end() )
		{
			CUserInfo clsInfo;

			clsInfo.m_strPassWord = pszPassWord;
			clsInfo.m_strSipServerIp = pszSipServerIp;
			clsInfo.m_strIp = pszIp;
			clsInfo.m_iPort = iPort;

			m_clsMap.insert( USER_MAP::value_type( pszUserId, clsInfo ) );
			m_clsKeyMap.insert( USER_KEY_MAP::value_type( strKey, pszUserId ) );

			bRes = true;
		}
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TestWebRtc
 * @brief 사용자 정보를 검색한다.
 * @param pszUserId		사용자 아이디
 * @param clsUserInfo 사용자 정보 저장 객체
 * @returns 검색되면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CUserMap::Select( const char * pszUserId, CUserInfo & clsUserInfo )
{
	bool bRes = false;
	USER_MAP::iterator itMap;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszUserId );
	if( itMap != m_clsMap.end() )
	{
		clsUserInfo = itMap->second;

		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TestWebRtc
 * @brief WebSocket 클라이언트 정보로 사용자 아이디를 검색한다.
 * @param pszIp			WebSocket 클라이언트 IP 주소
 * @param iPort			WebSocket 클라이언트 포트 번호
 * @param strUserId 사용자 아이디
 * @returns 검색되면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CUserMap::SelectUserId( const char * pszIp, int iPort, std::string & strUserId )
{
	bool bRes = false;
	std::string strKey;
	USER_KEY_MAP::iterator itKeyMap;

	GetKey( pszIp, iPort, strKey );

	m_clsMutex.acquire();
	itKeyMap = m_clsKeyMap.find( strKey );
	if( itKeyMap != m_clsKeyMap.end() )
	{
		strUserId = itKeyMap->second;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TestWebRtc
 * @brief 사용자 아이디로 사용자 정보를 삭제한다.
 * @param pszUserId 사용자 아이디
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CUserMap::Delete( const char * pszUserId )
{
	bool bRes = false;
	std::string strKey;
	USER_MAP::iterator itMap;
	USER_KEY_MAP::iterator itKeyMap;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszUserId );
	if( itMap != m_clsMap.end() )
	{
		GetKey( itMap->second.m_strIp.c_str(), itMap->second.m_iPort, strKey );

		itKeyMap = m_clsKeyMap.find( strKey );
		if( itKeyMap != m_clsKeyMap.end() )
		{
			m_clsKeyMap.erase( itKeyMap );
		}

		m_clsMap.erase( itMap );

		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup TestWebRtc
 * @brief 자료구조에 사용할 키를 생성한다.
 * @param pszIp		WebSocket 클라이언트 IP 주소
 * @param iPort		WebSocket 클라이언트 포트 번호
 * @param strKey	자료구조 키
 */
void CUserMap::GetKey( const char * pszIp, int iPort, std::string & strKey )
{
	char szPort[11];

	snprintf( szPort, sizeof(szPort), "%d", iPort );

	strKey = pszIp;
	strKey.append( "_" );
	strKey.append( szPort );
}
