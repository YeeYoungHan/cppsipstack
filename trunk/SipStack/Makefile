# Makefile for SipStack
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/07/12

CC=g++
CFLAGS=-Wall -O2 -D_REENTRANT -g #-pg
INC=-I../SipParser

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: libsipstack.a

LIB_OBJ=SipDeleteQueue.o \
	SipICTList.o SipISTList.o SipNICTList.o SipNISTList.o \
	SipInviteTransaction.o SipNonInviteTransaction.o SipTransactionList.o \
	SipStack.o SipStackSetup.o \
	SipStackThread.o SipUdpThread.o SipTcpThread.o \
	TcpSessionList.o TcpSocketMap.o TcpThreadList.o SipBuffer.o \
	SipUdp.o SipTcp.o SipMd5.o

libsipstack.a: $(LIB_OBJ)
	ar crv $@ $(LIB_OBJ)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

