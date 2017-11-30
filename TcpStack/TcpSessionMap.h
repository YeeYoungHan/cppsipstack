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

#ifndef _TCP_SESSION_MAP_H_
#define _TCP_SESSION_MAP_H_

#include "TcpSessionList.h"
#include <map>

typedef std::map< std::string, CTcpSessionInfo * > TCP_SESSION_MAP;

/**
 * @ingroup TcpStack
 * @brief TCP 세션 정보를 저장하는 클래스 - m_bUseThreadPipe 가 false 일 때에 사용되는 클래스
 */
class CTcpSessionMap
{
public:
	CTcpSessionMap();
	~CTcpSessionMap();

	bool Insert( const char * pszIp, int iPort, CTcpSessionInfo * pclsSessionInfo );
	bool Delete( const char * pszIp, int iPort );
	int GetCount( );

	bool Send( const char * pszIp, int iPort, const char * pszPacket, int iPacketLen );
	bool Send( int iThreadIndex, const char * pszPacket, int iPacketLen );
	bool SendAll( const char * pszPacket, int iPacketLen, ITcpStackCallBack * pclsCallBack );
	bool SendAllExcept( const char * pszPacket, int iPacketLen, ITcpStackCallBack * pclsCallBack, int iThreadIndex, int iSessionIndex );

private:
	void GetKey( const char * pszIp, int iPort, std::string & strKey );
	int GetThreadIndex();
	bool SelectThreadIndex( int iThreadIndex );

	TCP_SESSION_MAP m_clsMap;
	CSipMutex m_clsMutex;
	int m_iThreadIndex;
};

#endif
