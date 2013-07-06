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
#include "AndroidClass.h"
#include "AndroidClassConvert.h"
#include "SipClient.h"
#include "Log.h"

CSipUserAgent gclsUserAgent;

/**
 * @brief 
 * @param env 
 * @param jcSipUserAgent 
 * @param joSipServerInfo 
 * @returns 
 */
JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_InsertRegisterInfo( JNIEnv * env, jclass, jobject joSipServerInfo )
{
	CSipServerInfo clsSipServerInfo;

	if( GetSipServerInfo( env, joSipServerInfo, clsSipServerInfo ) == false ) return JNI_FALSE;

	gclsUserAgent.InsertRegisterInfo( clsSipServerInfo );

	return JNI_TRUE;
}

/**
 * @brief 
 * @param env 
 * @param jcSipUserAgent 
 * @param joSipStackSetup 
 * @returns 
 */
JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_Start( JNIEnv * env, jclass jcSipUserAgent, jobject joSipStackSetup )
{
	if( gclsClass.Init( env ) == false )
	{
		AndroidErrorLog( "Start - gclsClass.Init error" );
		return false;
	}

	CSipStackSetup clsSetup;

	if( GetSipStackSetup( env, joSipStackSetup, clsSetup ) == false ) 
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

	return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_Stop( JNIEnv * env, jclass )
{
	gclsUserAgent.Stop();

	return JNI_TRUE;
}

JNIEXPORT jstring JNICALL Java_com_cppsipstack_SipUserAgent_StartCall( JNIEnv * env, jclass, jstring jsFrom, jstring jsTo, jobject joSipCallRtp )
{
	std::string	strFrom, strTo, strCallId;
	CSipCallRtp clsRtp;
	CSipCallRoute clsRoute;

	if( GetString( env, jsFrom, strFrom ) == false ) return NULL;
	if( GetString( env, jsTo, strTo ) == false ) return NULL;
	if( GetSipCallRtp( env, joSipCallRtp, clsRtp ) == false ) return NULL;

	SIP_SERVER_INFO_LIST::iterator	itList;
	bool bRouteFound = false;

	for( itList = gclsUserAgent.m_clsRegisterList.begin(); itList != gclsUserAgent.m_clsRegisterList.end(); ++itList )
	{
		if( itList->m_bLogin )
		{
			clsRoute.m_strDestIp = itList->m_strIp;
			clsRoute.m_iDestPort = itList->m_iPort;
			bRouteFound = true;
			break;
		}
	}

	if( bRouteFound == false )
	{
		AndroidErrorLog( "StartCall - call route is not found" );
		return NULL;
	}

	if( gclsUserAgent.StartCall( strFrom.c_str(), strTo.c_str(), &clsRtp, &clsRoute, strCallId ) == false )
	{
		AndroidErrorLog( "StartCall - StartCall error" );
		return NULL;
	}

	jstring jsCallId = env->NewStringUTF( strCallId.c_str() );
	if( jsCallId == NULL )
	{
		AndroidErrorLog( "StartCall - NewStringUTF error" );
		gclsUserAgent.StopCall( strCallId.c_str(), SIP_INTERNAL_SERVER_ERROR );
		return NULL;
	}

	env->DeleteLocalRef( jsCallId );

	return jsCallId;
}

JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_StopCall( JNIEnv * env, jclass, jstring jsCallId, jint iSipCode )
{
	return JNI_TRUE;
}

JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_AcceptCall( JNIEnv * env, jclass, jstring jsCallId )
{
	return JNI_TRUE;
}
