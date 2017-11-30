# Makefile for SipPlatform
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/11/22

include ../Makefile.mk

INC=

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: libsipplatform.a

LIB_OBJ=Directory.o Log.o FileLog.o ServerUtility.o \
	SipMd5.o SipMutex.o \
	SipTcp.o SipUdp.o \
	StringUtility.o StringMap.o \
	TimeString.o TimeUtility.o \
	FileUtility.o \
	Random.o Base64.o

libsipplatform.a: $(LIB_OBJ)
	ar crv $@ $(LIB_OBJ)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

