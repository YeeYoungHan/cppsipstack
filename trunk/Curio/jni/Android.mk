# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

					
LOCAL_MODULE    := SipUA_a
LOCAL_CPP_EXTENSION := .cpp
LOCAL_SRC_FILES :=  SdpParser/SdpAttribute.cpp \
					SdpParser/SdpBandWidth.cpp \
					SdpParser/SdpConnection.cpp \
					SdpParser/SdpMedia.cpp \
					SdpParser/SdpMessage.cpp \
					SdpParser/SdpOrigin.cpp \
					SdpParser/SdpTime.cpp \
					SipClient/SipClient.cpp \
					SipParser/SipAcceptData.cpp \
					SipParser/SipCallId.cpp \
					SipParser/SipChallenge.cpp \
					SipParser/SipContentType.cpp \
					SipParser/SipCredential.cpp \
					SipParser/SipCSeq.cpp \
					SipParser/SipFrom.cpp \
					SipParser/SipHeader.cpp \
					SipParser/SipMessage.cpp \
					SipParser/SipMutex.cpp \
					SipParser/SipParameter.cpp \
					SipParser/SipStatusCode.cpp \
					SipParser/SipUri.cpp \
					SipParser/SipUtility.cpp \
					SipParser/SipVia.cpp \
					SipStack/SipDeleteQueue.cpp \
					SipStack/SipICTList.cpp \
					SipStack/SipInviteTransaction.cpp \
					SipStack/SipISTList.cpp \
					SipStack/SipNICTList.cpp \
					SipStack/SipNISTList.cpp \
					SipStack/SipNonInviteTransaction.cpp \
					SipStack/SipStack.cpp \
					SipStack/SipStackSetup.cpp \
					SipStack/SipStackThread.cpp \
					SipStack/SipTransactionList.cpp \
					SipStack/SipUdp.cpp \
					SipStack/SipUdpThread.cpp \
					SipUserAgent/Md5.cpp \
					SipUserAgent/SipDialog.cpp \
					SipUserAgent/SipRegisterThread.cpp \
					SipUserAgent/SipServerInfo.cpp \
					SipUserAgent/SipUserAgent.cpp \
					TestSipParser/TestSdp.cpp \
					TestSipParser/TestSipFrom.cpp \
					TestSipParser/TestSipMessage.cpp \
					TestSipParser/TestSipParser.cpp \
					TestSipParser/TestSipUri.cpp \
					TestSipParser/TestSipVia.cpp \


LOCAL_CFLAGS	+= -I"./SIPSDK_SRC/" \
					-I"./OpenSSL/" \
		   			-D_CYGWIN \
		   			-D_LINUX \
					-D_ANDROID \
		   			-frtti		   			

LOCAL_LDLIBS     := OpenSSL_library/libcrypto.so -llog -lgcc
LOCAL_LDLIBS     += OpenSSL_library/libssl.so

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := SipUA_so
LOCAL_CPP_EXTENSION := .cpp
LOCAL_SRC_FILES :=  SdpParser/SdpAttribute.cpp \
					SdpParser/SdpBandWidth.cpp \
					SdpParser/SdpConnection.cpp \
					SdpParser/SdpMedia.cpp \
					SdpParser/SdpMessage.cpp \
					SdpParser/SdpOrigin.cpp \
					SdpParser/SdpTime.cpp \
					SipClient/SipClient.cpp \
					SipParser/SipAcceptData.cpp \
					SipParser/SipCallId.cpp \
					SipParser/SipChallenge.cpp \
					SipParser/SipContentType.cpp \
					SipParser/SipCredential.cpp \
					SipParser/SipCSeq.cpp \
					SipParser/SipFrom.cpp \
					SipParser/SipHeader.cpp \
					SipParser/SipMessage.cpp \
					SipParser/SipMutex.cpp \
					SipParser/SipParameter.cpp \
					SipParser/SipStatusCode.cpp \
					SipParser/SipUri.cpp \
					SipParser/SipUtility.cpp \
					SipParser/SipVia.cpp \
					SipStack/SipDeleteQueue.cpp \
					SipStack/SipICTList.cpp \
					SipStack/SipInviteTransaction.cpp \
					SipStack/SipISTList.cpp \
					SipStack/SipNICTList.cpp \
					SipStack/SipNISTList.cpp \
					SipStack/SipNonInviteTransaction.cpp \
					SipStack/SipStack.cpp \
					SipStack/SipStackSetup.cpp \
					SipStack/SipStackThread.cpp \
					SipStack/SipTransactionList.cpp \
					SipStack/SipUdp.cpp \
					SipStack/SipUdpThread.cpp \
					SipUserAgent/Md5.cpp \
					SipUserAgent/SipDialog.cpp \
					SipUserAgent/SipRegisterThread.cpp \
					SipUserAgent/SipServerInfo.cpp \
					SipUserAgent/SipUserAgent.cpp \
					TestSipParser/TestSdp.cpp \
					TestSipParser/TestSipFrom.cpp \
					TestSipParser/TestSipMessage.cpp \
					TestSipParser/TestSipParser.cpp \
					TestSipParser/TestSipUri.cpp \
					TestSipParser/TestSipVia.cpp \


LOCAL_CFLAGS	+= -I"./SIPSDK_SRC/" \
					-I"./OpenSSL/" \
		   			-D_CYGWIN \
		   			-D_LINUX \
					-D_ANDROID \
		   			-frtti		   			

LOCAL_LDLIBS     := OpenSSL_library/libcrypto.so -llog -lgcc
LOCAL_LDLIBS     += OpenSSL_library/libssl.so

LOCAL_STATIC_LIBRARIES := SipUA_a

include $(BUILD_SHARED_LIBRARY)
