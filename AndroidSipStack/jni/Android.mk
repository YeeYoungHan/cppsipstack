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
# CppSipStack

LOCAL_PATH := .

include $(CLEAR_VARS)

LOCAL_MODULE    := CppSipStack
#LOCAL_STATIC_LIBRARIES := libsipuseragent libsdpparser libsipstack libsipparser libsipplatform
LOCAL_SRC_FILES := AndroidSipStack.cpp AndroidSipUserAgent.cpp \
									AndroidGet.cpp AndroidPut.cpp AndroidLog.cpp \
									AndroidClass.cpp \
									SipClient.cpp
LOCAL_C_INCLUDES := ../../SipPlatform ../../SipParser ../../SipStack ../../SdpParser ../../SipUserAgent include
LOCAL_LDLIBS    := lib/libsipuseragent.a \
									lib/libsdpparser.a \
									lib/libsipstack.a \
									lib/libsipparser.a \
									lib/libsipplatform.a \
									lib/libgnustl_static.a \
									lib/libssl.a lib/libcrypto.a \
									-llog -ldl

include $(BUILD_SHARED_LIBRARY)

