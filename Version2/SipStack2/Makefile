# Makefile for SipStack
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/07/12

include ../Makefile.mk

INC=-I../SipParser2 -I../../SipPlatform $(OPENSSL_INC)

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: libsipstack.a

LIB_OBJ=SipDeleteQueue.o \
	SipICTList.o SipISTList.o SipNICTList.o SipNISTList.o \
	SipInviteTransaction.o SipNonInviteTransaction.o SipTransactionList.o \
	SipStack.o SipStackSetup.o \
	SipStackThread.o SipUdpThread.o SipTcpThread.o SipTlsThread.o \
	SipQueueThread.o SipQueue.o \
	SipTcpClientThread.o SipTcpMessage.o \
	SipTlsClientThread.o SipTlsMessage.o \
	TcpSessionList.o TcpSocketMap.o TcpThreadList.o TcpConnectMap.o \
	SipBuffer.o \
	TlsFunction.o

libsipstack.a: $(LIB_OBJ)
	ar crv $@ $(LIB_OBJ)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

