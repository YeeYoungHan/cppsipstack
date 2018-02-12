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

#ifndef _SIP_PLATFORM_DEFINE_H_
#define _SIP_PLATFORM_DEFINE_H_

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// Window XP 에서 빌드할 때에는 아래의 주석을 해제하라.
//#define WINXP

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC

#include <stdlib.h>
#include <crtdbg.h>
#endif

#define VC2008_VERSION	1500

#define strcasecmp	_stricmp
#define sleep(x)		Sleep(x*1000)

typedef unsigned __int64 uint64_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

typedef __int64 int64_t;
typedef int int32_t;
typedef __int16 int16_t;

#if _MSC_VER == VC2008_VERSION
typedef char int8_t;

#define snprintf		_snprintf
#define atoll(x) _atoi64(x)
#endif

#define THREAD_API	DWORD WINAPI

#else

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define THREAD_API	void *
#define LPVOID			void *

#endif

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/time.h>
#endif

#if __WORDSIZE == 64
#ifndef __APPLE__
#define LINUX_64
#endif
#endif

#endif

#if defined LINUX_64
#define UNSIGNED_LONG_LONG_FORMAT	"%lu"
#define LONG_LONG_FORMAT	"%ld"
#else
#define UNSIGNED_LONG_LONG_FORMAT	"%llu"
#define LONG_LONG_FORMAT	"%lld"
#endif

/**
 * @defgroup SipPlatform SipPlatform
 * SIP stack 프로젝트에서 사용되는 OS independent 라이브러리
 */
