#include "AndroidSipUserAgent.h"
#include "SipUserAgent.h"
#include "AndroidLog.h"

CSipUserAgent gclsUserAgent;

JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_InsertRegisterInfo( JNIEnv * env, jclass, jobject clsSipServerInfo )
{
	jclass cls = env->GetObjectClass( clsSipServerInfo );
	if( cls == NULL )
	{
		AndroidErrorLog( "clsSipServerInfo is not found" );
		return JNI_FALSE;
	}
	
	jfieldID fid = env->GetFieldID( cls, "m_strIp", "Ljava/lang/String;" );
	if( fid == NULL )
	{
		AndroidErrorLog( "fid is not found" );
		return JNI_FALSE;
	}
	
	jstring strIp = (jstring)env->GetObjectField( clsSipServerInfo, fid );
	const char * pszIp = env->GetStringUTFChars( strIp, NULL );
	
	AndroidDebugLog( "ip = [%s]", pszIp );
	
	env->ReleaseStringUTFChars( strIp, pszIp );
	
	return JNI_TRUE;
}

