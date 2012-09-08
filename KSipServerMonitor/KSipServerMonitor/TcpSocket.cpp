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
#include "KSipServerMonitor.h"
#include "TcpSocket.h"

#include <string>

CTcpSocket gclsSocket;

// CTcpSocket

CTcpSocket::CTcpSocket()
{
}

CTcpSocket::~CTcpSocket()
{
}

// CTcpSocket 멤버 함수

void CTcpSocket::OnReceive( int nErrorCode )
{
	if( nErrorCode != 0 )
	{
		// QQQ: 프로그램을 종료한다.
		return;
	}

	int		iPacketLen, n, iRecvLen = 0;
	char	szPacket[8192];
	std::string	strPacket;

	n = Receive( &iPacketLen, sizeof(iPacketLen) );
	if( n != sizeof(iPacketLen) )
	{
		// QQQ: 프로그램을 종료한다.
		return;
	}

	iPacketLen = ntohl( iPacketLen );
	if( iPacketLen <= 0 )
	{
		// QQQ
		return;
	}

	while( iRecvLen < iPacketLen )
	{
		n = Receive( szPacket, iPacketLen - iRecvLen );
		if( n <= 0 )
		{
			// QQQ
			return;
		}

		strPacket.append( szPacket, n );
	}

	CAsyncSocket::OnReceive(nErrorCode);
}
