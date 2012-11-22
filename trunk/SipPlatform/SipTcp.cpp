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

#include "SipPlatformDefine.h"
#include "SipTcp.h"

#ifndef WIN32
#include <ctype.h>

/** timeout 이 있는 connect() 함수이다. 
 *
 * @ingroup SipStack
 * @author Yee Young Han 
 * @param  iSockFd socket() 함수로 만들어진 file descriptor
 * @param  saptr   connect 하고자하는 서버의 IP, port 를 기록한 sockaddr 구조체
 * @param  saiLen  saptr 변수의 크기
 * @param  iSecond connect 가 되기까지 기다리는 시간 ( 초단위 ) 
 * @return 성공하면 0을 리턴하고 실패하면 음수의 값을 리턴한다.
 *			connect() 함수 호출 실패시에는 -101를 리턴한다.
 *			poll() 함수 호출 실패시에는 -102를 리턴한다.
 *			getsockopt() 함수 호출 실패시에는 -103을 리턴한다.
 *			SO_ERROR 가 발생한 경우 -104를 리턴한다. 
 */

static int ConnectTimeout( int iSockFd, const struct sockaddr *saptr, socklen_t saiLen, int iSecond )
{
	int					iFlags, n, iErrorNum = 0;
	socklen_t		iLen;

	iFlags = fcntl( iSockFd, F_GETFL, 0 );
	fcntl( iSockFd, F_SETFL, iFlags | O_NONBLOCK );

	if( ( n = connect( iSockFd, (struct sockaddr *)saptr, saiLen ) ) < 0 )
	{
		// QQQ : errno 가 17 이 계속 나오면서, 제대로 실행되지 않는 경우가 있었다.
		//     이를 패치하기 위해서 아래와 같이 EEXIST 를 무시하도록 수정하였다.
		// QQQ : errno 가 4 가 나와서 문제가 되는 경우가 있다. 그래서, 이를 패치함.
		if( errno != 0 && errno != EINPROGRESS && errno != EEXIST && errno != EINTR ) return -101;
	}

	if( n == 0 ) goto done;

	pollfd sttPoll[1];

	sttPoll[0].fd = iSockFd;
	sttPoll[0].events = POLLIN | POLLOUT;
	sttPoll[0].revents = 0;

	n = poll( sttPoll, 1, iSecond * 1000 );
	if( n < 0 )
	{
		return -102;
	}
	else if( n == 0 )
	{
		errno = ETIMEDOUT;
		return -102;
	}

	if( sttPoll[0].revents & ( POLLIN | POLLOUT ) )
	{
		iLen = sizeof(iErrorNum);
		if( getsockopt(iSockFd, SOL_SOCKET, SO_ERROR, &iErrorNum, &iLen) < 0 )
			return -103;
	}
	else
	{
		// iErrorNum log	
	}

done:
	fcntl( iSockFd, F_SETFL, iFlags );	// restore

	if( iErrorNum )
	{
		errno = iErrorNum;
		return -104;
	}

	return 0;
}
#endif

/**
 * @ingroup SipStack
 * @brief 호스트 이름으로 IP 주소를 검색한다.
 * @param szHostName	호스트 이름
 * @param szIp				IP 주소를 저장할 변수
 * @param iLen				IP 주소를 저장할 변수의 크기
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool GetIpByName( const char * szHostName, char * szIp, int iLen )
{
	struct	hostent	* hptr;

	if( (hptr = gethostbyname(szHostName)) == NULL ) return false;
	
	snprintf( szIp, iLen, "%s", inet_ntoa( *(struct in_addr *)hptr->h_addr_list[0] ));
	
	return true;
}

/**
 * @ingroup SipStack
 * @brief TCP 서버에 연결한다.
 * @param pszIp			TCP 서버 IP 주소
 * @param iPort			TCP 서버 포트 번호
 * @param iTimeout	연결 timeout 시간 ( 초단위 ) - 0 이상으로 설정해야 연결 timeout 기능이 동작한다.
 * @returns 성공하면 연결된 TCP 소켓을 리턴하고 그렇지 않으면 INVALID_SOCKET 를 리턴한다.
 */
