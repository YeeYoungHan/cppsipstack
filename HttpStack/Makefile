# Makefile for HttpStack
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/11/22

include ../Makefile.mk

INC=-I../TcpStack -I../HttpParser -I../SipStack -I../SipParser \
	-I../XmlParser -I../SipPlatform

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: libhttpstack.a

LIB_OBJ=HttpClient.o HttpStack.o WebSocketPacket.o

libhttpstack.a: $(LIB_OBJ)
	ar crv $@ $(LIB_OBJ)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

