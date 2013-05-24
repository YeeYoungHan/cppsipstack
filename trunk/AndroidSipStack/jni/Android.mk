LOCAL_PATH := ../../SipPlatform

include $(CLEAR_VARS)

LOCAL_MODULE    := libsipplatform
LOCAL_SRC_FILES := Directory.cpp Log.cpp ServerUtility.cpp SipMd5.cpp SipMutex.cpp SipTcp.cpp SipUdp.cpp StringMap.cpp StringUtility.cpp TimeString.cpp TimeUtility.cpp

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := CppSipStack
LOCAL_STATIC_LIBRARIES := libsipplatform

include $(BUILD_SHARED_LIBRARY)

