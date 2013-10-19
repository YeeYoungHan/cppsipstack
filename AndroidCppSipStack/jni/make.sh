#!/bin/sh

/android-ndk-r8b/ndk-build NDK_DEBUG=1
cp -f ../obj/local/armeabi/*.a ../../AndroidSipStack/jni/lib
