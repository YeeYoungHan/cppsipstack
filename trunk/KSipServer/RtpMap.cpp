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

CRtpInfo::CRtpInfo( uint8_t iSocketCount ) : m_phSocket(NULL), m_piIp(NULL), m_psttIp(NULL), m_piPort(NULL)
	, m_iStartPort(0), m_bStop(false), m_iSocketCount(iSocketCount)
{
}

/**
 * @ingroup KSipServer
 * @brief �ڿ��� �����Ѵ�.
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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

	m_psttIp = new IN6_ADDR[m_iSocketCount];
	if( m_psttIp == NULL )
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
 * @brief �ڿ��� �����Ѵ�.
 */
void CRtpInfo::Close()
{
	CloseSocket();

	delete [] m_phSocket;
	m_phSocket = NULL;

	delete [] m_psttIp;
	m_psttIp = NULL;

	delete [] m_piIp;
	m_piIp = NULL;

	delete [] m_piPort;
	m_piPort = NULL;
}

/**
 * @ingroup KSipServer
 * @brief ������ �ݴ´�.
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
 * @brief SIP Ŭ���̾�Ʈ�� RTP IP/Port ������ �����Ѵ�.
 * @param iIndex	���� �ε���
 * @param iIp			SIP Ŭ���̾�Ʈ�� RTP IP �ּ�
 * @param sPort		SIP Ŭ���̾�Ʈ�� RTP ��Ʈ ��ȣ
 */
void CRtpInfo::SetIpPort( int iIndex, uint32_t iIp, uint16_t sPort )
{
	m_piIp[iIndex] = iIp;
	m_piPort[iIndex] = sPort;
}

/**
 * @ingroup KSipServer
 * @brief SIP Ŭ���̾�Ʈ�� RTP IPv6/Port ������ �����Ѵ�.
 * @param iIndex	���� �ε���
 * @param iIp			SIP Ŭ���̾�Ʈ�� RTP IP �ּ�
 * @param sPort		SIP Ŭ���̾�Ʈ�� RTP ��Ʈ ��ȣ
 */
void CRtpInfo::SetIpPort( int iIndex, IN6_ADDR * psttAddr, uint16_t sPort )
{
	memcpy( &m_psttIp[iIndex], psttAddr, sizeof(m_psttIp[iIndex]) );
	m_piPort[iIndex] = sPort;
}

/**
 * @ingroup KSipServer
 * @brief SIP Ŭ���̾�Ʈ�� RTP IP/Port ������ �ʱ�ȭ��Ų��.
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
 * @brief RTP ��Ŷ�� �����Ѵ�.
 * @param iIndex			���� �ε���
 * @param pszPacket		RTP ��Ŷ
 * @param iPacketLen	RTP ��Ŷ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CRtpInfo::Send( int iIndex, char * pszPacket, int iPacketLen )
{
	if( gclsSetup.m_bIpv6 )
	{
		return UdpSend( m_phSocket[iIndex], pszPacket, iPacketLen, &m_psttIp[iIndex], m_piPort[iIndex] );
	}
	else
	{
		return UdpSend( m_phSocket[iIndex], pszPacket, iPacketLen, m_piIp[iIndex], m_piPort[iIndex] );
	}
}

CRtpMap::CRtpMap() : m_iStartPort(0)
{
}

CRtpMap::~CRtpMap()
{
}

/**
 * @ingroup KSipServer
 * @brief RTP relay �� ���ؼ� UDP ���ϵ��� �����Ѵ�.
 * @param iSocketCount ������ UDP ���� ����
 * @returns RTP ��Ʈ ��ȣ�� �����Ѵ�.
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
 * @brief RTP ��Ʈ�� ���� ������ �˻��Ѵ�.
 * @param iPort RTP ��Ʈ ��ȣ
 * @param ppclsRtpInfo RTP ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief RTP �����忡 ���� ����� �����Ѵ�.
 * @param iPort RTP ��Ʈ ��ȣ
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief RTP ������ �����Ѵ�.
 * @param iPort RTP ��Ʈ ��ȣ
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief SIP Ŭ���̾�Ʈ�� IP/Port ��ȣ�� reset �Ѵ�.
 * @param iPort RTP ��Ʈ ��ȣ
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief �ڷᱸ�� ����͸��� ���ڿ��� �����Ѵ�.
 * @param strBuf �ڷᱸ�� ����͸��� ���ڿ� ���� ����
 */
void CRtpMap::GetString( CMonitorString & strBuf )
{
	RTP_MAP::iterator	itMap;
	char	szTemp[51];
	int		i;
	uint32_t	iIp;

	strBuf.Clear();

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
		strBuf.AddCol( itMap->first );
		strBuf.AddCol( itMap->second.m_iStartPort );

		for( i = 0; i < itMap->second.m_iSocketCount; ++i )
		{
			iIp = itMap->second.m_piIp[i];

			snprintf( szTemp, sizeof(szTemp), "%d.%d.%d.%d:%d", (iIp)&0xFF, (iIp>>8)&0xFF, (iIp>>16)&0xFF, (iIp>>24)&0xFF, ntohs(itMap->second.m_piPort[i]) );
			strBuf.AddCol( szTemp );
		}

		strBuf.AddRow( itMap->second.m_bStop ? "stop" : "" );
	}
	m_clsMutex.release();
}

/**
 * @ingroup KSipServer
 * @brief RTP relay �� ����� UDP ���ϵ��� �����Ѵ�.
 * @param clsInfo RTP relay ���� ���� ��ü
 * @param iStart	UDP ���� ���� ���� ��Ʈ
 * @param iEnd		UDP ���� ���� ���� ��Ʈ
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
 */
bool CRtpMap::CreatePort( CRtpInfo & clsInfo, int iStart, int iEnd )
{
	for( int iPort = iStart; iPort < iEnd; iPort += clsInfo.m_iSocketCount )
	{
		bool bError = false;

		for( int i = 0; i < clsInfo.m_iSocketCount; ++i )
		{
			clsInfo.m_phSocket[i] = UdpListen( iPort + i, NULL, gclsSetup.m_bIpv6 );
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
