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

#ifndef _SIP_TCP_H_
#define _SIP_TCP_H_

#include "SipUdp.h"

bool GetIpByName( const char * szHostName, char * szIp, int iLen );
Socket TcpConnect( const char * pszIp, int iPort, int iTimeout = 0 );
int TcpSend( Socket fd, const char * szBuf, int iBufLen );
int TcpRecv( Socket fd, char * szBuf, int iBufLen, int iSecond );
int TcpRecvSize( Socket fd, char * szBuf, int iBufLen, int iSecond );
Socket TcpListen( int iPort, int iListenQ, const char * pszIp = NULL, bool bIpv6 = false );
Socket TcpAccept( Socket hListenFd, char * pszIp, int iIpSize, int * piPort, bool bIpv6 = false );
bool GetLocalIpPort( Socket hSocket, std::string & strIp, int & iPort );

#ifdef WIN32
int pipe( Socket filedes[2] );
#endif

#endif
