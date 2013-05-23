APP_PROJECT_PATH := $(call my-dir)
APP_MODULES      := AndroidCppSipStack

APP_ABI := armeabi
APP_STL := stlport_static
APP_CPPFLAGS += -fno-rtti 
APP_CPPFLAGS += -fno-exceptions
