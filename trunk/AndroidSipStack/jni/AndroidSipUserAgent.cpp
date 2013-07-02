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

#include "AndroidSipUserAgent.h"
#include "SipUserAgent.h"
#include "AndroidLog.h"
#include "AndroidGet.h"

CSipUserAgent gclsUserAgent;
bool gbAndroidDebug = false;

#include "AndroidSipUserAgent.hpp"

JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_InsertRegisterInfo( JNIEnv * env, jclass, jobject clsSipServerInfo )
{
	CSipServerInfo clsInfo;

	if( GetSipServerInfo( env, clsSipServerInfo, clsInfo ) == false ) return JNI_FALSE;

	gclsUserAgent.InsertRegisterInfo( clsInfo );

	return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_Start( JNIEnv * env, jclass, jobject clsSipStackSetup )
{
	CSipStackSetup clsSetup;

	if( GetSipStackSetup( env, clsSipStackSetup, clsSetup ) == false ) return JNI_FALSE;

	gclsUserAgent.Start( clsSetup );

	return JNI_TRUE;
}
