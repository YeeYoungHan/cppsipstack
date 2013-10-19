#!/bin/sh

/android-ndk-r8b/ndk-build NDK_DEBUG=1
cp ../libs/armeabi/libCppSipStack.so ../TestAndroidSipStack/libs/armeabi

