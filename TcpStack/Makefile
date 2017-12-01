# Makefile for TcpStack
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/11/22

include ../Makefile.mk

INC=-I../SipStack -I../SipParser -I../XmlParser -I../SipPlatform

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: libtcpstack.a

LIB_OBJ=TcpClientMap.o TcpClientThread.o TcpListenThread.o \
	TcpSessionList.o TcpSessionMap.o \
	TcpStack.o TcpStackSetup.o \
	TcpPipeThread.o TcpNoPipeThread.o TcpThreadList.o TlsFunction.o \
	FileLogSetup.o

libtcpstack.a: $(LIB_OBJ)
	ar crv $@ $(LIB_OBJ)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

