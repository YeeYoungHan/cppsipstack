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

#include "SipStackDefine.h"
#include <time.h>
#include <string.h>
#include "SipUdp.h"

/** UDP 소켓을 생성한다.
 *
 *	@return	성공하면 socket handle 을 리턴한다. 그렇지 않으면 INVALID_SOCKET 를 리턴한다.
 */
Socket UdpSocket()
{
	Socket iFd;
	
	iFd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if( iFd == INVALID_SOCKET )
	{
		return INVALID_SOCKET;
	}

	return iFd;
}

/** UDP listen 소켓을 생성한다. 
 *
 *	@param	iPort
 *	@param	pszIp
 *	@return	성공하면 socket handle 을 리턴한다. 그렇지 않으면 INVALID_SOCKET 를 리턴한다.
 */
Socket UdpListen( unsigned short iPort, const char * pszIp )
{
	if( iPort <= 0 )
	{
		return INVALID_SOCKET;
	}
	
	Socket iFd;
	
	iFd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if( iFd == INVALID_SOCKET )
	{
		return INVALID_SOCKET;
	}
	
	struct sockaddr_in addr;
	memset((char*) &(addr),0, sizeof((addr)));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(iPort);
	
	if( pszIp )
	{
	  addr.sin_addr.s_addr = inet_addr(pszIp);
	}
	else
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	
	if( bind( iFd,(struct sockaddr*)&addr, sizeof(addr)) != 0 )
	{
	  closesocket( iFd );
	  return INVALID_SOCKET;
	}
	
	return iFd;
}

/** UDP 메시지를 수신한다. */
bool UdpRecv( Socket iFd, char * pszBuf, int * piLen, char * pszIp, int iIpSize, unsigned short* piPort )
{
	if( iFd == INVALID_SOCKET ) return false;
	
	int iBufSize = *piLen;
	if( iBufSize <= 0 ) return false;
	
	struct sockaddr_in sttAddr;
	int iAddrSize = sizeof(sttAddr);
	
	*piLen = recvfrom( iFd, pszBuf, iBufSize, 0, (struct sockaddr *)&sttAddr, (socklen_t*)&iAddrSize );
	if( *piLen <= 0 )
	{
	  return false;
	}
	
	if( piPort ) *piPort = ntohs( sttAddr.sin_port );
	if( pszIp )
	{
#ifdef WIN32
		snprintf( pszIp, iIpSize, "%s", inet_ntoa( sttAddr.sin_addr ) );
#else
		inet_ntop( AF_INET, &sttAddr.sin_addr, pszIp, iIpSize );
#endif
	}

	if( (*piLen)+1 >= iBufSize )
	{
	  return false;
	}
	pszBuf[*piLen] = 0;
	
	return true;
}

/** UDP 메시지를 수신한다. */
bool UdpRecv( Socket iFd, char * pszBuf, int * piLen, unsigned int * piIp, unsigned short* piPort )
{
	if( iFd == INVALID_SOCKET ) return false;
	
	int iBufSize = *piLen;
	if( iBufSize <= 0 ) return false;
	
	struct sockaddr_in sttAddr;
	int iAddrSize = sizeof(sttAddr);
	
	*piLen = recvfrom( iFd, pszBuf, iBufSize-1, 0, (struct sockaddr *)&sttAddr, (socklen_t*)&iAddrSize );
	if( *piLen <= 0 )
	{
	  return false;
	}
	
	if( piPort ) *piPort = sttAddr.sin_port;
	if( piIp ) *piIp = sttAddr.sin_addr.s_addr;

	if( (*piLen)+1 > iBufSize )
	{
	  return false;
	}
	pszBuf[*piLen] = 0;
	
	return true;
}

/** UDP 메시지를 전송한다. */
bool UdpSend( Socket iFd, const char * pszBuf, int iBufLen, const char * pszIp, unsigned short iPort )
{
	if( iFd == INVALID_SOCKET || pszBuf == NULL || iBufLen <= 0 || pszIp == NULL || iPort <= 0 ) return false;
	
  struct sockaddr_in sttAddr;
  
  memset( &sttAddr, 0, sizeof(sttAddr) );
  sttAddr.sin_family = AF_INET;
  sttAddr.sin_port = htons( iPort );
  sttAddr.sin_addr.s_addr = inet_addr( pszIp );
  
  if( sendto( iFd, pszBuf, iBufLen, 0,(sockaddr*)&sttAddr, sizeof(sttAddr) ) != iBufLen )
	{
	  return false;
	}
	
	return true;
}

/** UDP 메시지를 전송한다. */
bool UdpSend( Socket iFd, const char * pszBuf, int iBufLen, unsigned int iIp, unsigned short iPort )
{
	if( iFd == INVALID_SOCKET || pszBuf == NULL || iBufLen <= 0 || iPort <= 0 ) return false;
	
  struct sockaddr_in sttAddr;
  
  memset( &sttAddr, 0, sizeof(sttAddr) );
  sttAddr.sin_family = AF_INET;
  sttAddr.sin_port = iPort;
  sttAddr.sin_addr.s_addr = iIp;
  
  if( sendto( iFd, pszBuf, iBufLen, 0,(sockaddr*)&sttAddr, sizeof(sttAddr) ) != iBufLen )
	{
	  return false;
	}
	
	return true;
}

