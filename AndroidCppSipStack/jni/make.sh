#!/bin/sh

/android-ndk-r8b/ndk-build
cp -f ../obj/local/armeabi/*.a ../../AndroidSipStack/jni/lib
