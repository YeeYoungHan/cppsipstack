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

#ifndef _IP_FRAGMENT_MAP_H_
#define _IP_FRAGMENT_MAP_H_

#include "PacketHeader.h"
#include "SipMutex.h"
#include <string>
#include <list>
#include <map>

/**
 * @ingroup SipCallDump
 * @brief IP Header 를 포함한 패킷을 저장하는 클래스
 */
class CIpPacket
{
public:
	CIpPacket();
	~CIpPacket();

	u_char		* m_pszPacket;
	int				m_iPacketLen;
};

/**
 * @ingroup SipCallDump
 * @brief 하나의 IP Fragment 된 패킷을 저장하는 클래스
 */
class CIpFragmentData
{
public:
	CIpFragmentData();
	~CIpFragmentData();

	bool			m_bMoreFragment;
	uint16_t	m_sFragmentOffset;
	u_char	* m_pszIpBody;
	int				m_iIpBodyLen;
};

typedef std::list< CIpFragmentData * > IP_FRAGMENT_DATA_LIST;

/**
 * @ingroup SipCallDump
 * @brief ID 가 일치하는 IP Fragment 된 패킷을 저장하는 클래스
 */
class CIpFragmentInfo
{
public:
	CIpFragmentInfo();

	bool Insert( Ip4Header * psttIp4Header, char * pszIpBody, int iIpBodyLen, bool & bEnd );
	bool GetPacket( CIpPacket * pclsPacket );

	IP_FRAGMENT_DATA_LIST m_clsList;
	time_t		m_iTime;
	bool			m_bRecvFragmentEnd;
	Ip4Header m_sttIpHeader;
};

typedef std::map< std::string, CIpFragmentInfo * > IP_FRAGMENT_MAP;

/**
 * @ingroup SipCallDump
 * @brief IP Fragment 된 패킷을 IP 및 ID 를 기준으로 저장하는 자료구조
 */
class CIpFragmentMap
{
public:
	CIpFragmentMap();
	~CIpFragmentMap();

	bool Insert( Ip4Header * psttIp4Header, char * pszIpBody, int iIpBodyLen, bool & bEnd );
	bool Delete( Ip4Header * psttIp4Header, CIpPacket * pclsPacket );

	void DeleteTimeout();

private:
	void GetKey( Ip4Header * psttIp4Header, std::string & strKey );

	IP_FRAGMENT_MAP m_clsMap;
	CSipMutex m_clsMutex;
};

extern CIpFragmentMap gclsIpFragmentMap;

#endif
