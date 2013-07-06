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

bool CAndroidClass::Init( JNIEnv * env )
{
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
