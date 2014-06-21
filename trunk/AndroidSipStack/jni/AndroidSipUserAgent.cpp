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
LogCallBack gclsLogCallBack;

bool GetSipCallRoute( CSipCallRoute & clsRoute )
{
	SIP_SERVER_INFO_LIST::iterator	itList;

	for( itList = gclsUserAgent.m_clsRegisterList.begin(); itList != gclsUserAgent.m_clsRegisterList.end(); ++itList )
	{
		if( itList->m_bLogin )
		{
			clsRoute.m_strDestIp = itList->m_strIp;
			clsRoute.m_iDestPort = itList->m_iPort;
			return true;
		}
	}

	return false;
}

/**
 * @ingroup AndroidSipStack
 * @brief SIP ���� ������ �����Ѵ�.
 * @param env							JNIEnv
 * @param jcSipUserAgent	java SipUserAgent Ŭ����
 * @param joSipServerInfo java SipServerInfo ��ü
 * @returns �����ϸ� JNI_TRUE �� �����ϰ� �����ϸ� JNI_FALSE �� �����Ѵ�.
 */
JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_InsertRegisterInfo( JNIEnv * env, jclass, jobject joSipServerInfo )
{
	CSipServerInfo clsSipServerInfo;

	if( GetSipServerInfo( env, joSipServerInfo, clsSipServerInfo ) == false ) return JNI_FALSE;

	gclsUserAgent.InsertRegisterInfo( clsSipServerInfo );

	return JNI_TRUE;
}

/**
 * @ingroup AndroidSipStack
 * @brief SIP stack �� �����Ѵ�.
 * @param env							JNIEnv
 * @param jcSipUserAgent	java SipUserAgent Ŭ����
 * @param joSipStackSetup java SipStackSetup ��ü
 * @returns �����ϸ� JNI_TRUE �� �����ϰ� �����ϸ� JNI_FALSE �� �����Ѵ�.
 */
JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_Start( JNIEnv * env, jclass jcSipUserAgent, jobject joSipStackSetup )
{
	if( gclsClass.Init( env ) == false )
	{
		AndroidErrorLog( "Start - gclsClass.Init error" );
		return JNI_FALSE;
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
	CLog::SetCallBack( &gclsLogCallBack );

	if( gclsUserAgent.Start( clsSetup, &gclsSipClient, NULL ) == false ) 
	{
		AndroidErrorLog( "Start - Start error" );
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

/**
 * @ingroup AndroidSipStack
 * @brief SIP stack �� �����Ѵ�.
 * @param env							JNIEnv
 * @param jcSipUserAgent	java SipUserAgent Ŭ����
 * @returns �����ϸ� JNI_TRUE �� �����ϰ� �����ϸ� JNI_FALSE �� �����Ѵ�.
 */
JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_Stop( JNIEnv * env, jclass )
{
	gclsUserAgent.Stop();

	return JNI_TRUE;
}

/**
 * @ingroup AndroidSipStack
 * @brief ��ȭ ��ȭ ��û�Ѵ�.
 * @param env							JNIEnv
 * @param jcSipUserAgent	java SipUserAgent Ŭ����
 * @param strFrom					�߽��� ���̵�
 * @param strTo						������ ���̵�
 * @param joSipCallRtp		RTP ����
 * @returns �����ϸ� SIP call-id �� ������ ���ڿ� ��ü�� �����ϰ� �����ϸ� NULL �� �����Ѵ�.
 */
JNIEXPORT jstring JNICALL Java_com_cppsipstack_SipUserAgent_StartCall( JNIEnv * env, jclass, jstring jsFrom, jstring jsTo, jobject joSipCallRtp )
{
	std::string	strFrom, strTo, strCallId;
	CSipCallRtp clsRtp;
	CSipCallRoute clsRoute;

	if( GetString( env, jsFrom, strFrom ) == false ) return NULL;
	if( GetString( env, jsTo, strTo ) == false ) return NULL;
	if( GetSipCallRtp( env, joSipCallRtp, clsRtp ) == false ) return NULL;

	if( GetSipCallRoute( clsRoute ) == false )
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

	return jsCallId;
}

/**
 * @ingroup AndroidSipStack
 * @brief ��ȭ ��ȭ�� �����Ѵ�.
 * @param env							JNIEnv
 * @param jcSipUserAgent	java SipUserAgent Ŭ����
 * @param strCallId				SIP call-id
 * @param iSipCode				SIP status code
 * @returns �����ϸ� JNI_TRUE �� �����ϰ� �����ϸ� JNI_FALSE �� �����Ѵ�.
 */
JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_StopCall( JNIEnv * env, jclass, jstring jsCallId, jint iSipCode )
{
	std::string	strCallId;

	if( GetString( env, jsCallId, strCallId ) == false ) return JNI_FALSE;

	if( gclsUserAgent.StopCall( strCallId.c_str(), iSipCode ) == false )
	{
		AndroidErrorLog( "StopCall - gclsUserAgent.StopCall error" );
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

/**
 * @ingroup AndroidSipStack
 * @brief ��ȭ ��û�� �����Ѵ�.
 * @param env							JNIEnv
 * @param jcSipUserAgent	java SipUserAgent Ŭ����
 * @param strCallId				SIP call-id
 * @param joSipCallRtp		RTP ����
 * @returns �����ϸ� JNI_TRUE �� �����ϰ� �����ϸ� JNI_FALSE �� �����Ѵ�.
 */
JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_AcceptCall( JNIEnv * env, jclass, jstring jsCallId, jobject joSipCallRtp )
{
	std::string	strCallId;
	CSipCallRtp clsRtp;

	if( GetString( env, jsCallId, strCallId ) == false ) return JNI_FALSE;
	if( GetSipCallRtp( env, joSipCallRtp, clsRtp ) == false ) return JNI_FALSE;

	if( gclsUserAgent.AcceptCall( strCallId.c_str(), &clsRtp ) == false )
	{
		AndroidErrorLog( "StopCall - gclsUserAgent.AcceptCall error" );
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

/**
 * @ingroup AndroidSipStack
 * @brief Blind transfer �� �����Ѵ�.
 * @param env							JNIEnv
 * @param jcSipUserAgent	java SipUserAgent Ŭ����
 * @param strCallId				SIP call-id
 * @param jsTo						��ȭ ������ ���� ���̵�
 * @returns �����ϸ� JNI_TRUE �� �����ϰ� �����ϸ� JNI_FALSE �� �����Ѵ�.
 */
JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_TransferCallBlind( JNIEnv * env, jclass, jstring jsCallId, jstring jsTo )
{
	std::string	strCallId, strTo;

	if( GetString( env, jsCallId, strCallId ) == false ) return JNI_FALSE;
	if( GetString( env, jsTo, strTo ) == false ) return JNI_FALSE;

	if( gclsUserAgent.TransferCallBlind( strCallId.c_str(), strTo.c_str() ) == false )
	{
		AndroidErrorLog( "StopCall - gclsUserAgent.TransferCallBlind error" );
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

/**
 * @ingroup AndroidSipStack
 * @brief Screened / Unscreened transfer �� �����Ѵ�.
 * @param env							JNIEnv
 * @param jcSipUserAgent	java SipUserAgent Ŭ����
 * @param strCallId				SIP call-id
 * @param jsToCallId			��ȭ ������ ���� SIP Call-ID
 * @returns �����ϸ� JNI_TRUE �� �����ϰ� �����ϸ� JNI_FALSE �� �����Ѵ�.
 */
JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_TransferCall( JNIEnv * env, jclass, jstring jsCallId, jstring jsToCallId )
{
	std::string	strCallId, strToCallId;

	if( GetString( env, jsCallId, strCallId ) == false ) return JNI_FALSE;
	if( GetString( env, jsToCallId, strToCallId ) == false ) return JNI_FALSE;

	if( gclsUserAgent.TransferCall( strCallId.c_str(), strToCallId.c_str() ) == false )
	{
		AndroidErrorLog( "StopCall - gclsUserAgent.TransferCall error" );
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

/**
 * @ingroup AndroidSipStack
 * @brief ReINVITE �޽����� �����Ѵ�.
 * @param env							JNIEnv
 * @param jcSipUserAgent	java SipUserAgent Ŭ����
 * @param strCallId				SIP call-id
 * @param joSipCallRtp		RTP ����
 * @returns �����ϸ� JNI_TRUE �� �����ϰ� �����ϸ� JNI_FALSE �� �����Ѵ�.
 */
JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_SendReInvite( JNIEnv * env, jclass, jstring jsCallId, jobject joSipCallRtp )
{
	std::string	strCallId;
	CSipCallRtp clsRtp;

	if( GetString( env, jsCallId, strCallId ) == false ) return JNI_FALSE;
	if( GetSipCallRtp( env, joSipCallRtp, clsRtp ) == false ) return JNI_FALSE;

	if( gclsUserAgent.SendReInvite( strCallId.c_str(), &clsRtp ) == false )
	{
		AndroidErrorLog( "StopCall - gclsUserAgent.SendReInvite error" );
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

/**
 * @ingroup AndroidSipStack
 * @brief text ��� SMS �޽����� �����Ѵ�.
 * @param env							JNIEnv
 * @param jcSipUserAgent	java SipUserAgent Ŭ����
 * @param jsFrom					�߽��� ���̵�
 * @param jsTo						������ ���̵�
 * @param jsText					�ؽ�Ʈ �޽���
 * @returns �����ϸ� JNI_TRUE �� �����ϰ� �����ϸ� JNI_FALSE �� �����Ѵ�.
 */
JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_SendSms( JNIEnv * env, jclass, jstring jsFrom, jstring jsTo, jstring jsText )
{
	std::string	strFrom, strTo, strText;
	CSipCallRtp clsRtp;
	CSipCallRoute clsRoute;

	if( GetString( env, jsFrom, strFrom ) == false ) return NULL;
	if( GetString( env, jsTo, strTo ) == false ) return NULL;
	if( GetString( env, jsTo, strText ) == false ) return NULL;

	if( GetSipCallRoute( clsRoute ) == false )
	{
		AndroidErrorLog( "SendSms - call route is not found" );
		return JNI_FALSE;
	}

	if( gclsUserAgent.SendSms( strFrom.c_str(), strTo.c_str(), strText.c_str(), &clsRoute ) == false )
	{
		AndroidErrorLog( "SendSms - gclsUserAgent.SendSms error" );
		return JNI_FALSE;
	}

	return JNI_TRUE;
}
