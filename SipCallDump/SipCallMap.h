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

#ifndef _SIP_CALL_MAP_H_
#define _SIP_CALL_MAP_H_

#include "SipMessage.h"
#include "SdpMessage.h"
#include "SipMutex.h"
#include <map>
#include <pcap.h>

class CSipRtpInfo
{
public:
	CSipRtpInfo();

	std::string	m_strIp;
	uint16_t		m_iPort;
};

class CSipCallInfo
{
public:
	CSipCallInfo();

	CSipRtpInfo	m_clsFromRtp;
	CSipRtpInfo	m_clsToRtp;

	pcap_dumper_t * m_psttPcap;
	time_t	m_iInviteTime;
	time_t	m_iCancelTime;
	time_t	m_iStartTime;
	time_t	m_iByeTime;
};

// key = SIP Call-ID
typedef std::map< std::string, CSipCallInfo > SIP_CALL_MAP;

class CSipCallMap
{
public:
	CSipCallMap();
	~CSipCallMap();

	bool Insert( pcap_t * psttPcap, struct pcap_pkthdr * psttHeader, const u_char * pszData, const char * pszUdpBody, int iUdpBodyLen );
	bool Insert( const char * pszCallId, struct pcap_pkthdr * psttHeader, const u_char * pszData );
	bool Delete( const char * pszCallId );
	void DeleteAll( );

private:
	bool InsertInvite( pcap_t * psttPcap, struct pcap_pkthdr * psttHeader, const u_char * pszData, CSipMessage * pclsMessage, CSdpMessage * pclsSdp );
	bool InsertInviteResponse( struct pcap_pkthdr * psttHeader, const	u_char * pszData, CSipMessage * pclsMessage, CSdpMessage * pclsSdp );
	
	bool Insert( struct pcap_pkthdr * psttHeader, const u_char * pszData, CSipMessage * pclsMessage );
	void Erase( SIP_CALL_MAP::iterator & itMap );

	SIP_CALL_MAP	m_clsMap;
	CSipMutex			m_clsMutex;
};

extern CSipCallMap gclsCallMap;

#endif
