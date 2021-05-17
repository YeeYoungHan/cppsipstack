#!/bin/sh

/android-ndk-r8b/ndk-build NDK_DEBUG=1
cp ../libs/armeabi/libCppSipStack.so ../TestAndroidSipStack/libs/armeabi
cp ../libs/armeabi/libCppSipStack.so ../TestAndroidSipStack2/app/src/main/jnilibs/armeabi-v7a

