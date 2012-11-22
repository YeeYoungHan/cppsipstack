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

#ifndef _SIP_UDP_H_
#define _SIP_UDP_H_

#include <string>

#ifdef WIN32

#include <winsock2.h>
#include <stdlib.h>
#include <io.h>

typedef int socklen_t;
typedef SOCKET Socket;

inline int GetError() { return WSAGetLastError(); }
int poll( struct pollfd *fds, unsigned int nfds, int timeout );

#else

#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>

typedef int Socket;
#define INVALID_SOCKET	-1
#define SOCKET_ERROR		-1

inline int closesocket( Socket fd ) { return close(fd); };
inline int GetError() { return errno; }

#endif

#include <errno.h>

Socket UdpSocket();
Socket UdpListen( unsigned short iPort, const char * pszIp );
bool UdpRecv( Socket iFd, char * pszBuf, int * piLen, unsigned int * piIp, unsigned short* piPort );
bool UdpRecv( Socket iFd, char * pszBuf, int * piLen, char * pszIp, int iIpSize, unsigned short* piPort );
bool UdpSend( Socket iFd, const char * pszBuf, int iBufLen, const char * pszIp, unsigned short iPort );
bool UdpSend( Socket iFd, const char * pszBuf, int iBufLen, unsigned int iIp, unsigned short iPort );
void TcpSetPollIn( struct pollfd & sttPollFd, Socket hSocket );
void InitNetwork();

bool GetLocalIp( std::string & strIp );

#endif
