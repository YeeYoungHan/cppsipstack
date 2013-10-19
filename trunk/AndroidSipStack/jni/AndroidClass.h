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

#ifndef _ANDROID_CLASS_H_
#define _ANDROID_CLASS_H_

#include <jni.h>

/**
 * @brief java 에서 사용하는 클래스 및 callback 메소드 저장 클래스
 */
class CAndroidClass
{
public:
	CAndroidClass();
	~CAndroidClass();

	bool Init( JNIEnv * env );

	jclass m_jcSipUserAgent;
	jclass m_jcSipServerInfo;
	jclass m_jcSipCallRtp;
	
	jmethodID m_jmEventRegister;
	jmethodID m_jmEventIncomingCall;
	jmethodID m_jmEventCallRing;
	jmethodID m_jmEventCallStart;
	jmethodID m_jmEventCallEnd;
	jmethodID m_jmEventReInvite;
	jmethodID m_jmEventBlindTransfer;
	
	jmethodID m_jmSipServerInfoInit;
	jmethodID m_jmSipCallRtpInit;
};

extern CAndroidClass gclsClass;

#endif
