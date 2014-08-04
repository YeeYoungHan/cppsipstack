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

#include "SipClient.h"
#include "AndroidClass.h"
#include "AndroidLog.h"
#include "AndroidClassConvert.h"
#include <time.h>

CSipClient gclsSipClient;
extern JavaVM * gjVm;

/**
 * @ingroup AndroidSipStack
 * @brief SIP REGISTER ���� �޽��� ���� �̺�Ʈ �ڵ鷯
 * @param pclsInfo	SIP REGISTER ���� �޽����� ������ IP-PBX ���� ���� ��ü
 * @param iStatus		SIP REGISTER ���� �ڵ�
 */
void CSipClient::EventRegister( CSipServerInfo * pclsInfo, int iStatus )
{
	JNIEnv * env;
	jobject joSipServerInfo = NULL;

	if( AttachCurrentThread( &env ) == false ) return;

	joSipServerInfo = env->NewObject( gclsClass.m_jcSipServerInfo, gclsClass.m_jmSipServerInfoInit );
	if( joSipServerInfo == NULL )
	{
		AndroidErrorLog( "%s NewObject error", __FUNCTION__ );
		goto FUNC_END;
	}

	if( PutSipServerInfo( env, joSipServerInfo, *pclsInfo ) == false )
	{
		AndroidErrorLog( "%s PutSipServerInfo error", __FUNCTION__ );
		goto FUNC_END;
	}

	env->CallStaticVoidMethod( gclsClass.m_jcSipUserAgent, gclsClass.m_jmEventRegister, joSipServerInfo, iStatus );

FUNC_END:
	if( joSipServerInfo ) env->DeleteLocalRef( joSipServerInfo );
}

/**
 * @ingroup AndroidSipStack
 * @brief SIP ��ȭ ��û ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId	SIP Call-ID
 * @param pszFrom		SIP From ����� ���̵�
 * @param pszTo			SIP To ����� ���̵�
 * @param pclsRtp		RTP ���� ���� ��ü
 */
void CSipClient::EventIncomingCall( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
{
	jstring jstrCallId = NULL, jstrFrom = NULL, jstrTo = NULL;
	jobject joSipCallRtp = NULL;
	JNIEnv * env;

	if( AttachCurrentThread( &env ) == false ) return;

	jstrCallId = env->NewStringUTF( pszCallId );
	if( jstrCallId == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszCallId );
		goto FUNC_END;
	}

	jstrFrom = env->NewStringUTF( pszFrom );
	if( jstrFrom == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszFrom );
		goto FUNC_END;
	}

	jstrTo = env->NewStringUTF( pszTo );
	if( jstrTo == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszTo );
		goto FUNC_END;
	}

	joSipCallRtp = env->NewObject( gclsClass.m_jcSipCallRtp, gclsClass.m_jmSipCallRtpInit );
	if( joSipCallRtp == NULL )
	{
		AndroidErrorLog( "%s NewObject error", __FUNCTION__ );
		goto FUNC_END;
	}

	if( PutSipCallRtp( env, joSipCallRtp, *pclsRtp ) == false )
	{
		AndroidErrorLog( "%s PutSipCallRtp error", __FUNCTION__ );
		goto FUNC_END;
	}

	env->CallStaticVoidMethod( gclsClass.m_jcSipUserAgent, gclsClass.m_jmEventIncomingCall, jstrCallId, jstrFrom, jstrTo, joSipCallRtp );

FUNC_END:
	if( jstrCallId ) env->DeleteLocalRef( jstrCallId );
	if( jstrFrom ) env->DeleteLocalRef( jstrFrom );
	if( jstrTo ) env->DeleteLocalRef( jstrTo );
	if( joSipCallRtp ) env->DeleteLocalRef( joSipCallRtp );
}

/**
 * @ingroup AndroidSipStack
 * @brief SIP Ring / Session Progress ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP ���� �ڵ�
 * @param pclsRtp			RTP ���� ���� ��ü
 */
