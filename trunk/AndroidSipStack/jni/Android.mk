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
LOCAL_SRC_FILES := Directory.cpp Log.cpp ServerUtility.cpp SipMd5.cpp SipMutex.cpp SipTcp.cpp SipUdp.cpp StringMap.cpp StringUtility.cpp TimeString.cpp TimeUtility.cpp

include $(BUILD_STATIC_LIBRARY)

##############################################################################
# SipParser

LOCAL_PATH := ../../SipParser

include $(CLEAR_VARS)

LOCAL_MODULE    := libsipparser
LOCAL_SRC_FILES := SipAcceptData.cpp SipCallId.cpp SipChallenge.cpp SipContentType.cpp SipCredential.cpp SipCSeq.cpp SipFrom.cpp SipHeader.cpp SipMessage.cpp \
	SipParameter.cpp SipParameterList.cpp SipReason.cpp SipStatusCode.cpp SipUri.cpp SipUtility.cpp SipVia.cpp
LOCAL_C_INCLUDES := ../../SipPlatform

include $(BUILD_STATIC_LIBRARY)

##############################################################################
# CppSipStack

include $(CLEAR_VARS)

LOCAL_MODULE    := CppSipStack
LOCAL_STATIC_LIBRARIES := libsipplatform libsipparser

include $(BUILD_SHARED_LIBRARY)

