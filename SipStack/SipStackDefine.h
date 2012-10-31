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

#ifndef _SIP_STACK_DEFINE_H_

#include "SipParserDefine.h"
#include "SipStackVersion.h"

#define SIP_PACKET_MIN_SIZE		100
#define SIP_PACKET_MAX_SIZE		8192
#define SIP_RING_TIMEOUT			300000

#define SIP_TCP_MAX_SOCKET_PER_THREAD	100
#define SIP_TCP_RECV_TIMEOUT					600

#define SIP_USER_AGENT	"CppSipStack_" SIP_STACK_VERSION
#define SIP_MAX_FORWARDS	70

#include "SipMessage.h"

#include <map>

int DiffTimeval( struct timeval * psttOld, struct timeval * psttNew );

#ifdef WIN32
bool StartThread( const char * pszName, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter );
#else
bool StartThread( const char * pszName, void *(*lpStartAddress)(void*), void * lpParameter );
#endif

void MiliSleep( int iMiliSecond );

#endif

// 자료구조 모니터링
#define MR_COL_SEP							"|"
#define MR_ROW_SEP							"\n"

/**
@mainpage C++ SIP Stack
@section intro 소개
	- C++ STL 을 사용한 SIP Stack 라이브러리
	- 프로젝트의 최종 목표는 IP-PBX 개발
	- 프로젝트 홈페이지 : http://dev.naver.com/projects/sipstack
@section  CREATEINFO 작성정보
  - 작성자 : 이영한
	- 홈페이지 : http://blog.naver.com/websearch
*/

