# Makefile for all of C++ SipStack
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2013/03/26

CC=g++
CFLAGS=-Wall -O2 -D_REENTRANT -g #-pg

# 32bit or 64bit OS
SYSTEM_BIT=$(shell getconf LONG_BIT)

OPENSSL_INC=
OPENSSL_LIB=

