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

#include "SipStackDefine.h"
#include "UserMap.h"
#include "SipServerMap.h"

CUserMap gclsUserMap;

CUserInfo::CUserInfo() : m_iSipServerPort(5060), m_iPort(0), m_eTransport(E_SIP_UDP)
{
}

/**
 * @brief 입력된 SIP 서버 정보와 일치하는지 검사한다.
 * @param pszIp SIP 서버 IP 주소
 * @param iPort SIP 서버 포트 번호
 * @returns 일치하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CUserInfo::EqualSipServer( const char * pszIp, int iPort )
{
	if( !strcmp( m_strSipServerIp.c_str(), pszIp ) && m_iSipServerPort == iPort ) return true;

	return false;
}

CUserMap::CUserMap()
{
}

CUserMap::~CUserMap()
{
}

/**
 * @ingroup SipLoadBalancer
 * @brief SIP 메시지로 SIP 클라이언트 정보를 저장한다.
 * @param pclsRequest SIP 메시지
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CUserMap::Insert( CSipMessage * pclsRequest )
{
	bool								bRes = true;
	USER_MAP::iterator	itMap;
	std::string & strUserId = pclsRequest->m_clsFrom.m_clsUri.m_strUser;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strUserId );
	if( itMap == m_clsMap.end() )
	{
		CSipServerInfo clsSipServerInfo;

		if( gclsSipServerMap.SelectNext( clsSipServerInfo ) )
		{
			CUserInfo clsUserInfo;

			clsUserInfo.m_strSipServerIp = clsSipServerInfo.m_strIp;
			clsUserInfo.m_iSipServerPort = clsSipServerInfo.m_iPort;
			clsUserInfo.m_strIp = pclsRequest->m_strClientIp;
			clsUserInfo.m_iPort = pclsRequest->m_iClientPort;
			clsUserInfo.m_eTransport = pclsRequest->m_eTransport;

			m_clsMap.insert( USER_MAP::value_type( strUserId, clsUserInfo ) );
		}
		else
		{
			bRes = false;
		}
	}
	else
	{
		if( strcmp( pclsRequest->m_strClientIp.c_str(), itMap->second.m_strIp.c_str() ) )
		{
			itMap->second.m_strIp = pclsRequest->m_strClientIp;
		}

		if( pclsRequest->m_iClientPort != itMap->second.m_iPort )
		{
			itMap->second.m_iPort = pclsRequest->m_iClientPort;
		}
	}
	m_clsMutex.release();

	return bRes;
}


/**
 * @ingroup SipLoadBalancer
 * @brief SIP 메시지과 관련된 SIP 클라이언트 정보를 가져온다.
 * @param pclsRequest SIP 메시지
 * @param clsUserInfo SIP 클라이언트 정보 저장 변수
 * @returns 검색되면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CUserMap::Select( CSipMessage * pclsRequest, CUserInfo & clsUserInfo )
{
	bool								bRes = true;
	USER_MAP::iterator	itMap;
	std::string & strUserId = pclsRequest->m_clsTo.m_clsUri.m_strUser;

	if( pclsRequest->IsMethod( "REGISTER" ) )
	{
		strUserId = pclsRequest->m_clsFrom.m_clsUri.m_strUser;
	}

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strUserId );
	if( itMap == m_clsMap.end() )
	{
		bRes = false;
	}
	else
	{
		clsUserInfo = itMap->second;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipLoadBalancer
 * @brief SIP 서버의 IP 주소/포트 번호를 가지고 있는 SIP 클라이언트를 삭제한다.
 * @param pszIp SIP 서버 IP 주소
 * @param iPort SIP 서버 포트 번호
 */
void CUserMap::DeleteSipServer( const char * pszIp, int iPort )
{
	USER_MAP::iterator	itMap, itNext;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
LOOP_START:
		if( itMap->second.EqualSipServer( pszIp, iPort ) == false ) continue;

		itNext = itMap;
		++itNext;

		m_clsMap.erase( itMap );

		if( itNext == m_clsMap.end() ) break;
		itMap = itNext;

		goto LOOP_START;
	}
	m_clsMutex.release();
}

/**
 * @ingroup SipLoadBalancer
 * @brief 자료구조 모니터링용 문자열을 생성한다. 
 * @param strBuf 자료구조 모니터링용 문자열 변수
 */
void CUserMap::GetString( std::string & strBuf )
{
	USER_MAP::iterator	itMap;
	char	szTemp[51];

	strBuf.clear();

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		strBuf.append( itMap->first );
		strBuf.append( MR_COL_SEP );

		strBuf.append( itMap->second.m_strIp );
		snprintf( szTemp, sizeof(szTemp), ":%d", itMap->second.m_iPort );
		strBuf.append( szTemp );
		strBuf.append( MR_COL_SEP );

		strBuf.append( itMap->second.m_strSipServerIp );
		snprintf( szTemp, sizeof(szTemp), ":%d", itMap->second.m_iSipServerPort );
		strBuf.append( szTemp );
		strBuf.append( MR_ROW_SEP );
	}
	m_clsMutex.release();
}
