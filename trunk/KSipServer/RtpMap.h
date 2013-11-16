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

#include "SipParserDefine.h"
#include "SipUdp.h"
#include "SipMutex.h"
#include <map>

#define RTP_INFO_SOCKET_COUNT	4

/**
 * @ingroup KSipServer
 * @brief RTP relay 를 위한 RTP 소켓 정보 저장 클래스
 */
class CRtpInfo
{
public:
	CRtpInfo();

	Socket		m_arrSocket[RTP_INFO_SOCKET_COUNT];
	uint32_t	m_arrIp[RTP_INFO_SOCKET_COUNT];
	uint16_t	m_arrPort[RTP_INFO_SOCKET_COUNT];

	int			m_iStartPort;
	bool		m_bStop;

	void CloseSocket();
	void SetIpPort( int iIndex, uint32_t iIp, uint16_t sPort );
	void ReSetIPPort( );
	bool Send( int iIndex, char * pszPacket, int iPacketLen );
};

typedef std::map< int, CRtpInfo > RTP_MAP;

/**
 * @ingroup KSipServer
 * @brief RTP relay 를 위한 RTP 맵 자료구조 저장 클래스
 */
class CRtpMap
{
public:
	CRtpMap();
	~CRtpMap();

	int CreatePort( );

	bool Select( int iPort, CRtpInfo ** ppclsRtpInfo );
	bool SetStop( int iPort );
	bool Delete( int iPort );
	bool ReSetIpPort( int iPort );

	void GetString( std::string & strBuf );

private:
	RTP_MAP		m_clsMap;
	CSipMutex	m_clsMutex;

	int				m_iStartPort;

	bool CreatePort( CRtpInfo & clsInfo, int iStart, int iEnd );
};

extern CRtpMap gclsRtpMap;

#endif
