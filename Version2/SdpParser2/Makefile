# Makefile for SdpParser
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/07/26

include ../Makefile.mk

INC=-I ../SipParser2 -I../../SipPlatform

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: libsdpparser.a

LIB_OBJ=SdpAttribute.o SdpBandWidth.o SdpConnection.o SdpMedia.o \
	SdpMessage.o SdpOrigin.o SdpTime.o SdpAttributeCrypto.o

libsdpparser.a: $(LIB_OBJ)
	ar crv $@ $(LIB_OBJ)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

