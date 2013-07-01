#ifndef _ANDROID_SIP_USER_AGENT_H_
#define _ANDROID_SIP_USER_AGENT_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jboolean JNICALL Java_com_cppsipstack_SipUserAgent_InsertRegisterInfo( JNIEnv * env, jclass, jobject clsSipServerInfo );

#ifdef __cplusplus
}
#endif
#endif

