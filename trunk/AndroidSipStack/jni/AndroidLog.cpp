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

#define _CRT_SECURE_NO_WARNINGS

#include "AndroidLog.h"
#include <stdarg.h>
#include <stdio.h>

#ifndef WIN32
#include <android/log.h>
#endif

void AndroidDebugLog( const char * fmt, ... )
{
	char	szBuf[LOG_MAX_SIZE];
	
	va_list		ap;
	
	va_start( ap, fmt );
	vsnprintf( szBuf, sizeof(szBuf)-1, fmt, ap );
	va_end( ap );
	
#ifndef WIN32
	__android_log_write( ANDROID_LOG_DEBUG, LOG_TAG, szBuf );
#endif
}

void AndroidErrorLog( const char * fmt, ... )
{
	char	szBuf[LOG_MAX_SIZE];
	
	va_list		ap;
	
	va_start( ap, fmt );
	vsnprintf( szBuf, sizeof(szBuf)-1, fmt, ap );
	va_end( ap );
	
#ifndef WIN32
	__android_log_write( ANDROID_LOG_ERROR, LOG_TAG, szBuf );
#endif
}

