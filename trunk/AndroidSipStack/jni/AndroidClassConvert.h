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

#ifndef _ANDROID_CLASS_CONVERT_H_
#define _ANDROID_CLASS_CONVERT_H_

#include <jni.h>
#include "SipUserAgent.h"

bool GetSipServerInfo( JNIEnv * env, jobject clsSipServerInfo, CSipServerInfo & clsOutput );
bool PutSipServerInfo( JNIEnv * env, jobject clsSipServerInfo, CSipServerInfo & clsOutput );

bool GetSipStackSetup( JNIEnv * env, jobject clsSipStackSetup, CSipStackSetup & clsOutput );

bool GetSipCallRtp( JNIEnv * env, jobject joSipCallRtp, CSipCallRtp & clsOutput );
bool PutSipCallRtp( JNIEnv * env, jobject joSipCallRtp, CSipCallRtp & clsInput );

#endif
