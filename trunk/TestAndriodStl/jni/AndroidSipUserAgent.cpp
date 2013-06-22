#include "AndroidSipUserAgent.h"
#include <android/log.h>
#include <string>

JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_InsertRegisterInfo( JNIEnv * env, jclass )
{
	std::string strName = "";
	
	printf( "%s\n", strName.c_str() );
	
	return JNI_TRUE;
}

