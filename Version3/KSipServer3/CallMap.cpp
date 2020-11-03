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

#include "SipServer.h"
#include "CallMap.h"
#include "RtpMap.h"
#include "MemoryDebug.h"

CCallMap gclsCallMap;
CCallMap gclsTransCallMap;

CCallInfo::CCallInfo() : m_bRecv(false), m_iPeerRtpPort(-1)
{
}

CCallMap::CCallMap()
{
}

CCallMap::~CCallMap()
{
}

/**
 * @ingroup KSipServer
 * @brief 통화 요청 Call-ID 와 전달된 통화 요청 Call-ID 를 자료구조에 저장한다.
 * @param pszRecvCallId 통화 요청 Call-ID
 * @param pszSendCallId 전달된 통화 요청 Call-ID
 * @param iStartRtpPort	생성된 RTP 포트에서 시작 포트 번호
 *											pszSendCallId 와 연동하는 RTP 포트 번호이다.
 * @returns true 를 리턴한다.
 */
bool CCallMap::Insert( const char * pszRecvCallId, const char * pszSendCallId, int iStartRtpPort )
{
	CALL_MAP::iterator	itMap;

	m_clsMutex.acquire();
	// INVITE 메시지를 수신한 Dialog 를 저장한다.
	itMap = m_clsMap.find( pszRecvCallId );
	if( itMap == m_clsMap.end() )
	{
		CCallInfo	clsCallInfo;

		clsCallInfo.m_strPeerCallId = pszSendCallId;
		clsCallInfo.m_bRecv = true;
		if( iStartRtpPort > 0 )
		{
			clsCallInfo.m_iPeerRtpPort = iStartRtpPort;
		}
		m_clsMap.insert( CALL_MAP::value_type( pszRecvCallId, clsCallInfo ) );
	}

	// INVITE 메시지를 전송한 Dialog 를 저장한다.
	itMap = m_clsMap.find( pszSendCallId );
	if( itMap == m_clsMap.end() )
	{
		CCallInfo	clsCallInfo;

		clsCallInfo.m_strPeerCallId = pszRecvCallId;
		clsCallInfo.m_bRecv = false;
		if( iStartRtpPort > 0 )
		{
			clsCallInfo.m_iPeerRtpPort = iStartRtpPort + 2;
		}
		m_clsMap.insert( CALL_MAP::value_type( pszSendCallId, clsCallInfo ) );
	}
	m_clsMutex.release();

	return true;
}

/**
 * @ingroup KSipServer
 * @brief SIP Call-ID 와 이와 연관된 통화 정보를 자료구조에 저장한다.
 * @param pszCallId		SIP Call-ID
 * @param clsCallInfo 통화 정보
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CCallMap::Insert( const char * pszCallId, CCallInfo & clsCallInfo )
{
	CALL_MAP::iterator	itMap;
	bool bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap == m_clsMap.end() )
	{
		m_clsMap.insert( CALL_MAP::value_type( pszCallId, clsCallInfo ) );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief 상대방 Call-ID 를 수정한다.
 * @param pszCallId			SIP Call-ID
 * @param pszPeerCallId 상대방 SIP Call-ID
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CCallMap::Update( const char * pszCallId, const char * pszPeerCallId )
{
	CALL_MAP::iterator	itMap;
	bool bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		itMap->second.m_strPeerCallId = pszPeerCallId;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief Call-ID 와 연결된 Call-ID 를 검색한다.
 * @param pszCallId SIP Call-ID
 * @param strCallId 연결된 SIP Call-ID
 * @returns 검색되면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CCallMap::Select( const char * pszCallId, std::string & strCallId )
{
	CALL_MAP::iterator	itMap;
	bool	bRes = false;

	strCallId.clear();

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		strCallId = itMap->second.m_strPeerCallId;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief Call-ID 에 대한 정보를 저장한다.
 * @param pszCallId SIP Call-ID
 * @param clsCallInfo Call-ID 정보를 저장할 객체
 * @returns 검색되면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CCallMap::Select( const char * pszCallId, CCallInfo & clsCallInfo )
{
	CALL_MAP::iterator	itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		clsCallInfo = itMap->second;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief Call-ID 에 대한 정보를 저장한다.
 * @param pszCallId SIP Call-ID
 * @returns 검색되면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CCallMap::Select( const char * pszCallId )
{
	CALL_MAP::iterator	itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @brief INVITE 메시지를 전송하고 통화 Ring 중인 Call ID 를 검색한다.
 * @param pszTo			SIP TO 아이디
 * @param strCallId SIP Call-ID 를 저장할 변수
 * @returns INVITE 메시지를 전송하고 통화 Ring 중인 Call ID 가 검색되면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CCallMap::SelectToRing( const char * pszTo, std::string & strCallId )
{
	CALL_MAP::iterator	itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		if( itMap->second.m_bRecv ) continue;
		if( gclsUserAgent.IsRingCall( itMap->first.c_str(), pszTo ) == false ) continue;

		strCallId = itMap->first;
		bRes = true;
		break;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief Call-ID 와 연결된 Call-ID 를 자료구조에서 삭제한다.
 * @param pszCallId SIP Call-ID
 * @param bStopPort	RTP 포트 사용을 중지시킬 것인가?
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CCallMap::Delete( const char * pszCallId, bool bStopPort )
{
	CALL_MAP::iterator	itMap;
	bool	bRes = false;
	std::string strCallId;
	int		iPort = -1;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		strCallId = itMap->second.m_strPeerCallId;
		if( itMap->second.m_bRecv )
		{
			iPort = itMap->second.m_iPeerRtpPort;
		}
		m_clsMap.erase( itMap );
		bRes = true;
	}

	if( bRes )
	{
		itMap = m_clsMap.find( strCallId );
		if( itMap != m_clsMap.end() )
		{
			if( itMap->second.m_bRecv )
			{
				iPort = itMap->second.m_iPeerRtpPort;
			}
			m_clsMap.erase( itMap );
		}
	}
	m_clsMutex.release();

	if( bStopPort && iPort > 0 )
	{
		gclsRtpMap.SetStop( iPort );
	}

	return bRes;
}

bool CCallMap::DeleteOne( const char * pszCallId )
{
	CALL_MAP::iterator	itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszCallId );
	if( itMap != m_clsMap.end() )
	{
		m_clsMap.erase( itMap );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief 모든 통화를 종료시킨다.
 */
void CCallMap::StopCallAll()
{
	CALL_MAP::iterator	itMap;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		gclsUserAgent.StopCall( itMap->first.c_str() );
	}
	m_clsMutex.release();
}

/**
 * @ingroup KSipServer
 * @brief 통화 개수를 리턴한다.
 * @returns 통화 개수를 리턴한다.
 */
int CCallMap::GetCount()
{
	int iCount;

	m_clsMutex.acquire();
	iCount = (int)m_clsMap.size();
	m_clsMutex.release();

	return iCount;
}

/**
 * @ingroup KSipServer
 * @brief 통화 맵 모니터링용 문자열을 생성한다.
 * @param strBuf 통화 맵 모니터링용 문자열 저장 변수
 */
void CCallMap::GetString( CMonitorString & strBuf )
{
	CALL_MAP::iterator	itMap;

	strBuf.Clear();

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		strBuf.AddCol( itMap->first );
		strBuf.AddCol( itMap->second.m_strPeerCallId );
		strBuf.AddCol( itMap->second.m_bRecv ? "recv" : "" );
		strBuf.AddRow( itMap->second.m_iPeerRtpPort );
	}
	m_clsMutex.release();
}
