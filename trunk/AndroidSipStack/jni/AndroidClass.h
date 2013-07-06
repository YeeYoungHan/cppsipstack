#ifndef _ANDROID_CLASS_H_
#define _ANDROID_CLASS_H_

#include <jni.h>

class CAndroidClass
{
public:
	CAndroidClass();
	~CAndroidClass();

	bool Init( JNIEnv * env );

	JNIEnv * m_jEnv;
	jclass m_jcSipUserAgent;
	jclass m_jcSipServerInfo;
	jclass m_jcSipCallRtp;
	
	jmethodID m_jmEventRegister;
	jmethodID m_jmEventIncomingCall;
	jmethodID m_jmEventCallRing;
	jmethodID m_jmEventCallStart;
	jmethodID m_jmEventCallEnd;
	
	jmethodID m_jmSipServerInfoInit;
	jmethodID m_jmSipCallRtpInit;
};

extern CAndroidClass gclsClass;

#endif