void CSipClient::EventCallRing( const char * pszCallId, int iSipStatus, CSipCallRtp * pclsRtp )
{
	jstring jstrCallId = NULL;
	jobject joSipCallRtp = NULL;
	JNIEnv * env = NULL;

	if( AttachCurrentThread( &env ) == false ) return;

	jstrCallId = env->NewStringUTF( pszCallId );
	if( jstrCallId == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszCallId );
		goto FUNC_END;
	}

	joSipCallRtp = env->NewObject( gclsClass.m_jcSipCallRtp, gclsClass.m_jmSipCallRtpInit );
	if( joSipCallRtp == NULL )
	{
		AndroidErrorLog( "%s NewObject error", __FUNCTION__ );
		goto FUNC_END;
	}

	if( pclsRtp )
	{
		if( PutSipCallRtp( env, joSipCallRtp, *pclsRtp ) == false )
		{
			AndroidErrorLog( "%s PutSipCallRtp error", __FUNCTION__ );
			goto FUNC_END;
		}
	}

	env->CallStaticVoidMethod( gclsClass.m_jcSipUserAgent, gclsClass.m_jmEventCallRing, jstrCallId, iSipStatus, joSipCallRtp );

FUNC_END:
	if( jstrCallId ) env->DeleteLocalRef( jstrCallId );
	if( joSipCallRtp ) env->DeleteLocalRef( joSipCallRtp );
}

/**
 * @ingroup AndroidSipStack
 * @brief SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP ���� ���� ��ü
 */
void CSipClient::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	jstring jstrCallId = NULL;
	jobject joSipCallRtp = NULL;
	JNIEnv * env;

	if( AttachCurrentThread( &env ) == false ) return;

	jstrCallId = env->NewStringUTF( pszCallId );
	if( jstrCallId == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszCallId );
		goto FUNC_END;
	}

	joSipCallRtp = env->NewObject( gclsClass.m_jcSipCallRtp, gclsClass.m_jmSipCallRtpInit );
	if( joSipCallRtp == NULL )
	{
		AndroidErrorLog( "%s NewObject error", __FUNCTION__ );
		goto FUNC_END;
	}

	if( pclsRtp )
	{
		if( PutSipCallRtp( env, joSipCallRtp, *pclsRtp ) == false )
		{
			AndroidErrorLog( "%s PutSipCallRtp error", __FUNCTION__ );
			goto FUNC_END;
		}
	}

	env->CallStaticVoidMethod( gclsClass.m_jcSipUserAgent, gclsClass.m_jmEventCallStart, jstrCallId, joSipCallRtp );

FUNC_END:
	if( jstrCallId ) env->DeleteLocalRef( jstrCallId );
	if( joSipCallRtp ) env->DeleteLocalRef( joSipCallRtp );
}

/**
 * @ingroup AndroidSipStack
 * @brief SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP ���� �ڵ�. INVITE �� ���� ���� �������� ��ȭ�� ����� ���, INVITE �� ���� �ڵ带 �����Ѵ�.
 */
void CSipClient::EventCallEnd( const char * pszCallId, int iSipStatus )
{
	jstring jstrCallId = NULL;
	JNIEnv * env;

	if( AttachCurrentThread( &env ) == false ) return;

	jstrCallId = env->NewStringUTF( pszCallId );
	if( jstrCallId == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszCallId );
		goto FUNC_END;
	}

	env->CallStaticVoidMethod( gclsClass.m_jcSipUserAgent, gclsClass.m_jmEventCallEnd, jstrCallId, iSipStatus );

FUNC_END:
	if( jstrCallId ) env->DeleteLocalRef( jstrCallId );
}

/**
 * @ingroup AndroidSipStack
 * @brief SIP ReINVITE ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId	SIP Call-ID
 * @param pclsRemoteRtp		���� RTP ���� ���� ��ü
 * @param pclsLocalRtp		�� RTP ���� ���� ��ü
 */
