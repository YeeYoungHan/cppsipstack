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
#include "RtpMap.h"
#include "SipServerSetup.h"
#include "Log.h"
#include "KSipServer.h"

CRtpMap gclsRtpMap;

CRtpInfo::CRtpInfo() : m_iStartPort(0), m_bStop(false)
{
	for( int i = 0; i < RTP_INFO_SOCKET_COUNT; ++i )
	{
		m_arrSocket[i] = INVALID_SOCKET;
		m_arrIp[i] = 0;
		m_arrPort[i] = 0;
	}
}

/**
 * @ingroup KSipServer
 * @brief 소켓을 닫는다.
 */
void CRtpInfo::CloseSocket()
{
	for( int i = 0; i < RTP_INFO_SOCKET_COUNT; ++i )
	{
		if( m_arrSocket[i] != INVALID_SOCKET )
		{
			closesocket( m_arrSocket[i] );
			m_arrSocket[i] = INVALID_SOCKET;
		}
	}
}

/**
 * @ingroup KSipServer
 * @brief SIP 클라이언트의 RTP IP/Port 정보를 설정한다.
 * @param iIndex	소켓 인덱스
 * @param iIp			SIP 클라이언트의 RTP IP 주소
 * @param sPort		SIP 클라이언트의 RTP 포트 번호
 */
void CRtpInfo::SetIpPort( int iIndex, uint32_t iIp, uint16_t sPort )
{
	m_arrIp[iIndex] = iIp;
	m_arrPort[iIndex] = sPort;
}

/**
 * @ingroup KSipServer
 * @brief SIP 클라이언트의 RTP IP/Port 정보를 초기화시킨다.
 */
void CRtpInfo::ReSetIPPort( )
{
	for( int i = 0; i < RTP_INFO_SOCKET_COUNT; ++i )
	{
		m_arrIp[i] = 0;
		m_arrPort[i] = 0;
	}
}

/**
 * @ingroup KSipServer
 * @brief RTP 패킷을 전송한다.
 * @param iIndex			소켓 인덱스
 * @param pszPacket		RTP 패킷
 * @param iPacketLen	RTP 패킷 길이
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CRtpInfo::Send( int iIndex, char * pszPacket, int iPacketLen )
{
	return UdpSend( m_arrSocket[iIndex], pszPacket, iPacketLen, m_arrIp[iIndex], m_arrPort[iIndex] );
}

CRtpMap::CRtpMap() : m_iStartPort(0)
{
}

CRtpMap::~CRtpMap()
{
}

/**
 * @ingroup KSipServer
 * @brief RTP relay 를 위해서 UDP 소켓 4개를 생성한다.
 * @returns RTP 포트 번호를 리턴한다.
 */
int CRtpMap::CreatePort( )
{
	bool			bRes = false;
	CRtpInfo	clsInfo;

	m_clsMutex.acquire();
	if( m_iStartPort == 0 ) m_iStartPort = gclsSetup.m_iBeginRtpPort;

	if( CreatePort( clsInfo, m_iStartPort, gclsSetup.m_iEndRtpPort ) )
	{
		bRes = true;
	}
	else if( m_iStartPort > gclsSetup.m_iBeginRtpPort )
	{
		if( CreatePort( clsInfo, gclsSetup.m_iBeginRtpPort, m_iStartPort ) )
		{
			bRes = true;
		}
	}

	if( bRes )
	{
		m_iStartPort = clsInfo.m_iStartPort + RTP_INFO_SOCKET_COUNT;
		if( m_iStartPort > gclsSetup.m_iEndRtpPort ) m_iStartPort = gclsSetup.m_iBeginRtpPort;

		m_clsMap.insert( RTP_MAP::value_type( clsInfo.m_iStartPort, clsInfo ) );
	}
	m_clsMutex.release();

	if( bRes )
	{
		if( StartRtpThread( clsInfo.m_iStartPort ) )
		{
			CLog::Print( LOG_DEBUG, "Create RtpPort(%d) success", clsInfo.m_iStartPort );
			return clsInfo.m_iStartPort;
		}

		Delete( clsInfo.m_iStartPort );
	}

	CLog::Print( LOG_ERROR, "Create RtpPort error" );

	return -1;
}

