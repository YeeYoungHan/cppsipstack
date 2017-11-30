# Makefile for HttpParser
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/11/22

include ../Makefile.mk

INC=-I../SipPlatform

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: libhttpparser.a

LIB_OBJ=HttpHeader.o HttpMessage.o HttpPacket.o \
	HttpStatusCode.o HttpUri.o

libhttpparser.a: $(LIB_OBJ)
	ar crv $@ $(LIB_OBJ)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

