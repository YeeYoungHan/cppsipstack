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
extern std::string gstrUserFileName;

CUserMap::CUserMap()
{
}

CUserMap::~CUserMap()
{
}

/**
 * @ingroup McuControlServer
 * @brief 로그인된 클라이언트 정보를 저장한다.
 * @param pclsMessage SIP REGISTER 메시지
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CUserMap::Insert( CSipMessage * pclsMessage, CSipFrom * pclsContact )
{
	CUserInfo			clsInfo;
	std::string		strUserId;
	USER_MAP::iterator	itMap;

	strUserId = pclsMessage->m_clsFrom.m_clsUri.m_strUser;
	if( strUserId.empty() ) return false;

	if( pclsMessage->GetTopViaIpPort( clsInfo.m_strIp, clsInfo.m_iPort ) == false ) return false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strUserId );
	if( itMap == m_clsMap.end() )
	{
		m_clsMap.insert( USER_MAP::value_type( strUserId, clsInfo ) );
	}
	else
	{
		itMap->second = clsInfo;
	}
	m_clsMutex.release();

	if( pclsContact )
	{
		pclsContact->m_clsUri.m_strProtocol = SIP_PROTOCOL;
		pclsContact->m_clsUri.m_strUser = strUserId;
		pclsContact->m_clsUri.m_strHost = clsInfo.m_strIp;
		pclsContact->m_clsUri.m_iPort = clsInfo.m_iPort;

		if( pclsMessage->m_iExpires > 0 )
		{
			char	szTemp[21];

			snprintf( szTemp, sizeof(szTemp), "%d", pclsMessage->m_iExpires );
			pclsContact->InsertParam( "expires", szTemp );
		}
	}

	// 사용자가 로그인할 때마다 파일로 저장한다.
	SaveFile( gstrUserFileName.c_str() );

	return true;
}

/**
 * @ingroup McuControlServer
 * @brief 사용자 ID 에 해당하는 정보를 검색한다.
 * @param pszUserId 사용자 ID
 * @param clsInfo		사용자 정보를 저장할 변수
 * @returns 사용자 ID 가 존재하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CUserMap::Select( const char * pszUserId, CUserInfo & clsInfo )
{
	bool bRes = false;
	USER_MAP::iterator	itMap;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszUserId );
	if( itMap != m_clsMap.end() )
	{
		clsInfo = itMap->second;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}


/**
 * @ingroup McuControlServer
 * @brief 사용자 정보를 파일에 저장한다.
 * @param pszFileName 사용자 정보를 저장할 파일 이름
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CUserMap::SaveFile( const char * pszFileName )
{
	FILE * fd = fopen( pszFileName, "wb" );
	if( fd == NULL )
	{
		return false;
	}

	USER_MAP::iterator	itMap;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		CUserInfoFile clsInfo;

		snprintf( clsInfo.m_szId, sizeof(clsInfo.m_szId), "%s", itMap->first.c_str() );
		snprintf( clsInfo.m_szIp, sizeof(clsInfo.m_szIp), "%s", itMap->second.m_strIp.c_str() );
		clsInfo.m_iPort = itMap->second.m_iPort;

		fwrite( &clsInfo, sizeof(clsInfo), 1, fd );
	}
	m_clsMutex.release();

	fclose( fd );

	return true;
}

/**
 * @ingroup McuControlServer
 * @brief 사용자 정보를 파일에서 읽는다.
 * @param pszFileName 사용자 정보가 저장된 파일 이름
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CUserMap::ReadFile( const char * pszFileName )
{
	FILE * fd = fopen( pszFileName, "rb" );
	if( fd == NULL )
	{
		return false;
	}

	USER_MAP::iterator	itMap;
	size_t n;

	while( 1 )
	{
		CUserInfoFile clsInfo;
		CUserInfo			clsUserInfo;

		n = fread( &clsInfo, sizeof(clsInfo), 1, fd );
		if( n != 1 ) break;

		clsUserInfo.m_strIp = clsInfo.m_szIp;
		clsUserInfo.m_iPort = clsInfo.m_iPort;

		m_clsMutex.acquire();
		itMap = m_clsMap.find( clsInfo.m_szId );
		if( itMap == m_clsMap.end() )
		{
			m_clsMap.insert( USER_MAP::value_type( clsInfo.m_szId, clsUserInfo ) );
		}
		m_clsMutex.release();
	}

	fclose( fd );

	return true;
}

/**
 * @brief 사용자 정보를 stdout 으로 출력한다.
 */
void CUserMap::Print( )
{
	USER_MAP::iterator	itMap;

	printf( "UserList\n" );

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		printf( "[%s] (%s:%d)\n", itMap->first.c_str(), itMap->second.m_strIp.c_str(), itMap->second.m_iPort );
	}
	m_clsMutex.release();

	printf( "\n" );
}
