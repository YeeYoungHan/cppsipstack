#include "AndroidSipUserAgent.h"
#include "SipUserAgent.h"
#include <android/log.h>

CSipUserAgent gclsUserAgent;

JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_InsertRegisterInfo( JNIEnv * env, jclass )
{
	
	return JNI_TRUE;
}

