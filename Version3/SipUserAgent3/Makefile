# Makefile for SipUserAgent
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2020/11/03

include ../Makefile.mk

INC=-I../SipStack3 -I../../SipParser -I../../SdpParser -I../../SipPlatform

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

