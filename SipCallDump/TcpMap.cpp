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

#include "TcpMap.h"
#include "SipCallMap.h"
#include "SipMessage.h"
#include "MemoryDebug.h"

CTcpMap gclsTcpMap;

CTcpPacket::CTcpPacket() : m_pszData(NULL), m_iSeq(0), m_iBodyPos(0), m_iBodyLen(0)
{
}

CTcpPacket::~CTcpPacket()
{
	if( m_pszData )
	{
		free( m_pszData );
		m_pszData = NULL;
	}
}

CTcpPacket * CTcpPacket::Create( struct pcap_pkthdr * psttHeader, const u_char * pszData, uint32_t iSeq, int iBodyPos, int iBodyLen )
{
	CTcpPacket * pclsPacket = new CTcpPacket();
	if( pclsPacket == NULL )
	{
		return NULL;
	}

	pclsPacket->m_pszData = (u_char *)malloc( psttHeader->caplen );
	if( pclsPacket->m_pszData == NULL )
	{
		delete pclsPacket;
		return NULL;
	}

	memcpy( &pclsPacket->m_sttHeader, psttHeader, sizeof(pclsPacket->m_sttHeader) );
	memcpy( pclsPacket->m_pszData, pszData, psttHeader->caplen );
	pclsPacket->m_iBodyPos = iBodyPos;
	pclsPacket->m_iBodyLen = iBodyLen;
	pclsPacket->m_iSeq = iSeq;

	return pclsPacket;
}

CTcpInfo::CTcpInfo() : m_iTime(0)
{
}

bool CTcpInfo::Insert( pcap_t * psttPcap, struct pcap_pkthdr * psttHeader, const u_char * pszData, TcpHeader * psttTcpHeader, int iBodyPos, int iBodyLen )
{
	time( &m_iTime );

	uint32_t iSeq = ntohl( psttTcpHeader->seqnum );

	if( m_clsTcpList.empty() == false )
	{
		TCP_LIST::iterator itTL;

		for( itTL = m_clsTcpList.begin(); itTL != m_clsTcpList.end(); ++itTL )
		{
			if( (*itTL)->m_iSeq == iSeq )
			{
				// 동일한 SEQ 는 저장하지 않는다.
				return true;
			}
			else if( (*itTL)->m_iSeq > iSeq )
			{
				CTcpPacket * pclsPacket = CTcpPacket::Create( psttHeader, pszData, iSeq, iBodyPos, iBodyLen );
				if( pclsPacket == NULL )
				{
					return false;
				}

				m_clsTcpList.insert( itTL, pclsPacket );
				CheckSip( psttPcap );

				return true;
			}
		}
	}

	CTcpPacket * pclsPacket = CTcpPacket::Create( psttHeader, pszData, iSeq, iBodyPos, iBodyLen );
	if( pclsPacket == NULL )
	{
		return false;
	}

	m_clsTcpList.push_back( pclsPacket );
	CheckSip( psttPcap );

	return true;
}

void CTcpInfo::DeleteAll( )
{
	TCP_LIST::iterator itTL;

	for( itTL = m_clsTcpList.begin(); itTL != m_clsTcpList.end(); ++itTL )
	{
		delete (*itTL);
	}

	m_clsTcpList.clear();
}

void CTcpInfo::CheckSip( pcap_t * psttPcap )
{
	TCP_LIST::iterator itTL;
	std::string strData;
	uint32_t iNextSeq = 0;
	int iCount = 0, iPacketLen = 0;

	for( itTL = m_clsTcpList.begin(); itTL != m_clsTcpList.end(); ++itTL )
	{
		if( itTL == m_clsTcpList.begin() )
		{
			iNextSeq = (*itTL)->m_iSeq + (*itTL)->m_iBodyLen;
			strData.append( (char *)(*itTL)->m_pszData + (*itTL)->m_iBodyPos, (*itTL)->m_iBodyLen );
			iPacketLen = (*itTL)->m_iBodyPos + (*itTL)->m_iBodyLen;
			++iCount;
		}
		else if( (*itTL)->m_iSeq == iNextSeq )
		{
			iNextSeq = (*itTL)->m_iSeq + (*itTL)->m_iBodyLen;
			strData.append( (char *)(*itTL)->m_pszData + (*itTL)->m_iBodyPos, (*itTL)->m_iBodyLen );
			iPacketLen += (*itTL)->m_iBodyLen;
			++iCount;
		}
		else
		{
			break;
		}
	}

	if( strData.empty() == false )
	{
		CSipMessage clsMessage;

		if( clsMessage.Parse( strData.c_str(), strData.length() ) > 0 )
		{
			// N 개의 TCP 패킷을 하나의 패킷으로 저장한다.
			u_char * pszPacket = (u_char *)malloc( iPacketLen );
			if( pszPacket )
			{
				struct pcap_pkthdr sttHeader;
				int iPacketPos = 0;

				for( itTL = m_clsTcpList.begin(); itTL != m_clsTcpList.end(); ++itTL )
				{
					if( itTL == m_clsTcpList.begin() )
					{
						memcpy( &sttHeader, &(*itTL)->m_sttHeader, sizeof(sttHeader) );
						memcpy( pszPacket, (*itTL)->m_pszData, (*itTL)->m_iBodyPos + (*itTL)->m_iBodyLen );
						iPacketPos = (*itTL)->m_iBodyPos + (*itTL)->m_iBodyLen;
					}
					else
					{
						memcpy( pszPacket + iPacketPos, (*itTL)->m_pszData + (*itTL)->m_iBodyPos, (*itTL)->m_iBodyLen );
						iPacketPos += (*itTL)->m_iBodyLen;
					}
				}

				// pcap packet 헤더를 수정한다.
				sttHeader.caplen = iPacketLen;
				sttHeader.len = iPacketLen;

				// IP 헤더를 수정한다.
				int iIpPos = 14;

				if( pszPacket[12] == 0x81 )
				{
					iIpPos = 18;		// VLAN
				}

				Ip4Header * psttIp4Header = (Ip4Header *)( pszPacket + iIpPos );
				psttIp4Header->tlen = htons( iPacketLen - iIpPos );

				gclsCallMap.Insert( psttPcap, &sttHeader, pszPacket, &clsMessage );

				free( pszPacket );
			}

			TCP_LIST::iterator itNext;
			int iDelCount = 0;

			for( itTL = m_clsTcpList.begin(); itTL != m_clsTcpList.end(); )
			{
				itNext = itTL;
				++itNext;
				delete (*itTL);
				m_clsTcpList.erase( itTL );
				++iDelCount;

				if( iDelCount == iCount ) break;
				itTL = itNext;
			}
		}
	}
}

