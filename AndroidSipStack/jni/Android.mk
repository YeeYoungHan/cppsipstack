# Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
#


##############################################################################
# SipPlatform

LOCAL_PATH := ../../SipPlatform

include $(CLEAR_VARS)

LOCAL_MODULE    := libsipplatform
LOCAL_SRC_FILES := Directory.cpp Log.cpp ServerUtility.cpp \
	SipMd5.cpp SipMutex.cpp \
	SipTcp.cpp SipUdp.cpp \
	StringUtility.cpp StringMap.cpp \
	TimeString.cpp TimeUtility.cpp

include $(BUILD_STATIC_LIBRARY)

##############################################################################
# SipParser

LOCAL_PATH := ../../SipParser

include $(CLEAR_VARS)

LOCAL_MODULE    := libsipparser
LOCAL_SRC_FILES := SipAcceptData.cpp SipCallId.cpp SipChallenge.cpp SipContentType.cpp \
	SipCredential.cpp SipCSeq.cpp SipFrom.cpp SipHeader.cpp SipMessage.cpp \
	SipParameter.cpp SipUri.cpp SipVia.cpp SipStatusCode.cpp SipReason.cpp \
	SipUtility.cpp SipParameterList.cpp
LOCAL_C_INCLUDES := ../../SipPlatform

include $(BUILD_STATIC_LIBRARY)

##############################################################################
# SipStack

LOCAL_PATH := ../../SipStack

include $(CLEAR_VARS)

LOCAL_MODULE    := libsipstack
LOCAL_SRC_FILES := SipDeleteQueue.cpp \
	SipICTList.cpp SipISTList.cpp SipNICTList.cpp SipNISTList.cpp \
	SipInviteTransaction.cpp SipNonInviteTransaction.cpp SipTransactionList.cpp \
	SipStack.cpp SipStackSetup.cpp \
	SipStackThread.cpp SipUdpThread.cpp SipTcpThread.cpp SipTlsThread.cpp \
	TcpSessionList.cpp TcpSocketMap.cpp TcpThreadList.cpp \
	SipBuffer.cpp \
	TlsFunction.cpp
LOCAL_C_INCLUDES := ../../SipPlatform ../../SipParser include

include $(BUILD_STATIC_LIBRARY)

##############################################################################
# SdpParser

LOCAL_PATH := ../../SdpParser

include $(CLEAR_VARS)

LOCAL_MODULE    := libsdpparser
LOCAL_SRC_FILES := SdpAttribute.cpp SdpBandWidth.cpp SdpConnection.cpp SdpMedia.cpp \
	SdpMessage.cpp SdpOrigin.cpp SdpTime.cpp
LOCAL_C_INCLUDES := ../../SipPlatform ../../SipParser

include $(BUILD_STATIC_LIBRARY)

##############################################################################
# SipUserAgent

LOCAL_PATH := ../../SipUserAgent

include $(CLEAR_VARS)

LOCAL_MODULE    := libsipuseragent
LOCAL_SRC_FILES := SipDialog.cpp \
	SipRegisterThread.cpp SipServerInfo.cpp \
	SipUserAgent.cpp \
	SipDialog.cpp SipCdr.cpp
LOCAL_C_INCLUDES := ../../SipPlatform ../../SipParser ../../SipStack ../../SdpParser include

include $(BUILD_STATIC_LIBRARY)

##############################################################################
# CppSipStack

LOCAL_PATH := .

include $(CLEAR_VARS)

LOCAL_MODULE    := CppSipStack
LOCAL_STATIC_LIBRARIES := libsipuseragent libsdpparser libsipstack libsipparser libsipplatform
LOCAL_SRC_FILES := AndroidSipStack.cpp AndroidSipUserAgent.cpp
LOCAL_LDLIBS    := lib/libssl.a lib/libcrypto.a -llog -ldl
LOCAL_C_INCLUDES := ../../SipPlatform ../../SipParser ../../SipStack ../../SdpParser ../../SipUserAgent include

include $(BUILD_SHARED_LIBRARY)

