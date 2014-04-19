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
#include "MemoryDebug.h"

CRtpMap gclsRtpMap;

CRtpInfo::CRtpInfo( uint8_t iSocketCount ) : m_iStartPort(0), m_bStop(false), m_iSocketCount(iSocketCount)
{
	
}

/**
 * @ingroup KSipServer
 * @brief 자원을 생성한다.
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CRtpInfo::Create()
{
	m_phSocket = new Socket[m_iSocketCount];
	if( m_phSocket == NULL ) return false;

	m_piIp = new uint32_t[m_iSocketCount];
	if( m_piIp == NULL )
	{
		Close();
		return false;
	}

	m_piPort = new uint16_t[m_iSocketCount];
	if( m_piPort == NULL )
	{
		Close();
		return false;
	}

	for( uint8_t i = 0; i < m_iSocketCount; ++i )
	{
		m_phSocket[i] = INVALID_SOCKET;
		m_piIp[i] = 0;
		m_piPort[i] = 0;
	}

	return true;
}

/**
 * @ingroup KSipServer
 * @brief 자원을 해제한다.
 */
void CRtpInfo::Close()
{
	CloseSocket();

	delete [] m_phSocket;
	m_phSocket = NULL;

	delete [] m_piIp;
	m_piIp = NULL;

	delete [] m_piPort;
	m_piPort = NULL;
}

/**
 * @ingroup KSipServer
 * @brief 소켓을 닫는다.
 */
void CRtpInfo::CloseSocket()
{
	if( m_phSocket )
	{
		for( uint8_t i = 0; i < m_iSocketCount; ++i )
		{
			if( m_phSocket[i] != INVALID_SOCKET )
			{
				closesocket( m_phSocket[i] );
				m_phSocket[i] = INVALID_SOCKET;
			}
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
	m_piIp[iIndex] = iIp;
	m_piPort[iIndex] = sPort;
}

/**
 * @ingroup KSipServer
 * @brief SIP 클라이언트의 RTP IP/Port 정보를 초기화시킨다.
 */
void CRtpInfo::ReSetIPPort( )
{
	for( uint8_t i = 0; i < m_iSocketCount; ++i )
	{
		m_piIp[i] = 0;
		m_piPort[i] = 0;
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
	return UdpSend( m_phSocket[iIndex], pszPacket, iPacketLen, m_piIp[iIndex], m_piPort[iIndex] );
}

CRtpMap::CRtpMap() : m_iStartPort(0)
{
}

CRtpMap::~CRtpMap()
{
}

/**
 * @ingroup KSipServer
 * @brief RTP relay 를 위해서 UDP 소켓들을 생성한다.
 * @param iSocketCount 생성할 UDP 소켓 개수
 * @returns RTP 포트 번호를 리턴한다.
 */
int CRtpMap::CreatePort( int iSocketCount )
{
	bool			bRes = false;
	CRtpInfo	clsInfo( iSocketCount );

	if( clsInfo.Create() == false ) return -1;

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
		m_iStartPort = clsInfo.m_iStartPort + clsInfo.m_iSocketCount;
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
		itMap->second.Close();
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

		for( i = 0; i < itMap->second.m_iSocketCount; ++i )
		{
			iIp = itMap->second.m_piIp[i];

			snprintf( szTemp, sizeof(szTemp), "%d.%d.%d.%d:%d", (iIp)&0xFF, (iIp>>8)&0xFF, (iIp>>16)&0xFF, (iIp>>24)&0xFF, ntohs(itMap->second.m_piPort[i]) );
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
	for( int iPort = iStart; iPort < iEnd; iPort += clsInfo.m_iSocketCount )
	{
		bool bError = false;

		for( int i = 0; i < clsInfo.m_iSocketCount; ++i )
		{
			clsInfo.m_phSocket[i] = UdpListen( iPort + i, NULL );
			if( clsInfo.m_phSocket[i] == INVALID_SOCKET )
			{
				bError = true;
				clsInfo.CloseSocket();
				break;
			}
		}

		if( bError == false )
		{
			clsInfo.m_iStartPort = iPort;
			return true;
		}
	}

	return false;
}
