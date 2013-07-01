#ifndef _ANDROID_LOG_H_
#define _ANDROID_LOG_H_

#define LOG_TAG				"SipStack"
#define LOG_MAX_SIZE	8192

void AndroidDebugLog( const char * fmt, ... );
void AndroidErrorLog( const char * fmt, ... );

#endif

