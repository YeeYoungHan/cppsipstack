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

#include <stdio.h>
#include "AndroidClass.h"
#include "AndroidLog.h"

CAndroidClass gclsClass;

CAndroidClass::CAndroidClass()
{
}

CAndroidClass::~CAndroidClass()
{
}

/**
 * @brief Java 에서 생성된 클래스 정보를 저장하고 callback 정보를 저장한다.
 * @param env JNIEnv
 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CAndroidClass::Init( JNIEnv * env )
{
	// 클래스 정보 저장
	m_jcSipUserAgent = env->FindClass( "com/cppsipstack/SipUserAgent" );
	if( m_jcSipUserAgent == NULL )
	{
		AndroidErrorLog( "com/cppsipstack/SipUserAgent is not found" );
		return false;
	}

	m_jcSipUserAgent = (jclass)env->NewGlobalRef( m_jcSipUserAgent );

	m_jcSipServerInfo = env->FindClass( "com/cppsipstack/SipServerInfo" );
	if( m_jcSipServerInfo == NULL )
	{
		AndroidErrorLog( "com/cppsipstack/SipServerInfo is not found" );
		return false;
	}

	m_jcSipServerInfo = (jclass)env->NewGlobalRef( m_jcSipServerInfo );

	m_jcSipCallRtp = env->FindClass( "com/cppsipstack/SipCallRtp" );
	if( m_jcSipCallRtp == NULL )
	{
		AndroidErrorLog( "com/cppsipstack/SipCallRtp is not found" );
		return false;
	}

	m_jcSipCallRtp = (jclass)env->NewGlobalRef( m_jcSipCallRtp );

	// callback 정보 저장
	m_jmEventRegister = env->GetStaticMethodID( m_jcSipUserAgent, "EventRegister", "(Lcom/cppsipstack/SipServerInfo;I)V" );
	if( m_jmEventRegister == NULL )
	{
		AndroidErrorLog( "EventRegister is not found" );
		return false;
	}

	m_jmEventIncomingCall = env->GetStaticMethodID( m_jcSipUserAgent, "EventIncomingCall"
		, "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lcom/cppsipstack/SipCallRtp;)V" );
	if( m_jmEventIncomingCall == NULL )
	{
		AndroidErrorLog( "EventIncomingCall is not found" );
		return false;
	}
	
	m_jmEventCallRing = env->GetStaticMethodID( m_jcSipUserAgent, "EventCallRing", "(Ljava/lang/String;ILcom/cppsipstack/SipCallRtp;)V" );
	if( m_jmEventCallRing == NULL )
	{
		AndroidErrorLog( "EventCallRing is not found" );
		return false;
	}
	
	m_jmEventCallStart = env->GetStaticMethodID( m_jcSipUserAgent, "EventCallStart", "(Ljava/lang/String;Lcom/cppsipstack/SipCallRtp;)V" );
	if( m_jmEventCallStart == NULL )
	{
		AndroidErrorLog( "EventCallStart is not found" );
		return false;
	}
	
	m_jmEventCallEnd = env->GetStaticMethodID( m_jcSipUserAgent, "EventCallEnd", "(Ljava/lang/String;I)V" );
	if( m_jmEventCallEnd == NULL )
	{
		AndroidErrorLog( "EventCallEnd is not found" );
		return false;
	}

	m_jmEventReInvite = env->GetStaticMethodID( m_jcSipUserAgent, "EventReInvite", "(Ljava/lang/String;Lcom/cppsipstack/SipCallRtp;)V" );
	if( m_jmEventReInvite == NULL )
	{
		AndroidErrorLog( "EventReInvite is not found" );
		return false;
	}

	m_jmEventTransfer = env->GetStaticMethodID( m_jcSipUserAgent, "EventTransfer", "(Ljava/lang/String;Ljava/lang/String;Z)V" );
	if( m_jmEventTransfer == NULL )
	{
		AndroidErrorLog( "EventTransfer is not found" );
		return false;
	}

	m_jmEventBlindTransfer = env->GetStaticMethodID( m_jcSipUserAgent, "EventBlindTransfer", "(Ljava/lang/String;Ljava/lang/String;)V" );
	if( m_jmEventBlindTransfer == NULL )
	{
		AndroidErrorLog( "EventBlindTransfer is not found" );
		return false;
	}

	m_jmEventMessage = env->GetStaticMethodID( m_jcSipUserAgent, "EventMessage", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
	if( m_jmEventMessage == NULL )
	{
		AndroidErrorLog( "EventMessage is not found" );
		return false;
	}

	//
	m_jmSipServerInfoInit = env->GetMethodID( m_jcSipServerInfo, "<init>", "()V" );
	if( m_jmSipServerInfoInit == NULL )
	{
		AndroidErrorLog( "SipServerInfo<init> is not found" );
		return false;
	}

	m_jmSipCallRtpInit = env->GetMethodID( m_jcSipCallRtp, "<init>", "()V" );
	if( m_jmSipCallRtpInit == NULL )
	{
		AndroidErrorLog( "SipCallRtp<init> is not found" );
		return false;
	}

	return true;
}
