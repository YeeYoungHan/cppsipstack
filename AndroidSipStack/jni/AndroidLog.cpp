#include "AndroidLog.h"
#include <stdarg.h>
#include <stdio.h>
#include <android/log.h>

void AndroidDebugLog( const char * fmt, ... )
{
	char	szBuf[LOG_MAX_SIZE];
	
	va_list		ap;
	
	va_start( ap, fmt );
	vsnprintf( szBuf, sizeof(szBuf)-1, fmt, ap );
	va_end( ap );
	
	__android_log_write( ANDROID_LOG_DEBUG, LOG_TAG, szBuf );
}

void AndroidErrorLog( const char * fmt, ... )
{
	char	szBuf[LOG_MAX_SIZE];
	
	va_list		ap;
	
	va_start( ap, fmt );
	vsnprintf( szBuf, sizeof(szBuf)-1, fmt, ap );
	va_end( ap );
	
	__android_log_write( ANDROID_LOG_ERROR, LOG_TAG, szBuf );
}

