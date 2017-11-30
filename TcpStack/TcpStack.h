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

#ifndef _TCP_STACK_H_
#define _TCP_STACK_H_

#include "TcpThreadList.h"
#include "TcpStackSetup.h"
#include "TcpClientMap.h"
#include "TcpSessionMap.h"
#include "TcpStackCallBack.h"
#include "TcpStackSetup.h"

/**
 * @ingroup TcpStack
 * @brief TCP listen / connect 및 수신/전송 클래스
 */
class CTcpStack
{
public:
	CTcpStack();
	~CTcpStack();

	bool Start( CTcpStackSetup * pclsSetup, ITcpStackCallBack * pclsCallBack );
	bool Stop( );

	bool Send( const char * pszIp, int iPort, const char * pszPacket, int iPacketLen, bool bConnectIfNoSession );
	bool Send( int iThreadIndex, int iSessionIndex, const char * pszPacket, int iPacketLen );
	bool SendAll( const char * pszPacket, int iPacketLen );
	bool SendAllExcept( const char * pszPacket, int iPacketLen, int iThreadIndex, int iSessionIndex );

	CTcpStackSetup m_clsSetup;
	CTcpThreadList	m_clsThreadList;
	CTcpClientMap		m_clsClientMap;
	CTcpSessionMap	m_clsSessionMap;

	ITcpStackCallBack * m_pclsCallBack;

	Socket m_hTcpListenFd;
	bool m_bStop;
};

#endif
