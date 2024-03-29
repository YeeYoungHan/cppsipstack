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

#ifndef _RTP_MAP_H_
#define _RTP_MAP_H_

#include "SipPlatformDefine.h"
#include <string>
#include <map>
#include "SipMutex.h"
#include "PacketHeader.h"
#include "StringUtility.h"

/**
 * @ingroup SipCallDump
 * @brief RTP 세션 정보 저장 클래스
 */
class CRtpInfo
{
public:
	CRtpInfo();

	std::string m_strCallId;
	time_t			m_iRecvTime;
	uint64_t		m_iRtpCount;
};

// key = IP:Port
typedef std::map< std::string, CRtpInfo > RTP_MAP;

/**
 * @ingroup SipCallDump
 * @brief RTP 세션 정보 저장 자료구조
 */
class CRtpMap
{
public:
	CRtpMap();
	~CRtpMap();

	bool Insert( struct pcap_pkthdr * psttHeader, const u_char * pszData, Ip4Header * psttIpHeader, UdpHeader * psttUdpHeader );

	bool Insert( const char * pszIp, int iPort, const char * pszCallId );
	bool Delete( const char * pszIp, int iPort );
	bool Select( const char * pszIp, int iPort, CRtpInfo & clsInfo );

	void SelectTimeout( STRING_LIST & clsCallIdList );

private:
	void GetKey( const char * pszIp, int iPort, std::string & strKey );
	void GetKey( uint32_t iIp, uint16_t sPort, std::string & strKey );

	RTP_MAP		m_clsMap;
	CSipMutex m_clsMutex;
};

extern CRtpMap gclsRtpMap;

#endif