/**
 * @ingroup KSipServer
 * @brief RTP 포트에 대한 정보를 검색한다.
 * @param iPort RTP 포트 번호
 * @param ppclsRtpInfo RTP 정보
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CRtpMap::Select( int iPort, CRtpInfo ** ppclsRtpInfo )
{
	RTP_MAP::iterator	itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( iPort );
	if( itMap != m_clsMap.end() )
	{
		*ppclsRtpInfo = &itMap->second;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief RTP 쓰레드에 중지 명령을 전달한다.
 * @param iPort RTP 포트 번호
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CRtpMap::SetStop( int iPort )
{
	RTP_MAP::iterator	itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( iPort );
	if( itMap != m_clsMap.end() )
	{
		itMap->second.m_bStop = true;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief RTP 소켓을 종료한다.
 * @param iPort RTP 포트 번호
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CRtpMap::Delete( int iPort )
{
	RTP_MAP::iterator	itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( iPort );
	if( itMap != m_clsMap.end() )
	{
		itMap->second.CloseSocket();
		m_clsMap.erase( itMap );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief SIP 클라이언트의 IP/Port 번호를 reset 한다.
 * @param iPort RTP 포트 번호
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CRtpMap::ReSetIpPort( int iPort )
{
	RTP_MAP::iterator	itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( iPort );
	if( itMap != m_clsMap.end() )
	{
		itMap->second.ReSetIPPort();
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup KSipServer
 * @brief 자료구조 모니터링용 문자열을 생성한다.
 * @param strBuf 자료구조 모니터링용 문자열 저장 변수
 */
void CRtpMap::GetString( std::string & strBuf )
{
	RTP_MAP::iterator	itMap;
	char	szTemp[51];
	int		i;
	uint32_t	iIp;

	strBuf.clear();

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		snprintf( szTemp, sizeof(szTemp), "%d", itMap->first );
		strBuf.append( szTemp );
		strBuf.append( MR_COL_SEP );

		snprintf( szTemp, sizeof(szTemp), "%d", itMap->second.m_iStartPort );
		strBuf.append( szTemp );
		strBuf.append( MR_COL_SEP );

		for( i = 0; i < RTP_INFO_SOCKET_COUNT; ++i )
		{
			iIp = itMap->second.m_arrIp[i];

			snprintf( szTemp, sizeof(szTemp), "%d.%d.%d.%d:%d", (iIp)&0xFF, (iIp>>8)&0xFF, (iIp>>16)&0xFF, (iIp>>24)&0xFF, ntohs(itMap->second.m_arrPort[i]) );
			strBuf.append( szTemp );
			strBuf.append( MR_COL_SEP );
		}

		if( itMap->second.m_bStop )
		{
			strBuf.append( "stop" );
		}
		strBuf.append( MR_ROW_SEP );
	}
	m_clsMutex.release();
}

/**
 * @ingroup KSipServer
 * @brief RTP relay 에 사용할 UDP 소켓들을 생성한다.
 * @param clsInfo RTP relay 정보 저장 객체
 * @param iStart	UDP 소켓 생성 시작 포트
 * @param iEnd		UDP 소켓 생성 종료 포트
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CRtpMap::CreatePort( CRtpInfo & clsInfo, int iStart, int iEnd )
{
	for( int iPort = iStart; iPort < iEnd; iPort += RTP_INFO_SOCKET_COUNT )
	{
		clsInfo.m_arrSocket[0] = UdpListen( iPort, NULL );
		if( clsInfo.m_arrSocket[0] == INVALID_SOCKET ) continue;

		clsInfo.m_arrSocket[1] = UdpListen( iPort + 1, NULL );
		if( clsInfo.m_arrSocket[1] == INVALID_SOCKET )
		{
			clsInfo.CloseSocket();
			continue;
		}

		clsInfo.m_arrSocket[2] = UdpListen( iPort + 2, NULL );
		if( clsInfo.m_arrSocket[2] == INVALID_SOCKET )
		{
			clsInfo.CloseSocket();
			continue;
		}

		clsInfo.m_arrSocket[3] =  UdpListen( iPort + 3, NULL );
		if( clsInfo.m_arrSocket[3] == INVALID_SOCKET )
		{
			clsInfo.CloseSocket();
			continue;
		}

		clsInfo.m_iStartPort = iPort;
		return true;
	}

	return false;
}