void CSipClient::EventReInvite( const char * pszCallId, CSipCallRtp * pclsRemoteRtp, CSipCallRtp * pclsLocalRtp )
{
	jstring jstrCallId = NULL;
	jobject joRemoteRtp = NULL, joLocalRtp = NULL;
	JNIEnv * env;

	if( AttachCurrentThread( &env ) == false ) return;

	jstrCallId = env->NewStringUTF( pszCallId );
	if( jstrCallId == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszCallId );
		goto FUNC_END;
	}

	joRemoteRtp = env->NewObject( gclsClass.m_jcSipCallRtp, gclsClass.m_jmSipCallRtpInit );
	if( joRemoteRtp == NULL )
	{
		AndroidErrorLog( "%s NewObject error", __FUNCTION__ );
		goto FUNC_END;
	}

	joLocalRtp = env->NewObject( gclsClass.m_jcSipCallRtp, gclsClass.m_jmSipCallRtpInit );
	if( joLocalRtp == NULL )
	{
		AndroidErrorLog( "%s NewObject error", __FUNCTION__ );
		goto FUNC_END;
	}

	if( pclsRemoteRtp )
	{
		if( PutSipCallRtp( env, joRemoteRtp, *pclsRemoteRtp ) == false )
		{
			AndroidErrorLog( "%s PutSipCallRtp error", __FUNCTION__ );
			goto FUNC_END;
		}
	}

	if( pclsLocalRtp )
	{
		if( PutSipCallRtp( env, joLocalRtp, *pclsLocalRtp ) == false )
		{
			AndroidErrorLog( "%s PutSipCallRtp error", __FUNCTION__ );
			goto FUNC_END;
		}
	}

	env->CallStaticVoidMethod( gclsClass.m_jcSipUserAgent, gclsClass.m_jmEventReInvite, jstrCallId, joRemoteRtp, joLocalRtp );

FUNC_END:
	if( jstrCallId ) env->DeleteLocalRef( jstrCallId );
	if( joRemoteRtp ) env->DeleteLocalRef( joRemoteRtp );
	if( joLocalRtp ) env->DeleteLocalRef( joLocalRtp );
}

/**
 * @ingroup AndroidSipStack
 * @brief SIP PRACK ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP ���� ���� ��ü
 */
void CSipClient::EventPrack( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	jstring jstrCallId = NULL;
	jobject joSipCallRtp = NULL;
	JNIEnv * env;

	if( AttachCurrentThread( &env ) == false ) return;

	jstrCallId = env->NewStringUTF( pszCallId );
	if( jstrCallId == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszCallId );
		goto FUNC_END;
	}

	joSipCallRtp = env->NewObject( gclsClass.m_jcSipCallRtp, gclsClass.m_jmSipCallRtpInit );
	if( joSipCallRtp == NULL )
	{
		AndroidErrorLog( "%s NewObject error", __FUNCTION__ );
		goto FUNC_END;
	}

	if( pclsRtp )
	{
		if( PutSipCallRtp( env, joSipCallRtp, *pclsRtp ) == false )
		{
			AndroidErrorLog( "%s PutSipCallRtp error", __FUNCTION__ );
			goto FUNC_END;
		}
	}

	env->CallStaticVoidMethod( gclsClass.m_jcSipUserAgent, gclsClass.m_jmEventPrack, jstrCallId, joSipCallRtp );

FUNC_END:
	if( jstrCallId ) env->DeleteLocalRef( jstrCallId );
	if( joSipCallRtp ) env->DeleteLocalRef( joSipCallRtp );
}

