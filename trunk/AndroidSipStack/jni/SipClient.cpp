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
static CSipMutex gclsMutex;
extern JavaVM * gjVm;

/**
 * @ingroup SipClient
 * @brief SIP REGISTER ���� �޽��� ���� �̺�Ʈ �ڵ鷯
 * @param pclsInfo	SIP REGISTER ���� �޽����� ������ IP-PBX ���� ���� ��ü
 * @param iStatus		SIP REGISTER ���� �ڵ�
 */
void CSipClient::EventRegister( CSipServerInfo * pclsInfo, int iStatus )
{
	JNIEnv * env;
	int iRet;
	jobject joSipServerInfo = NULL;

	gclsMutex.acquire();
#ifdef WIN32
	iRet = gjVm->AttachCurrentThread( (void **)&env, NULL );
#else
	iRet = gjVm->AttachCurrentThread( &env, NULL );
#endif

	AndroidDebugLog( "%s AttachCurrentThread return(%d)", __FUNCTION__, iRet );

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

	gjVm->DetachCurrentThread();
	gclsMutex.release();
}

/**
 * @ingroup SipClient
 * @brief SIP ��ȭ ��û ���ſ� ���� ���� Ȯ�� �̺�Ʈ �ڵ鷯
 * @param pclsMessage	SIP INVITE ��û �޽���
 * @return ������ �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipClient::EventIncomingRequestAuth( CSipMessage * pclsMessage )
{
	return true;
}

/**
 * @ingroup SipClient
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
	int iRet;

	gclsMutex.acquire();
#ifdef WIN32
	iRet = gjVm->AttachCurrentThread( (void **)&env, NULL );
#else
	iRet = gjVm->AttachCurrentThread( &env, NULL );
#endif

	AndroidDebugLog( "%s AttachCurrentThread return(%d)", __FUNCTION__, iRet );

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

	gjVm->DetachCurrentThread();
	gclsMutex.release();
}

/**
 * @ingroup SipClient
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
	int iRet;

	gclsMutex.acquire();
#ifdef WIN32
	iRet = gjVm->AttachCurrentThread( (void **)&env, NULL );
#else
	iRet = gjVm->AttachCurrentThread( &env, NULL );
#endif

	AndroidDebugLog( "%s AttachCurrentThread return(%d) (%p)", __FUNCTION__, iRet, env );

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

	gjVm->DetachCurrentThread();
	gclsMutex.release();	
}

/**
 * @ingroup SipClient
 * @brief SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP ���� ���� ��ü
 */
void CSipClient::EventCallStart( const char * pszCallId, CSipCallRtp * pclsRtp )
{
	jstring jstrCallId = NULL;
	jobject joSipCallRtp = NULL;

	JNIEnv * env;
	int iRet;

	gclsMutex.acquire();
#ifdef WIN32
	iRet = gjVm->AttachCurrentThread( (void **)&env, NULL );
#else
	iRet = gjVm->AttachCurrentThread( &env, NULL );
#endif

	AndroidDebugLog( "%s AttachCurrentThread return(%d)", __FUNCTION__, iRet );

	jstrCallId = env->NewStringUTF( pszCallId );
	if( jstrCallId == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszCallId );
		goto FUNC_END;
	}

	AndroidDebugLog( "1" );

	joSipCallRtp = env->NewObject( gclsClass.m_jcSipCallRtp, gclsClass.m_jmSipCallRtpInit );
	if( joSipCallRtp == NULL )
	{
		AndroidErrorLog( "%s NewObject error", __FUNCTION__ );
		goto FUNC_END;
	}

	AndroidDebugLog( "2" );

	if( pclsRtp )
	{
		if( PutSipCallRtp( env, joSipCallRtp, *pclsRtp ) == false )
		{
			AndroidErrorLog( "%s PutSipCallRtp error", __FUNCTION__ );
			goto FUNC_END;
		}
	}

	AndroidDebugLog( "3" );

	env->CallStaticVoidMethod( gclsClass.m_jcSipUserAgent, gclsClass.m_jmEventCallStart, jstrCallId, joSipCallRtp );

FUNC_END:
	if( jstrCallId ) env->DeleteLocalRef( jstrCallId );
	if( joSipCallRtp ) env->DeleteLocalRef( joSipCallRtp );

	gjVm->DetachCurrentThread();
	gclsMutex.release();	
}

/**
 * @ingroup SipClient
 * @brief SIP ��ȭ ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId		SIP Call-ID
 * @param iSipStatus	SIP ���� �ڵ�. INVITE �� ���� ���� �������� ��ȭ�� ����� ���, INVITE �� ���� �ڵ带 �����Ѵ�.
 */
void CSipClient::EventCallEnd( const char * pszCallId, int iSipStatus )
{
	jstring jstrCallId = NULL;

	JNIEnv * env;
	int iRet;

	gclsMutex.acquire();
#ifdef WIN32
	iRet = gjVm->AttachCurrentThread( (void **)&env, NULL );
#else
	iRet = gjVm->AttachCurrentThread( &env, NULL );
#endif

	AndroidDebugLog( "%s AttachCurrentThread return(%d)", __FUNCTION__, iRet );

	jstrCallId = env->NewStringUTF( pszCallId );
	if( jstrCallId == NULL )
	{
		AndroidErrorLog( "%s NewStringUTF(%s) error", __FUNCTION__, pszCallId );
		goto FUNC_END;
	}

	env->CallStaticVoidMethod( gclsClass.m_jcSipUserAgent, gclsClass.m_jmEventCallEnd, jstrCallId, iSipStatus );

FUNC_END:
	if( jstrCallId ) env->DeleteLocalRef( jstrCallId );

	gjVm->DetachCurrentThread();
	gclsMutex.release();	
}

/**
 * @ingroup SipClient
 * @brief SIP ReINVITE ���� �̺�Ʈ �ڵ鷯
 * @param	pszCallId	SIP Call-ID
 * @param pclsRtp		RTP ���� ���� ��ü
 */
void CSipClient::EventReInvite( const char * pszCallId, CSipCallRtp * pclsRtp )
{
}

/**
 * @ingroup SipClient
 * @brief Screened / Unscreened Transfer ��û ���� �̺�Ʈ �ڵ鷯
 * @param pszCallId					SIP Call-ID
 * @param pszReferToCallId	��ȭ�� ���޵� SIP Call-ID
 * @param bScreenedTransfer Screened Transfer �̸� true �� �Էµǰ� Unscreened Transfer �̸� false �� �Էµȴ�.
 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipClient::EventTransfer( const char * pszCallId, const char * pszReferToCallId, bool bScreenedTransfer )
{
	return false;
}

/**
 * @ingroup SipClient
 * @brief Blind Transfer ��û ���� �̺�Ʈ �ڵ鷯
 * @param pszCallId			SIP Call-ID
 * @param pszReferToId	��ȭ�� ���޵� ����� ���̵�
 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipClient::EventBlindTransfer( const char * pszCallId, const char * pszReferToId )
{
	return false;
}

/**
 * @ingroup SipClient
 * @brief SIP MESSAGE ���� �̺�Ʈ �ڵ鷯
 * @param pszFrom			SIP �޽��� ���� ���̵�
 * @param pszTo				SIP �޽��� ���� ���̵�
 * @param pclsMessage SIP �޽���
 * @returns ��û�� �����ϸ� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool CSipClient::EventMessage( const char * pszFrom, const char * pszTo, CSipMessage * pclsMessage )
{
	return true;
}
