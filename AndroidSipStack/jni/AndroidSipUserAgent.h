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

#ifndef _ANDROID_SIP_USER_AGENT_H_
#define _ANDROID_SIP_USER_AGENT_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_InsertRegisterInfo( JNIEnv * env, jclass, jobject joSipServerInfo );
JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_Start( JNIEnv * env, jclass, jobject joSipStackSetup );
JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_Stop( JNIEnv * env, jclass );
JNIEXPORT jstring JNICALL Java_com_cppsipstack_SipUserAgent_StartCall( JNIEnv * env, jclass, jstring strFrom, jstring strTo, jobject joSipCallRtp );
JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_StopCall( JNIEnv * env, jclass, jstring strCallId, jint iSipCode );
JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_AcceptCall( JNIEnv * env, jclass, jstring strCallId, jobject joSipCallRtp );

#ifdef __cplusplus
}
#endif
#endif

