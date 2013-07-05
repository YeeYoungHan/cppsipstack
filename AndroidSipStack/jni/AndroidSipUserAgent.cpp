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
#include "AndroidPut.h"
#include "SipClient.h"
#include "Log.h"

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

JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_Start( JNIEnv * env, jclass clsSipUserAgent, jobject clsSipStackSetup )
{
	CSipStackSetup clsSetup;

	if( GetSipStackSetup( env, clsSipStackSetup, clsSetup ) == false ) 
	{
		AndroidErrorLog( "Start - GetSipStackSetup error" );
		return JNI_FALSE;
	}

	if( clsSetup.m_strLocalIp.empty() )
	{
		GetLocalIp( clsSetup.m_strLocalIp );
	}

	AndroidDebugLog( "local ip[%s]", clsSetup.m_strLocalIp.c_str() );

	CLog::SetLevel( LOG_DEBUG | LOG_INFO | LOG_NETWORK | LOG_SYSTEM );

	gclsUserAgent.m_pclsCallBack = &gclsSipClient;
	if( gclsUserAgent.Start( clsSetup ) == false ) 
	{
		AndroidErrorLog( "Start - Start error" );
		return JNI_FALSE;
	}

	jmethodID mid = env->GetStaticMethodID( clsSipUserAgent, "EventRegister", "(Lcom/cppsipstack/SipServerInfo;I)V" );

	jclass jClass = env->FindClass( "com/cppsipstack/SipServerInfo" );
	jmethodID cid = env->GetMethodID( jClass, "<init>", "()V" );

	jobject clsInfo = env->NewObject( jClass, cid );

	PutString( env, clsInfo, jClass, "m_strIp", "127.0.0.1" );

	env->CallStaticVoidMethod( clsSipUserAgent, mid, clsInfo, 200 );
	env->DeleteLocalRef( clsInfo );

	return JNI_TRUE;
}
