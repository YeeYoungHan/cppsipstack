# Makefile for SipUserAgent
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/07/24

CC=g++
CFLAGS=-Wall -O2 -D_REENTRANT -g #-pg
INC=-I../SipStack -I../SipParser -I../SdpParser

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: libsipuseragent.a

LIB_OBJ=SipDialog.o \
	SipRegisterThread.o SipServerInfo.o \
	SipUserAgent.o \
	SipDialog.o \
	Md5.o

libsipuseragent.a: $(LIB_OBJ)
	ar crv $@ $(LIB_OBJ)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

