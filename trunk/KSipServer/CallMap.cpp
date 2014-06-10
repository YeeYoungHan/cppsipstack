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
 * @brief ��ȭ ��û Call-ID �� ���޵� ��ȭ ��û Call-ID �� �ڷᱸ���� �����Ѵ�.
 * @param pszRecvCallId ��ȭ ��û Call-ID
 * @param pszSendCallId ���޵� ��ȭ ��û Call-ID
 * @param iStartRtpPort	������ RTP ��Ʈ���� ���� ��Ʈ ��ȣ
 *											pszSendCallId �� �����ϴ� RTP ��Ʈ ��ȣ�̴�.
 * @returns true �� �����Ѵ�.
 */
bool CCallMap::Insert( const char * pszRecvCallId, const char * pszSendCallId, int iStartRtpPort )
{
	CALL_MAP::iterator	itMap;

	m_clsMutex.acquire();
	// INVITE �޽����� ������ Dialog �� �����Ѵ�.
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

	// INVITE �޽����� ������ Dialog �� �����Ѵ�.
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
 * @brief SIP Call-ID �� �̿� ������ ��ȭ ������ �ڷᱸ���� �����Ѵ�.
 * @param pszCallId		SIP Call-ID
 * @param clsCallInfo ��ȭ ����
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief ���� Call-ID �� �����Ѵ�.
 * @param pszCallId			SIP Call-ID
 * @param pszPeerCallId ���� SIP Call-ID
 * @returns �����ϸ� true �� �����ϰ� �����ϸ� false �� �����Ѵ�.
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
 * @brief Call-ID �� ����� Call-ID �� �˻��Ѵ�.
 * @param pszCallId SIP Call-ID
 * @param strCallId ����� SIP Call-ID
 * @returns �˻��Ǹ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief Call-ID �� ���� ������ �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @param clsCallInfo Call-ID ������ ������ ��ü
 * @returns �˻��Ǹ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief Call-ID �� ���� ������ �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @returns �˻��Ǹ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief INVITE �޽����� �����ϰ� ��ȭ Ring ���� Call ID �� �˻��Ѵ�.
 * @param pszTo			SIP TO ���̵�
 * @param strCallId SIP Call-ID �� ������ ����
 * @returns INVITE �޽����� �����ϰ� ��ȭ Ring ���� Call ID �� �˻��Ǹ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief Call-ID �� ����� Call-ID �� �ڷᱸ������ �����Ѵ�.
 * @param pszCallId SIP Call-ID
 * @param bStopPort	RTP ��Ʈ ����� ������ų ���ΰ�?
 * @returns �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
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
 * @brief ��� ��ȭ�� �����Ų��.
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
 * @brief ��ȭ ������ �����Ѵ�.
 * @returns ��ȭ ������ �����Ѵ�.
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
 * @brief ��ȭ �� ����͸��� ���ڿ��� �����Ѵ�.
 * @param strBuf ��ȭ �� ����͸��� ���ڿ� ���� ����
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
