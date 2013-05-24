LOCAL_PATH := ../../SipPlatform

include $(CLEAR_VARS)

LOCAL_MODULE    := libsipplatform
LOCAL_SRC_FILES := Directory.cpp Log.cpp ServerUtility.cpp SipMd5.cpp SipMutex.cpp SipTcp.cpp SipUdp.cpp StringMap.cpp StringUtility.cpp TimeString.cpp TimeUtility.cpp

include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := ../../SipParser

include $(CLEAR_VARS)

LOCAL_MODULE    := libsipparser
LOCAL_SRC_FILES := SipAcceptData.cpp SipCallId.cpp SipChallenge.cpp SipContentType.cpp SipCredential.cpp SipCSeq.cpp SipFrom.cpp SipHeader.cpp SipMessage.cpp \
	SipParameter.cpp SipParameterList.cpp SipReason.cpp SipStatusCode.cpp SipUri.cpp SipUtility.cpp SipVia.cpp
LOCAL_C_INCLUDES := ../../SipPlatform

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := CppSipStack
LOCAL_STATIC_LIBRARIES := libsipplatform libsipparser

include $(BUILD_SHARED_LIBRARY)

