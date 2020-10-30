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
#define _SIP_STACK_DEFINE_H_

// TLS 기능 없이 SIP stack 을 빌드하고 싶으면 아래의 선언을 주석 처리하세요.
#define USE_TLS

// transaction list 에 map 을 사용하고 싶으면 아래의 선언을 주석 처리하세요. 아래의 선언을 주석 처리하지 않으면 unordered_map 을 사용합니다.
//#define USE_HASH_MAP

#ifdef USE_TLS
// TLS 를 최종 종료할 때에 메모리 할당을 해제하는 기능을 사용하고 싶지 않을 경우 주석 처리하세요.
#define USE_TLS_FREE
#endif

#include "SipParserDefine.h"
#include "SipStackVersion.h"

#define SIP_PACKET_MIN_SIZE		100
#define SIP_PACKET_MAX_SIZE		8192
#define SIP_RING_TIMEOUT			300000

#define SIP_TCP_MAX_SOCKET_PER_THREAD	100
#define SIP_TCP_RECV_TIMEOUT					600
#define SIP_TCP_CONNECT_TIMEOUT				10
#define SIP_TLS_ACCEPT_TIMEOUT				10

#define SIP_UDP_PORT			5060
#define SIP_TCP_PORT			5060
#define SIP_TLS_PORT			5061

#define SIP_USER_AGENT	"CppSipStack_" SIP_STACK_VERSION
#define SIP_MAX_FORWARDS	70

#include "SipMessage.h"

#include <map>

#ifdef USE_HASH_MAP

#include <unordered_map>

#if defined(WIN32) && _MSC_VER == VC2008_VERSION
#define MAP std::tr1::unordered_map
#else
#define MAP std::unordered_map
#endif

#else

#define MAP std::map

#endif

// 자료구조 모니터링
#define MR_COL_SEP							"|"
#define MR_ROW_SEP							"\n"

#endif

/**
@mainpage C++ SIP Stack
@section intro 소개
	- C++ STL 을 사용한 SIP Stack 라이브러리
	- 프로젝트의 최종 목표는 IP-PBX 개발
	- 프로젝트 홈페이지 : https://github.com/YeeYoungHan/cppsipstack
@section  CREATEINFO 작성정보
  - 작성자 : 이영한
	- 홈페이지 : http://blog.naver.com/websearch
*/

