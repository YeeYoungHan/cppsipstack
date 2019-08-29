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

CTcpMap::CTcpMap()
{
}

CTcpMap::~CTcpMap()
{
}

bool CTcpMap::Insert( struct pcap_pkthdr * psttHeader, const u_char * pszData, Ip4Header * psttIpHeader, TcpHeader * psttTcpHeader, int iBodyPos, int iBodyLen )
{
	TCP_MAP::iterator itMap;
	std::string strKey;

	GetKey( psttIpHeader, psttTcpHeader, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap == m_clsMap.end() )
	{
		// Insert
	}
	else
	{
		// Update
	}
	m_clsMutex.release();

	return true;
}

bool CTcpMap::Update( struct pcap_pkthdr * psttHeader, const u_char * pszData, Ip4Header * psttIpHeader, TcpHeader * psttTcpHeader, int iBodyPos, int iBodyLen )
{

	return true;
}

void CTcpMap::GetKey( Ip4Header * psttIp4Header, TcpHeader * psttTcpHeader, std::string & strKey )
{
	char	szKey[40];

	snprintf( szKey, sizeof(szKey), "%X_%X_%X_%X", psttIp4Header->saddr, psttIp4Header->daddr, psttTcpHeader->sport, psttTcpHeader->dport );
	strKey = szKey;
}