Socket TcpConnect( const char * pszIp, int iPort, int iTimeout )
{
	char		szIp[16];
	Socket	fd;

	struct	sockaddr_in	addr;		
	
	memset( szIp, 0, sizeof(szIp) );
	if( isdigit(pszIp[0]) == 0 )
	{
		// if first character is not digit, suppose it is domain main.
		GetIpByName( pszIp, szIp, sizeof(szIp) );
	}
	else
	{
		snprintf( szIp, sizeof(szIp), "%s", pszIp );
	}

	// connect server.
	if( ( fd = socket( AF_INET, SOCK_STREAM, 0 )) == INVALID_SOCKET )
	{
		return INVALID_SOCKET;
	}

	addr.sin_family = AF_INET;
	addr.sin_port   = htons(iPort);
	addr.sin_addr.s_addr = inet_addr( szIp );	

#ifndef WIN32
	if( iTimeout > 0 )
	{
		if( ConnectTimeout( fd, (struct sockaddr *)&addr, sizeof(addr), iTimeout ) != 0 )
		{
			closesocket( fd );
			return INVALID_SOCKET;
		}
	}
	else
#endif
	if( connect( fd, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR )
	{
		closesocket( fd );
		return INVALID_SOCKET;
	}

	return fd;
}

/** 
 * @ingroup SipStack
 * @brief 네트워크 전송 함수
 * @param	fd			소켓 핸들
 * @param	szBuf		전송 버퍼
 * @param	iBufLen	전송 버퍼 크기
 * @return 성공하면 전송한 크기를 리턴하고 실패하면 SOCKET_ERROR 를 리턴한다.
 */
int TcpSend( Socket fd, const char * szBuf, int iBufLen )
{
	int		n;	
	int		iSendLen = 0;
	
	while( 1 )
	{
		n = send( fd, szBuf + iSendLen, iBufLen - iSendLen, 0 );
		if( n == SOCKET_ERROR ) return SOCKET_ERROR ;
	
		iSendLen += n;
		if( iSendLen == iBufLen ) break;	
	}
	
	return iBufLen;
}

/**
 * @ingroup SipStack
 * @brief timeout 을 가진 TCP 수신 메소드
 * @param fd			소켓 핸들
 * @param szBuf		수신 버퍼
 * @param iBufLen 수신 버퍼 크기
 * @param iSecond 수신 timeout ( 초 단위 )
 * @returns 성공하면 수신 버퍼 크기를 리턴하고 실패하면 SOCKET_ERROR 을 리턴한다.
 */
int TcpRecv( Socket fd, char * szBuf, int iBufLen, int iSecond )
{
	int				n;
	pollfd sttPoll[1];

	sttPoll[0].fd = fd;
	sttPoll[0].events = POLLIN;
	sttPoll[0].revents = 0;

	n = poll( sttPoll, 1, 1000 * iSecond );
	if( n <= 0 )
	{
		return SOCKET_ERROR;
	}

	return recv( fd, szBuf, iBufLen, 0 );
}

/**
 * @ingroup SipStack
 * @brief 수신 버퍼가 가득 찰 때까지 데이터를 수신한다.
 * @param fd			소켓 핸들
 * @param szBuf		수신 버퍼
 * @param iBufLen 수신 버퍼 크기
 * @param iSecond 수신 timeout ( 초 단위 )
 * @returns 성공하면 수신 버퍼 크기를 리턴하고 실패하면 SOCKET_ERROR 을 리턴한다.
 */
int TcpRecvSize( Socket fd, char * szBuf, int iBufLen, int iSecond )
{
	int		 n, iRecvLen = 0;
	pollfd sttPoll[1];

	sttPoll[0].fd = fd;
	sttPoll[0].events = POLLIN;
	sttPoll[0].revents = 0;

	while( iRecvLen < iBufLen )
	{
		n = poll( sttPoll, 1, 1000 * iSecond );
		if( n <= 0 )
		{
			return SOCKET_ERROR;
		}

		n = recv( fd, szBuf + iRecvLen, iBufLen - iRecvLen, 0 );
		if( n <= 0 ) return SOCKET_ERROR;

		iRecvLen += n;
	}

	return iRecvLen;
}

/**
 * @ingroup SipStack
 * @brief TCP 서버 소켓을 생성한다.
 * @param	iPort			TCP 포트 번호
 * @param	iListenQ	queue number to listen
 * @param	pszIp			수신 IP 주소
 * @return 성공하면 소켓 핸들을 리턴하고 실패하면 INVALID_SOCKET 를 리턴한다.
 */
Socket TcpListen( int iPort, int iListenQ, const char * pszIp )
{
	Socket	fd;

	struct	sockaddr_in	addr;
	const 	int		on = 1;

	// create socket.
	if( ( fd = socket( AF_INET, SOCK_STREAM, 0 )) == INVALID_SOCKET )
	{
		return INVALID_SOCKET;
	}
	
	memset( &addr, 0, sizeof(addr));
	
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(iPort);

	if( pszIp )
	{
		addr.sin_addr.s_addr = inet_addr(pszIp);
	}
	else
	{
		addr.sin_addr.s_addr = INADDR_ANY;	
	}

	if( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) == -1 )
	{
		
	}

	if( bind( fd, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR )
	{
		closesocket( fd );
		return INVALID_SOCKET;
	}

	if( listen( fd, iListenQ ) == SOCKET_ERROR )
	{
		closesocket( fd );
		return INVALID_SOCKET;
	}

	return fd;
}

/**
 * @ingroup SipStack
 * @brief TCP accept wrapper function
 * @param hListenFd TCP 서버 소켓
 * @param pszIp			연결된 클라이언트 IP 주소가 저장될 변수
 * @param iIpSize		pszIp 변수의 크기
 * @param piPort		연결된 클라이언트 포트가 저장될 변수
 * @returns 성공하면 연결된 클라이언트 소켓 핸들을 리턴한다.
 *					실패하면 INVALID_SOCKET 를 리턴한다.
 */
Socket TcpAccept( Socket hListenFd, char * pszIp, int iIpSize, int * piPort )
{
	struct sockaddr_in sttAddr;
	socklen_t		iAddrLen;
	Socket			hConnFd;

	iAddrLen = sizeof(sttAddr);
	hConnFd = accept( hListenFd, (struct sockaddr *)&sttAddr, &iAddrLen );
	if( hConnFd != INVALID_SOCKET )
	{
		if( piPort ) *piPort = ntohs( sttAddr.sin_port );

		if( pszIp && iIpSize > 0 )
		{
			snprintf( pszIp, iIpSize, "%s", inet_ntoa( sttAddr.sin_addr ) );
		}
	}

	return hConnFd;
}

#ifdef WIN32

/** make listening socket for TCP server.
 *	- port reuse option 을 사용하지 않는다.
 *	- 윈도우용 pipe 를 위해서 만들어졌다.
 *
 *  @author Yee Young Han 
 *	@param	iPort		port number to listen
 *	@param	iListenQ	queue number to listen
 *  @return if success, return socket.
 *			if can not create socket, return INVALID_SOCKET.
 */
static Socket TcpListenNotReuse( int iPort, int iListenQ, const char * pszIp )
{
	Socket	fd;

	struct	sockaddr_in	addr;

	// create socket.
	if( ( fd = socket( AF_INET, SOCK_STREAM, 0 )) == INVALID_SOCKET )
	{
		return INVALID_SOCKET;
	}
	
	memset( &addr, 0, sizeof(addr));
	
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(iPort);

	if( pszIp )
	{
		addr.sin_addr.s_addr = inet_addr(pszIp);
	}
	else
	{
		addr.sin_addr.s_addr = INADDR_ANY;	
	}

	if( bind( fd, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR )
	{
		closesocket( fd );
		return INVALID_SOCKET;
	}

	if( listen( fd, iListenQ ) == SOCKET_ERROR )
	{
		closesocket( fd );
		return INVALID_SOCKET;
	}

	return fd;
}

static bool gbIsStartPipeThread = false;
static int giListenPort = 0;

/** 윈도우용 pipe 메소드를 위한 쓰레드 */
DWORD WINAPI PipeThread( LPVOID lpParameter )
{
	int			iPort;
	char		szBuf[12];
	Socket	hListenFd, hConnFd;

	for( iPort = 1024; iPort < 65535; ++iPort )
	{
		hListenFd = TcpListenNotReuse( iPort, 255, "127.0.0.1" );
		if( hListenFd != INVALID_SOCKET ) break;
	}

	if( hListenFd == INVALID_SOCKET ) return 0;

	giListenPort = iPort;

	while( 1 )
	{
		hConnFd = accept( hListenFd, NULL, NULL );
		if( hConnFd == INVALID_SOCKET ) break;

		memcpy( szBuf, &hConnFd, sizeof(hConnFd) );
		send( hConnFd, szBuf, sizeof(hConnFd), 0 );
	}

	return 0;
}

/** 윈도우용 pipe 메소드
 *
 *	@param	filedes	pipe 를 위한 소켓 배열
 *	@return	성공하면 0 을 리턴한다. 실패하면 -1 을 리턴한다.
 */
int pipe( Socket filedes[2] )
{
	if( gbIsStartPipeThread == false )
	{
		DWORD		dwThreadId;
		HANDLE	hThread;

		hThread = CreateThread( NULL, 0, PipeThread, NULL, 0, &dwThreadId );
		if( hThread == NULL )
		{
			return -1;
		}

		gbIsStartPipeThread = true;

		// PipeThread 에서 소켓을 생성할 때까지 대기한다. (1초)
		for( int i = 0; i < 50; ++i )
		{
			if( giListenPort != 0 ) break;
			Sleep( 20 );
		}
	}

	int		n;
	char	szBuf[12];

	if( giListenPort == 0 ) return -1;

	filedes[1] = TcpConnect( "127.0.0.1", giListenPort );
	if( filedes[1] == INVALID_SOCKET ) return -1;

	n = recv( filedes[1], szBuf, sizeof(Socket), 0 );
	if( n != sizeof(Socket) )
	{
		closesocket( filedes[1] );
		return -1;
	}

	memcpy( &filedes[0], szBuf, sizeof(Socket) );

	return 0;
}

#endif
