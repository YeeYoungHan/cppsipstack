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

#include "IpFragmentMap.h"
#include "TimeUtility.h"
#include "Log.h"
#include "MemoryDebug.h"

CIpFragmentMap gclsIpFragmentMap;

CIpPacket::CIpPacket() : m_pszPacket(NULL), m_iPacketLen(0)
{
}

CIpPacket::~CIpPacket()
{
	if( m_pszPacket )
	{
		free( m_pszPacket );
		m_pszPacket = NULL;
	}
}

CIpFragmentData::CIpFragmentData() : m_bMoreFragment(false), m_sFragmentOffset(0), m_pszIpBody(NULL), m_iIpBodyLen(0)
{
}

CIpFragmentData::~CIpFragmentData()
{
	if( m_pszIpBody )
	{
		free( m_pszIpBody );
		m_pszIpBody = NULL;
	}
}

CIpFragmentInfo::CIpFragmentInfo() : m_iTime(NULL), m_bRecvFragmentEnd(false)
{
}

/**
 * @ingroup SipCallDump
 * @brief fragment 된 패킷을 저장한다.
 * @param psttIp4Header IPv4 헤더
 * @param pszIpBody			IP body
 * @param iIpBodyLen		IP body len
 * @param bEnd					[out] 모든 fragment 패킷이 수신되었으면 true 가 저장된다.
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CIpFragmentInfo::Insert( Ip4Header * psttIp4Header, char * pszIpBody, int iIpBodyLen, bool & bEnd )
{
	uint16_t sFlags = ntohs(psttIp4Header->flags_fo);

	CIpFragmentData * pclsData = new CIpFragmentData();
	if( pclsData == NULL )
	{
		CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		return false;
	}

	pclsData->m_pszIpBody = (u_char *)malloc( iIpBodyLen );
	if( pclsData->m_pszIpBody == NULL )
	{
		CLog::Print( LOG_ERROR, "%s malloc error", __FUNCTION__ );
		delete pclsData;
		return false;
	}

	memcpy( pclsData->m_pszIpBody, pszIpBody, iIpBodyLen );
	pclsData->m_iIpBodyLen = iIpBodyLen;

	if( sFlags & 0x2000 )
	{
		pclsData->m_bMoreFragment = true;
	}
	else
	{
		m_bRecvFragmentEnd = true;
	}

	pclsData->m_sFragmentOffset = sFlags & 0x1FFF;

	if( pclsData->m_sFragmentOffset == 0 )
	{
		memcpy( &m_sttIpHeader, psttIp4Header, sizeof(m_sttIpHeader) );
	}

	if( m_clsList.empty() )
	{
		m_clsList.push_back( pclsData );
	}
	else
	{
		IP_FRAGMENT_DATA_LIST::iterator itList;
		bool bFound = false;

		for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
		{
			if( pclsData->m_sFragmentOffset == (*itList)->m_sFragmentOffset )
			{
				// 동일한 Fragment Offset 이면 저장하지 않는다.
				delete pclsData;
				bFound = true;
				break;
			}
			else if( pclsData->m_sFragmentOffset < (*itList)->m_sFragmentOffset )
			{
				m_clsList.insert( itList, pclsData );
				bFound = true;
				break;
			}
		}

		if( bFound == false )
		{
			m_clsList.push_back( pclsData );
		}
	}

	time( &m_iTime );

	if( m_bRecvFragmentEnd )
	{
		IP_FRAGMENT_DATA_LIST::iterator itList;
		int iWantOffset = 0;
		bool bError = false;

		for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
		{
			if( iWantOffset != (*itList)->m_sFragmentOffset )
			{
				bError = true;
				break;
			}

			iWantOffset += (*itList)->m_iIpBodyLen;
		}

		if( bError == false )
		{
			bEnd = true;
		}
	}

	return true;
}

/**
 * @ingroup SipCallDump
 * @brief fragment 된 패킷들을 합쳐서 하나의 IP 패킷으로 생성한다.
 * @param pclsPacket [out] 하나의 IP 패킷 저장 객체
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CIpFragmentInfo::GetPacket( CIpPacket * pclsPacket )
{
	IP_FRAGMENT_DATA_LIST::iterator itList;
	int iIpBodyLen = 0;

	for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
	{
		iIpBodyLen += (*itList)->m_iIpBodyLen;
	}

	pclsPacket->m_pszPacket = (u_char *)malloc( sizeof(m_sttIpHeader) + iIpBodyLen );
	if( pclsPacket->m_pszPacket == NULL )
	{
		CLog::Print( LOG_ERROR, "%s malloc error", __FUNCTION__ );
		return false;
	}

	memcpy( pclsPacket->m_pszPacket, &m_sttIpHeader, sizeof(m_sttIpHeader) );
	pclsPacket->m_iPacketLen = sizeof(m_sttIpHeader);

	for( itList = m_clsList.begin(); itList != m_clsList.end(); ++itList )
	{
		memcpy( pclsPacket->m_pszPacket + pclsPacket->m_iPacketLen, (*itList)->m_pszIpBody, (*itList)->m_iIpBodyLen );
		pclsPacket->m_iPacketLen += (*itList)->m_iIpBodyLen;
	}

	Ip4Header * psttIpHeader = (Ip4Header *)pclsPacket->m_pszPacket;
	psttIpHeader->flags_fo = 0;
	psttIpHeader->tlen = htons( pclsPacket->m_iPacketLen );

	return true;
}

CIpFragmentMap::CIpFragmentMap()
{
}

CIpFragmentMap::~CIpFragmentMap()
{
}

/**
 * @ingroup SipCallDump
 * @brief 자료구조에 fragment 된 패킷을 저장한다.
 * @param psttIp4Header IPv4 헤더
 * @param pszIpBody			IP body
 * @param iIpBodyLen		IP body len
 * @param bEnd					[out] 모든 fragment 패킷이 수신되었으면 true 가 저장된다.
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CIpFragmentMap::Insert( Ip4Header * psttIp4Header, char * pszIpBody, int iIpBodyLen, bool & bEnd )
{
	if( psttIp4Header->flags_fo == 0 ) return false;

	std::string strKey;
	IP_FRAGMENT_MAP::iterator itMap;

	bEnd = false;
	GetKey( psttIp4Header, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap == m_clsMap.end() )
	{
		CIpFragmentInfo * pclsInfo = new CIpFragmentInfo();
		if( pclsInfo == NULL )
		{
			CLog::Print( LOG_ERROR, "%s new error", __FUNCTION__ );
		}
		else
		{
			pclsInfo->Insert( psttIp4Header, pszIpBody, iIpBodyLen, bEnd );
		}
	}
	else
	{
		itMap->second->Insert( psttIp4Header, pszIpBody, iIpBodyLen, bEnd );
	}
	m_clsMutex.release();
	
	return true;
}

/**
 * @ingroup SipCallDump
 * @brief 자료구조에서 fragment 패킷 관련 정보를 삭제한다.
 * @param psttIp4Header IPv4 헤더
 * @param pclsPacket		[out] 삭제한 fragment 패킷들을 합쳐서 하나의 IP 패킷으로 저장된 객체
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CIpFragmentMap::Delete( Ip4Header * psttIp4Header, CIpPacket * pclsPacket )
{
	bool bRes = false;
	std::string strKey;
	IP_FRAGMENT_MAP::iterator itMap;

	GetKey( psttIp4Header, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap != m_clsMap.end() )
	{
		if( pclsPacket )
		{
			bRes = itMap->second->GetPacket( pclsPacket );
		}
		else
		{
			bRes = true;
		}

		m_clsMap.erase( itMap );
	}
	m_clsMutex.release();

	return bRes;
}

/**
 * @ingroup SipCallDump
 * @brief 60초 이상 지난 fragment 패킷들을 삭제한다.
 */
void CIpFragmentMap::DeleteTimeout()
{
	IP_FRAGMENT_MAP::iterator itMap, itNext;
	time_t iTime;

	time( &iTime );
	iTime -= 60;

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
LOOP_START:
		if( iTime > itMap->second->m_iTime )
		{
			itNext = itMap;
			++itNext;
			delete itMap->second;
			m_clsMap.erase( itMap );

			if( itNext == m_clsMap.end() ) break;
			itMap = itNext;
			goto LOOP_START;
		}
	}
	m_clsMutex.release();
}

/**
 * @ingroup SipCallDump
 * @brief 키를 생성한다.
 * @param psttIp4Header IPv4 헤더
 * @param strKey				키 저장 변수
 */
void CIpFragmentMap::GetKey( Ip4Header * psttIp4Header, std::string & strKey )
{
	char	szKey[22];

	snprintf( szKey, sizeof(szKey), "%X_%X_%X", psttIp4Header->saddr, psttIp4Header->daddr, psttIp4Header->identification );
	strKey = szKey;
}
