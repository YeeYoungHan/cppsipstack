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

#ifndef _SIP_DEFINE_H_
#define _SIP_DEFINE_H_

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC

#include <stdlib.h>
#include <crtdbg.h>
#endif

#define snprintf		_snprintf
#define strcasecmp	_stricmp
#define sleep(x)		Sleep(x*1000)

typedef unsigned __int64 uint64_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

typedef __int64 int64_t;
typedef int int32_t;
typedef __int16 int16_t;
typedef char int8_t;

#else

#include <stdint.h>

#endif

#define SIP_MESSAGE_MAX_LEN				8192
#define SIP_TAG_MAX_SIZE					21
#define SIP_BRANCH_MAX_SIZE				51
#define SIP_CALL_ID_NAME_MAX_SIZE	51

#define VIA_PREFIX						"z9hG4bK"
#define VIA_PREFIX_LEN				7

#define SIP_PROTOCOL					"sip"
#define SIP_VERSION						"SIP/2.0"

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/time.h>
#endif

#ifdef WIN32
int gettimeofday( struct timeval *tv, struct timezone *tz );
#endif

#endif

/**
 * @defgroup SipParser SipParser
 * SIP 메시지 파서/생성 라이브러리
 */

