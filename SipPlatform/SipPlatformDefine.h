#ifndef _SIP_PLATFORM_DEFINE_H_
#define _SIP_PLATFORM_DEFINE_H_

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

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/time.h>
#endif

#ifdef WIN32
int gettimeofday( struct timeval *tv, struct timezone *tz );
#endif

#endif
