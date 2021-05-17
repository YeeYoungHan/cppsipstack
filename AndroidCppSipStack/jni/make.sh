#!/bin/sh

/android-ndk-r12b/build/ndk-build NDK_DEBUG=1
cp -f ../obj/local/armeabi-v7a/*.a ../../AndroidSipStack/jni/lib/armeabi-v7a
cp -f ../obj/local/arm64-v8a/*.a ../../AndroidSipStack/jni/lib/arm64-v8a