CTcpMap::CTcpMap()
{
}

CTcpMap::~CTcpMap()
{
}

bool CTcpMap::Insert( pcap_t * psttPcap, struct pcap_pkthdr * psttHeader, const u_char * pszData, Ip4Header * psttIpHeader, TcpHeader * psttTcpHeader, int iBodyPos, int iBodyLen )
{
	TCP_MAP::iterator itMap;
	std::string strKey;
	bool bRes = false;
	bool bFound = false;

	GetKey( psttIpHeader, psttTcpHeader, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap != m_clsMap.end() )
	{
		bFound = true;
	}
	m_clsMutex.release();

	if( bFound == false )
	{
		CSipMessage clsMessage;

		// 하나의 TCP 패킷에 SIP 메시지가 모두 존재하면 자료구조에 저장하지 않고 CallMap 에 저장한다.
		if( clsMessage.Parse( (const char *)(pszData + iBodyPos), iBodyLen ) > 0 )
		{
			return gclsCallMap.Insert( psttPcap, psttHeader, pszData, &clsMessage );
		}
	}

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap == m_clsMap.end() )
	{
		CTcpInfo clsInfo;

		m_clsMap.insert( TCP_MAP::value_type( strKey, clsInfo ) );
		itMap = m_clsMap.find( strKey );
		if( itMap != m_clsMap.end() )
		{
			bRes = itMap->second.Insert( psttPcap, psttHeader, pszData, psttTcpHeader, iBodyPos, iBodyLen );
		}
	}
	else
	{
		bRes = itMap->second.Insert( psttPcap, psttHeader, pszData, psttTcpHeader, iBodyPos, iBodyLen );
	}
	m_clsMutex.release();

	return bRes;
}

bool CTcpMap::Update( pcap_t * psttPcap, struct pcap_pkthdr * psttHeader, const u_char * pszData, Ip4Header * psttIpHeader, TcpHeader * psttTcpHeader, int iBodyPos, int iBodyLen )
{
	TCP_MAP::iterator itMap;
	std::string strKey;
	bool bRes = false;

	GetKey( psttIpHeader, psttTcpHeader, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap != m_clsMap.end() )
	{
		bRes = itMap->second.Insert( psttPcap, psttHeader, pszData, psttTcpHeader, iBodyPos, iBodyLen );
	}
	m_clsMutex.release();

	return bRes;
}

void CTcpMap::DeleteTimeout( )
{
	TCP_MAP::iterator itMap, itNext;
	time_t iTime;

	time( &iTime );

	m_clsMutex.acquire();
	for( itMap = m_clsMap.begin(); itMap != m_clsMap.end(); ++itMap )
	{
LOOP_START:
		if( ( iTime - itMap->second.m_iTime ) >= 300 )
		{
			itNext = itMap;
			++itNext;
			itMap->second.DeleteAll();
			m_clsMap.erase( itMap );

			if( itNext == m_clsMap.end() ) break;
			itMap = itNext;
			goto LOOP_START;
		}
	}
	m_clsMutex.release();
}

void CTcpMap::GetKey( Ip4Header * psttIp4Header, TcpHeader * psttTcpHeader, std::string & strKey )
{
	char	szKey[40];

	snprintf( szKey, sizeof(szKey), "%X_%X_%X_%X", psttIp4Header->saddr, psttIp4Header->daddr, psttTcpHeader->sport, psttTcpHeader->dport );
	strKey = szKey;
}
