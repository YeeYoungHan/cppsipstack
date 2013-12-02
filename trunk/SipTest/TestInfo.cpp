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

#include "stdafx.h"
#include "TestInfo.h"

CTestInfo gclsTestInfo;

CTestRtpInfo::CTestRtpInfo() : m_iPort(0), m_hSocket(INVALID_SOCKET), m_iRecvCount(0)
{
}

bool CTestRtpInfo::Create( int iPort )
{
	m_hSocket = UdpListen( iPort, NULL );
	if( m_hSocket == INVALID_SOCKET ) return false;

	m_iPort = iPort;

	return true;
}

void CTestRtpInfo::Close( )
{
	if( m_hSocket != INVALID_SOCKET )
	{
		closesocket( m_hSocket );
		m_hSocket = INVALID_SOCKET;
	}

	m_iPort = 0;
}

void CTestRtpInfo::Clear( )
{
	m_iRecvCount = 0;
}

bool CTestInfo::CreateRtp( )
{
	bool bRes = false;

	for( int iPort = 4000; iPort < 5000; ++ iPort )
	{
		if( m_clsCallerRtp.Create( iPort ) ) 
		{
			bRes = true;
			break;
		}
	}

	if( bRes == false ) return false;

	bRes = false;

	for( int iPort = 5000; iPort < 6000; ++ iPort )
	{
		if( m_clsCalleeRtp.Create( iPort ) ) 
		{
			bRes = true;
			break;
		}
	}

	if( bRes == false )
	{
		
	}

	return bRes;
}

void CTestInfo::CloseRtp( )
{
	m_clsCallerRtp.Close();
	m_clsCalleeRtp.Close();
}

void CTestInfo::ClearRtp( )
{
	m_clsCallerRtp.Clear();
	m_clsCalleeRtp.Clear();
}
