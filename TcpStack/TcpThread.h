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

#ifndef _TCP_THREAD_H_
#define _TCP_THREAD_H_

#include "SipPlatformDefine.h"
#include "SipTcp.h"

class CTcpStack;

class CTcpNoPipeThreadArg
{
public:
	CTcpNoPipeThreadArg() : m_bClient(false)
	{}

	std::string	m_strIp;
	int			m_iPort;
	Socket	m_hSocket;
	CTcpStack * m_pclsStack;
	bool		m_bClient;
};

bool StartTcpClientThread( const char * pszIp, int iPort, CTcpStack * pclsStack );

THREAD_API TcpNoPipeThread( LPVOID lpParameter );
THREAD_API TcpPipeThread( LPVOID lpParameter );
THREAD_API TcpListenThread( LPVOID lpParameter );

bool IsTcpListenThreadRun();

#endif
