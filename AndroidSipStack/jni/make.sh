#!/bin/sh

/android-ndk-r12b/build/ndk-build NDK_DEBUG=1
cp ../libs/armeabi-v7a/libCppSipStack.so ../TestAndroidSipStack/libs/armeabi
cp ../libs/armeabi-v7a/libCppSipStack.so ../TestAndroidSipStack2/app/src/main/jnilibs/armeabi-v7a
cp ../libs/arm64-v8a/libCppSipStack.so ../TestAndroidSipStack2/app/src/main/jnilibs/arm64-v8a