/** 네트워크 API 를 초기화시킨다. */
void InitNetwork()
{
#ifdef WIN32
	static bool bInitNetwork = false;

	if( bInitNetwork == false )
	{
		WORD 		wVersionRequested = MAKEWORD( 2, 2 );
		WSADATA wsaData;
		
		if( WSAStartup( wVersionRequested, &wsaData ) != 0 ) 
		{
			return;
		}

		bInitNetwork = true;
	}
#endif
}

void TcpSetPollIn( struct pollfd & sttPollFd, Socket hSocket )
{
	sttPollFd.fd = hSocket;
	sttPollFd.events = POLLIN;
	sttPollFd.revents = 0;
}

#ifdef WIN32
/** 윈도우용 poll 메소드
 *	- 현재는 read event 만 처리할 수 있다.
 */
int poll( struct pollfd *fds, unsigned int nfds, int timeout )
{
	FD_SET	rset;
	unsigned int		i;
	int		n, iCount = 0;
	struct timeval	sttTimeout;

	sttTimeout.tv_sec = timeout / 1000;
	sttTimeout.tv_usec = ( timeout % 1000 ) * 1000;

	FD_ZERO(&rset);
	for( i = 0; i < nfds; ++i )
	{
		if( fds[i].fd != -1 ) FD_SET( fds[i].fd, &rset );
	}

	n = select( 0, &rset, NULL, NULL, &sttTimeout );
	if( n <= 0 )
	{
		return n;
	}

	for( i = 0; i < nfds; ++i )
	{
		if( FD_ISSET( fds[i].fd, &rset ) )
		{
			fds[i].revents = POLLIN;
			++iCount;
		}
		else
		{
			fds[i].revents = 0;
		}
	}

	return iCount;
}
#endif

/** localhost IP 주소를 제외한 호스트에 연결된 IP 주소를 가져온다. 
 *
 *	@param	strIp	IP 주소를 저장할 변수
 *	@return	성공하면 true 를 리턴한다. 실패하면 false 를 리턴한다.
 */
bool GetLocalIp( std::string & strIp )
{
#ifdef WIN32
	char szHostName[128], szIpAddr[16];
	struct sockaddr_in sttAddr;
	struct hostent     *psttHost = NULL;

	InitNetwork();

	memset( szHostName, 0, sizeof(szHostName) );
	if( gethostname(szHostName, sizeof(szHostName)) )	return false;

	psttHost = gethostbyname( szHostName );
	if( !psttHost ) return false;
	
	for( int i = 0; psttHost->h_addr_list[i]; i++ )
	{
		memcpy( &sttAddr.sin_addr, psttHost->h_addr_list[i], psttHost->h_length );
		snprintf( szIpAddr, sizeof(szIpAddr), "%s", inet_ntoa(sttAddr.sin_addr) );
		if( strcmp( szIpAddr, "127.0.0.1" ) )
		{
			strIp = szIpAddr;
			break;
		}
	}
#else
	struct ifconf ifc;
	
	int hSocket = socket( AF_INET, SOCK_DGRAM, 0 );
	int len = 100 * sizeof(struct ifreq);
	
	char buf[ len ];
	
	ifc.ifc_len = len;
	ifc.ifc_buf = buf;
	
	int e = ioctl( hSocket, SIOCGIFCONF, &ifc );
	if( e == -1 ) return false;

	char *ptr = buf;
	int tl = ifc.ifc_len;
	char	szIpAddr[16];
	
	while ( tl > 0 )
	{
		struct ifreq * ifr = (struct ifreq *)ptr;
		int si;
		
		si = sizeof(ifr->ifr_name) + sizeof(ifr->ifr_map);

		tl -= si;
		ptr += si;
		
		struct ifreq ifr2;
		ifr2 = *ifr;
		
		e = ioctl( hSocket, SIOCGIFADDR, &ifr2 );
		if( e == -1 )
		{
			printf( "ioctl error(%d) - %s\n", errno, strerror(errno) );
			continue;
		}
		
		struct sockaddr a = ifr2.ifr_addr;
		struct sockaddr_in* addr = (struct sockaddr_in*) &a;
		
		unsigned int ai = ntohl( addr->sin_addr.s_addr );

		// 127.0.0.1 주소는 출력하지 않는다.
		if( int((ai>>24)&0xFF) == 127 ) continue;

		snprintf( szIpAddr, sizeof(szIpAddr), "%d.%d.%d.%d", int((ai>>24)&0xFF)
			, int((ai>>16)&0xFF)
			, int((ai>> 8)&0xFF) 
		  , int((ai    )&0xFF) );
		strIp = szIpAddr;
		break;
	}
	
	close(hSocket);
#endif

	return true;
}
