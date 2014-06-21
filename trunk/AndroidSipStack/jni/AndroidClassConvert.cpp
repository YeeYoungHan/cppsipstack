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

#include "AndroidClassConvert.h"
#include "AndroidLog.h"
#include "AndroidGet.h"
#include "AndroidPut.h"

/**
 * @ingroup AndroidSipStack
 * @brief Java SipServerInfo 객체를 C++ CSipServerInfo 객체로 복사한다.
 * @param env							JNI
 * @param joSipServerInfo	Java SipServerInfo 객체
 * @param clsOutput				C++ CSipServerInfo 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool GetSipServerInfo( JNIEnv * env, jobject joSipServerInfo, CSipServerInfo & clsOutput )
{
	jclass jc = env->GetObjectClass( joSipServerInfo );
	if( jc == NULL )
	{
		AndroidErrorLog( "joSipServerInfo is not found" );
		return false;
	}

	if( GetString( env, joSipServerInfo, jc, "m_strIp", clsOutput.m_strIp ) == false ) return false;
	if( GetInt( env, joSipServerInfo, jc, "m_iPort", clsOutput.m_iPort ) == false ) return false;
	if( GetString( env, joSipServerInfo, jc, "m_strDomain", clsOutput.m_strDomain ) == false ) return false;
	if( GetString( env, joSipServerInfo, jc, "m_strUserId", clsOutput.m_strUserId ) == false ) return false;
	if( GetString( env, joSipServerInfo, jc, "m_strPassWord", clsOutput.m_strPassWord ) == false ) return false;

	if( gbAndroidDebug )
	{
		AndroidDebugLog( "ip(%s) port(%d) domain(%s) userid(%s) password(%s)"
			, clsOutput.m_strIp.c_str(), clsOutput.m_iPort, clsOutput.m_strDomain.c_str()
			, clsOutput.m_strUserId.c_str(), clsOutput.m_strPassWord.c_str() );
	}

	return true;
}

/**
 * @ingroup AndroidSipStack
 * @brief C++ CSipServerInfo 객체를 Java SipServerInfo 객체로 복사한다.
 * @param env							JNIEnv
 * @param joSipServerInfo Java SipServerInfo 객체
 * @param clsInput				C++ CSipServerInfo 객체
 * @returns 
 */
bool PutSipServerInfo( JNIEnv * env, jobject joSipServerInfo, CSipServerInfo & clsInput )
{
	jclass jc = env->GetObjectClass( joSipServerInfo );
	if( jc == NULL )
	{
		AndroidErrorLog( "joSipServerInfo is not found" );
		return false;
	}

	if( PutString( env, joSipServerInfo, jc, "m_strIp", clsInput.m_strIp.c_str() ) == false ) return false;
	if( PutInt( env, joSipServerInfo, jc, "m_iPort", clsInput.m_iPort ) == false ) return false;
	if( PutString( env, joSipServerInfo, jc, "m_strDomain", clsInput.m_strDomain.c_str() ) == false ) return false;
	if( PutString( env, joSipServerInfo, jc, "m_strUserId", clsInput.m_strUserId.c_str() ) == false ) return false;
	if( PutString( env, joSipServerInfo, jc, "m_strPassWord", clsInput.m_strPassWord.c_str() ) == false ) return false;
	
	return true;
}