/**
 * @ingroup AndroidSipStack
 * @brief Screened / Unscreened Transfer ��û ���� �̺�Ʈ �ڵ鷯
 * @param pszCallId					SIP Call-ID
 * @param pszReferToCallId	��ȭ�� ���޵� SIP Call-ID
 * @param bScreenedTransfer Screened Transfer �̸� true �� �Էµǰ� Unscreened Transfer �̸� false �� �Էµȴ�.
 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipClient::EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer )
{
	jstring jstrCallId = NULL, jstrReferToCallId = NULL;
	jboolean jbScreenedTransfer = JNI_FALSE;
	JNIEnv * env;

	if( AttachCurrentThread( &env ) == false ) return true;

	jstrCallId = env->NewStringUTF( pszCallId );
	if( jstrCallId == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszCallId );
		goto FUNC_END;
	}

	jstrReferToCallId = env->NewStringUTF( pszReferToCallId );
	if( jstrReferToCallId == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszReferToCallId );
		goto FUNC_END;
	}

	if( bScreenedTransfer )
	{
		jbScreenedTransfer = JNI_TRUE;
	}

	env->CallStaticVoidMethod( gclsClass.m_jcSipUserAgent, gclsClass.m_jmEventTransfer, jstrCallId, jstrReferToCallId, jbScreenedTransfer );

FUNC_END:
	if( jstrCallId ) env->DeleteLocalRef( jstrCallId );
	if( jstrReferToCallId ) env->DeleteLocalRef( jstrReferToCallId );

	return true;
}

/**
 * @ingroup AndroidSipStack
 * @brief Blind Transfer ��û ���� �̺�Ʈ �ڵ鷯
 * @param pszCallId			SIP Call-ID
 * @param pszReferToId	��ȭ�� ���޵� ����� ���̵�
 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipClient::EventBlindTransfer( const char * pszCallId, const char * pszReferToId )
{
	jstring jstrCallId = NULL, jstrReferToId = NULL;
	JNIEnv * env;

	if( AttachCurrentThread( &env ) == false ) return true;

	jstrCallId = env->NewStringUTF( pszCallId );
	if( jstrCallId == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszCallId );
		goto FUNC_END;
	}

	jstrReferToId = env->NewStringUTF( pszReferToId );
	if( jstrReferToId == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszReferToId );
		goto FUNC_END;
	}

	env->CallStaticVoidMethod( gclsClass.m_jcSipUserAgent, gclsClass.m_jmEventBlindTransfer, jstrCallId, jstrReferToId );

FUNC_END:
	if( jstrCallId ) env->DeleteLocalRef( jstrCallId );
	if( jstrReferToId ) env->DeleteLocalRef( jstrReferToId );

	return true;
}

/**
 * @ingroup AndroidSipStack
 * @brief SIP MESSAGE ���� �̺�Ʈ �ڵ鷯
 * @param pszFrom			SIP �޽��� ���� ���̵�
 * @param pszTo				SIP �޽��� ���� ���̵�
 * @param pclsMessage SIP �޽���
 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipClient::EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage )
{
	if( pclsMessage->m_strBody.length() == 0 ) return false;

	jstring jstrFrom = NULL, jstrTo = NULL, jstrSms = NULL;
	JNIEnv * env;

	if( AttachCurrentThread( &env ) == false ) return true;

	jstrFrom = env->NewStringUTF( pszFrom );
	if( jstrFrom == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszFrom );
		goto FUNC_END;
	}

	jstrTo = env->NewStringUTF( pszTo );
	if( jstrTo == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszTo );
		goto FUNC_END;
	}

	jstrSms = env->NewStringUTF( pclsMessage->m_strBody.c_str() );
	if( jstrSms == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, jstrSms );
		goto FUNC_END;
	}

	env->CallStaticVoidMethod( gclsClass.m_jcSipUserAgent, gclsClass.m_jmEventMessage, jstrFrom, jstrTo, jstrSms );

FUNC_END:
	if( jstrFrom ) env->DeleteLocalRef( jstrFrom );
	if( jstrTo ) env->DeleteLocalRef( jstrTo );
	if( jstrSms ) env->DeleteLocalRef( jstrSms );

	return true;
}

/**
 * @ingroup AndroidSipStack
 * @brief SIP �޽��� ���� �����尡 ������� �˷��ִ� �̺�Ʈ �ڵ鷯
 * @param iThreadId UDP ������ ��ȣ
 */
void CSipClient::EventThreadEnd( int iThreadId )
{
	JNIEnv * env;

	if( AttachCurrentThread( &env ) == false ) return;

	gjVm->DetachCurrentThread();
}

bool CSipClient::AttachCurrentThread( JNIEnv ** env )
{
	int iRet;

#ifdef WIN32
	iRet = gjVm->AttachCurrentThread( (void **)env, NULL );
#else
	iRet = gjVm->AttachCurrentThread( env, NULL );
#endif

	if( iRet != 0 )
	{
		AndroidErrorLog( "%s AttachCurrentThread return(%d)", __FUNCTION__, iRet );
		return false;
	}

	return true;
}
