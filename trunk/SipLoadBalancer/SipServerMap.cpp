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
#include "SipServerMap.h"
#include "UserMap.h"
#include "MemoryDebug.h"

CSipServerMap gclsSipServerMap;

CSipServerInfo::CSipServerInfo() : m_iPort(5060), m_bUse(true), m_bDelete(false)
{
}

/**
 * @ingroup SipLoadBalancer
 * @brief 입력된 IP 주소 및 포트 번호가 SIP 서버 정보와 일치하는지 검사한다.
 * @param pszIp IP 주소
 * @param iPort 포트 번호
 * @returns 입력됮 정보가 SIP 서버 정보와 일치하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServerInfo::Equal( const char * pszIp, int iPort )
{
	if( !strcmp( m_strIp.c_str(), pszIp ) && m_iPort == iPort ) return true;

	return false;
}

CSipServerMap::CSipServerMap() : m_iIndex(0)
{
}

CSipServerMap::~CSipServerMap()
{
}

/**
 * @ingroup SipLoadBalancer
 * @brief SIP 서버 정보를 리스트에 추가한다.
 * @param pszIp IP 주소
 * @param iPort 포트 번호
 * @param bUse	사용 유무
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServerMap::Insert( const char * pszIp, int iPort, bool bUse )
{
	if( pszIp == NULL || strlen(pszIp) == 0 ) return false;
	if( iPort <= 0 || iPort > 65535 ) return false;

	// 이미 리스트에 존재하면 추가하지 않는다.
	if( Select( pszIp, iPort, bUse ) ) return true;

	CSipServerInfo clsInfo;

	clsInfo.m_strIp = pszIp;
	clsInfo.m_iPort = iPort;
	clsInfo.m_bUse = bUse;
	
	m_clsMutex.acquire();
	m_clsList.push_back( clsInfo );
	m_clsMutex.release();

	return true;
}

/**
 * @ingroup SipLoadBalancer
 * @brief 접속할 SIP 서버 정보를 가져온다.
 * @param clsInfo SIP 서버 정보를 저장하는 변수
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipServerMap::SelectNext( CSipServerInfo & clsInfo )
{
	bool bRes = false;

	m_clsMutex.acquire();
	int iCount = (int)m_clsList.size();
	if( iCount > 0 )
	{
		if( m_iIndex >= iCount )
		{
			m_iIndex = 0;
		}

		clsInfo = m_clsList[m_iIndex];
		++m_iIndex;

		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipLoadBalancer
 * @brief 입력된 IP 주소, 포트번호와 일치하는 SIP 서버 정보가 존재하는지 검사한다.
 * @param pszIp IP 주소
 * @param iPort 포트 번호
 * @returns SIP 서버가 검색되면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServerMap::Select( const char * pszIp, int iPort )
{
	SIP_SERVER_LIST::iterator itList;
	bool bFound = false;

	m_clsMutex.acquire();
	for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
	{
		if( itList->Equal( pszIp, iPort ) )
		{
			bFound = true;
			break;
		}
	}
	m_clsMutex.release();

	return bFound;
}

/**
 * @ingroup SipLoadBalancer
 * @brief 입력된 IP 주소, 포트번호와 일치하는 SIP 서버 정보가 존재하면 사용 유무 및 삭제 유무를 업데이트한다.
 * @param pszIp IP 주소
 * @param iPort 포트 번호
 * @param bUse	사용 유무
 * @returns SIP 서버가 검색되면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CSipServerMap::Select( const char * pszIp, int iPort, bool bUse )
{
	SIP_SERVER_LIST::iterator itList;
	bool bFound = false;

	m_clsMutex.acquire();
	for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
	{
		if( itList->Equal( pszIp, iPort ) )
		{
			itList->m_bUse = bUse;
			itList->m_bDelete = false;
			bFound = true;
			break;
		}
	}
	m_clsMutex.release();

	return bFound;
}

/**
 * @ingroup SipLoadBalancer
 * @brief 모든 SIP 서버 정보에 삭제 표시를 한다.
 */
void CSipServerMap::SetDeleteAll( )
{
	SIP_SERVER_LIST::iterator itList;

	m_clsMutex.acquire();
	for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
	{
		itList->m_bDelete = true;
	}
	m_clsMutex.release();
}

/**
 * @ingroup SipLoadBalancer
 * @brief 삭제 표시가 된 SIP 서버를 삭제한다.
 */
void CSipServerMap::DeleteIfSet( )
{
	SIP_SERVER_LIST::iterator itList, itNext;

	m_clsMutex.acquire();
	for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
	{
LOOP_START:
		if( itList->m_bDelete == false ) 
		{
			if( itList->m_bUse == false )
			{
				gclsUserMap.DeleteSipServer( itList->m_strIp.c_str(), itList->m_iPort );
			}
			continue;
		}

		itNext = itList;
		++itNext;

		gclsUserMap.DeleteSipServer( itList->m_strIp.c_str(), itList->m_iPort );
		m_clsList.erase( itList );

		if( itNext == m_clsList.end() ) break;

		itList = itNext;
		goto LOOP_START;
	}
	m_clsMutex.release();
}

/**
 * @ingroup SipLoadBalancer
 * @brief 자료구조 모니터링용 문자열을 생성한다. 
 * @param strBuf 자료구조 모니터링용 문자열 변수
 */
void CSipServerMap::GetString( CMonitorString & strBuf )
{
	SIP_SERVER_LIST::iterator itList;

	strBuf.Clear();

	m_clsMutex.acquire();
	for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
	{
		strBuf.AddCol( itList->m_strIp, itList->m_iPort );
		strBuf.AddCol( itList->m_bUse ? "use" : "no use" );
		strBuf.AddRow( itList->m_bDelete ? "del" : "" );
	}
	m_clsMutex.release();
}