/**
 * @ingroup AndroidSipStack
 * @brief Java SipStackSetup 객체를 C++ CSipStackSetup 객체로 복사한다.
 * @param env							JNI
 * @param joSipStackSetup	Java SipStackSetup 객체
 * @param clsOutput				C++ CSipStackSetup 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool GetSipStackSetup( JNIEnv * env, jobject joSipStackSetup, CSipStackSetup & clsOutput )
{
	jclass jc = env->GetObjectClass( joSipStackSetup );
	if( jc == NULL )
	{
		AndroidErrorLog( "joSipStackSetup is not found" );
		return false;
	}

	if( GetString( env, joSipStackSetup, jc, "m_strLocalIp", clsOutput.m_strLocalIp ) == false ) return false;
	if( GetInt( env, joSipStackSetup, jc, "m_iLocalUdpPort", clsOutput.m_iLocalUdpPort ) == false ) return false;
	if( GetInt( env, joSipStackSetup, jc, "m_iUdpThreadCount", clsOutput.m_iUdpThreadCount ) == false ) return false;
	if( GetInt( env, joSipStackSetup, jc, "m_iLocalTcpPort", clsOutput.m_iLocalTcpPort ) == false ) return false;
	if( GetInt( env, joSipStackSetup, jc, "m_iLocalTlsPort", clsOutput.m_iLocalTlsPort ) == false ) return false;
	if( GetInt( env, joSipStackSetup, jc, "m_iTcpThreadCount", clsOutput.m_iTcpThreadCount ) == false ) return false;
	if( GetInt( env, joSipStackSetup, jc, "m_iTcpMaxSocketPerThread", clsOutput.m_iTcpMaxSocketPerThread ) == false ) return false;
	if( GetInt( env, joSipStackSetup, jc, "m_iTcpRecvTimeout", clsOutput.m_iTcpRecvTimeout ) == false ) return false;
	if( GetInt( env, joSipStackSetup, jc, "m_iTlsAcceptTimeout", clsOutput.m_iTlsAcceptTimeout ) == false ) return false;
	if( GetString( env, joSipStackSetup, jc, "m_strCertFile", clsOutput.m_strCertFile ) == false ) return false;
	if( GetString( env, joSipStackSetup, jc, "m_strUserAgent", clsOutput.m_strUserAgent ) == false ) return false;
	if( GetBool( env, joSipStackSetup, jc, "m_bUseSipCompactForm", clsOutput.m_bUseSipCompactForm ) == false ) return false;
	
	return true;
}

/**
 * @ingroup AndroidSipStack
 * @brief Java SipCallRtp 객체를 C++ CSipCallRtp 객체로 복사한다.
 * @param env						JNI
 * @param joSipCallRtp	Java SipCallRtp 객체
 * @param clsOutput			C++ CSipCallRtp 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool GetSipCallRtp( JNIEnv * env, jobject joSipCallRtp, CSipCallRtp & clsOutput )
{
	jclass jc = env->GetObjectClass( joSipCallRtp );
	if( jc == NULL )
	{
		AndroidErrorLog( "joSipCallRtp is not found" );
		return false;
	}

	if( GetString( env, joSipCallRtp, jc, "m_strIp", clsOutput.m_strIp ) == false ) return false;
	if( GetInt( env, joSipCallRtp, jc, "m_iPort", clsOutput.m_iPort ) == false ) return false;
	if( GetInt( env, joSipCallRtp, jc, "m_iCodec", clsOutput.m_iCodec ) == false ) return false;
	if( GetInt( env, joSipCallRtp, jc, "m_iDirection", (int &)clsOutput.m_eDirection ) == false ) return false;
	
	return true;
}

/**
 * @ingroup AndroidSipStack
 * @brief C++ CSipCallRtp 객체를 Java SipCallRtp 객체로 복사한다.
 * @param env						JNI
 * @param joSipCallRtp	Java SipCallRtp 객체
 * @param clsInput			C++ CSipCallRtp 객체
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool PutSipCallRtp( JNIEnv * env, jobject joSipCallRtp, CSipCallRtp & clsInput )
{
	jclass jc = env->GetObjectClass( joSipCallRtp );
	if( jc == NULL )
	{
		AndroidErrorLog( "joSipCallRtp is not found" );
		return false;
	}

	if( PutString( env, joSipCallRtp, jc, "m_strIp", clsInput.m_strIp.c_str() ) == false ) return false;
	if( PutInt( env, joSipCallRtp, jc, "m_iPort", clsInput.m_iPort ) == false ) return false;
	if( PutInt( env, joSipCallRtp, jc, "m_iCodec", clsInput.m_iCodec ) == false ) return false;
	if( PutInt( env, joSipCallRtp, jc, "m_iDirection", clsInput.m_eDirection ) == false ) return false;
	
	return true;
}
