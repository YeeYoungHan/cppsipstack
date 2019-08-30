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

#ifndef _TCP_MAP_H_
#define _TCP_MAP_H_

#include "SipPlatformDefine.h"
#include <string>
#include <list>
#include <map>
#include "SipMutex.h"
#include "PacketHeader.h"
#include "pcap.h"

class CTcpPacket
{
public:
	CTcpPacket();
	~CTcpPacket();

	static CTcpPacket * Create( struct pcap_pkthdr * psttHeader, const u_char * pszData, uint32_t iSeq, int iBodyPos, int iBodyLen );

	struct pcap_pkthdr m_sttHeader;
	u_char  * m_pszData;
	uint32_t	m_iSeq;
	int				m_iBodyPos;
	int				m_iBodyLen;
};

typedef std::list< CTcpPacket * > TCP_LIST;

class CTcpInfo
{
public:
	bool Insert( struct pcap_pkthdr * psttHeader, const u_char * pszData, TcpHeader * psttTcpHeader, int iBodyPos, int iBodyLen );

private:
	void CheckSip( );

	TCP_LIST m_clsTcpList;
};

typedef std::map< std::string, CTcpInfo > TCP_MAP;

class CTcpMap
{
public:
	CTcpMap();
	~CTcpMap();

	bool Insert( pcap_t * psttPcap, struct pcap_pkthdr * psttHeader, const u_char * pszData, Ip4Header * psttIpHeader, TcpHeader * psttTcpHeader, int iBodyPos, int iBodyLen );
	bool Update( pcap_t * psttPcap, struct pcap_pkthdr * psttHeader, const u_char * pszData, Ip4Header * psttIpHeader, TcpHeader * psttTcpHeader, int iBodyPos, int iBodyLen );

private:
	void GetKey( Ip4Header * psttIp4Header, TcpHeader * psttTcpHeader, std::string & strKey );

	TCP_MAP		m_clsMap;
	CSipMutex	m_clsMutex;
};

extern CTcpMap gclsTcpMap;

#endif
