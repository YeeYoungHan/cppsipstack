#!/bin/sh

/android-ndk-r12b/build/ndk-build NDK_DEBUG=1
cp -f ../obj/local/armeabi/*.a ../../AndroidSipStack/jni/lib
