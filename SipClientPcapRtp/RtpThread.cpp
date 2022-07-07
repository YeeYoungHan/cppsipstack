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

#include "SipClientSetup.h"
#include "RtpThread.h"
#include "TimeUtility.h"
#include "ServerUtility.h"
#include "RtpHeader.h"
#include "PacketHeader.h"
#include "Log.h"
#include <pcap.h>

CRtpThread gclsRtpThread;
extern int giCodec;

#include "RtpThreadSend.hpp"
#include "RtpThreadRecv.hpp"

CRtpThread::CRtpThread() : m_hSocket(INVALID_SOCKET), m_iPort(0), m_bStopEvent(false), m_bSendThreadRun(false), m_bRecvThreadRun(false)
{
}

CRtpThread::~CRtpThread()
{
	Destroy( );
}

bool CRtpThread::Create( )
{
	if( m_hSocket != INVALID_SOCKET )
	{
		return true;
	}

	for( int i = 10000; i < 11000; i += 2 )
	{
		m_hSocket = UdpListen( i, NULL );
		if( m_hSocket != INVALID_SOCKET )
		{
			m_iPort = i;
			break;
		}
	}

	if( m_hSocket == INVALID_SOCKET )
	{
		return false;
	}

	return true;
}

bool CRtpThread::Destroy( )
{
	if( m_hSocket != INVALID_SOCKET )
	{
		closesocket( m_hSocket );
		m_hSocket = INVALID_SOCKET;
	}

	return true;
}

bool CRtpThread::Start( const char * pszDestIp, int iDestPort )
{
	if( m_hSocket == INVALID_SOCKET )
	{
		return false;
	}

	m_strDestIp = pszDestIp;
	m_iDestPort = iDestPort;

	if( m_bSendThreadRun || m_bRecvThreadRun )
	{
		return true;
	}

	if( StartThread( "RtpThreadSend", RtpThreadSend, NULL ) == false )
	{
		Stop();
		return false;
	}

	if( StartThread( "RtpThreadRecv", RtpThreadRecv, NULL ) == false )
	{
		Stop();
		return false;
	}

	return true;
}

bool CRtpThread::Stop( )
{
	m_bStopEvent = true;

	for( int i = 0; i < 100; ++i )
	{
		if( m_bSendThreadRun == false && m_bRecvThreadRun == false )
		{
			break;
		}

		MiliSleep(20);
	}

	m_bStopEvent = false;

	return true;
}
