# Makefile for SipUserAgent
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/07/24

include ../Makefile.mk

INC=-I../SipStack2 -I../SipParser2 -I../SdpParser2 -I../../SipPlatform

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: libsipuseragent.a

LIB_OBJ=SipDialog.o \
	SipRegisterThread.o SipServerInfo.o \
	SipUserAgent.o SipUserAgentCallBack.o \
	SipDialog.o SipCdr.o \
	RtpDirection.o

libsipuseragent.a: $(LIB_OBJ)
	ar crv $@ $(LIB_OBJ)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

