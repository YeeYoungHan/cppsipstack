# Makefile for SipPlatform
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/11/22

CC=g++
CFLAGS=-Wall -O2 -D_REENTRANT -g #-pg
INC=

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: libsipplatform.a

LIB_OBJ=Directory.o Log.o ServerUtility.o \
  SipMd5.o SipMutex.o \
  SipTcp.o SipUdp.o \
  StringUtility.o TimeString.o

libsipplatform.a: $(LIB_OBJ)
	ar crv $@ $(LIB_OBJ)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

