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

#ifndef _CALL_MAP_H_
#define _CALL_MAP_H_

#include "SipUdp.h"
#include <map>
#include <string>
#include "SipMessage.h"
#include "SipMutex.h"

class CRtpInfo
{
public:
	CRtpInfo() : m_iAudioPort(0), m_iVideoPort(0)
	{}

	int		m_iAudioPort;
	int		m_iVideoPort;
};

class CCallInfo
{
public:
	CCallInfo() : m_hAudioRtp(INVALID_SOCKET), m_hAudioRtcp(INVALID_SOCKET), m_hVideoRtp(INVALID_SOCKET), m_hVideoRtcp(INVALID_SOCKET)
	{}

	void CloseSocket( Socket & hSocket )
	{
		if( hSocket != INVALID_SOCKET )
		{
			closesocket( hSocket );
			hSocket = INVALID_SOCKET;
		}
	}

	void CloseSocket()
	{
		CloseSocket( m_hAudioRtp );
		CloseSocket( m_hAudioRtcp );
		CloseSocket( m_hVideoRtp );
		CloseSocket( m_hVideoRtcp );
	}

	CRtpInfo	m_clsLocalRtp;
	CRtpInfo	m_clsRemoteRtp;

	std::string	m_strRoomId;

	Socket	m_hAudioRtp;
	Socket	m_hAudioRtcp;
	Socket	m_hVideoRtp;
	Socket	m_hVideoRtcp;
};

// key ´Â SIP Call-ID ÀÌ´Ù.
typedef std::map< std::string, CCallInfo > CALL_MAP;

class CCallMap
{
public:
	CCallMap();
	~CCallMap();

	bool Insert( CSipMessage * pclsMessage );
	bool Select( CSipMessage * pclsMessage, CCallInfo & clsCallInfo );
	bool Select( const char * pszKey, CCallInfo & clsCallInfo );
	bool Delete( CSipMessage * pclsMessage );

private:
	CSipMutex	m_clsMutex;
	CALL_MAP	m_clsMap;

	int		m_iUdpStartPort;
	int		m_iUdpStopPort;
	int		m_iUdpPort;

	void GetKey( CSipMessage * pclsMessage, std::string & strKey );

	bool CreatePort( CCallInfo & clsCallInfo );
	bool CreatePort( CCallInfo & clsCallInfo, int sStart, int sEnd );
	bool SetRemoteRtp( CCallInfo & clsCallInfo, CSipMessage * pclsMessage );
};

extern CCallMap gclsCallMap;

#endif
