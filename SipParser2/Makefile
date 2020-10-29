# Makefile for SipParser
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/07/12

include ../Makefile.mk

INC=-I../SipPlatform

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: libsipparser.a

LIB_OBJ=SipAcceptData.o SipCallId.o SipChallenge.o SipContentType.o \
	SipCredential.o SipCSeq.o SipFrom.o SipHeader.o SipMessage.o \
	SipParameter.o SipUri.o SipVia.o SipStatusCode.o SipReason.o \
	SipUtility.o SipParameterList.o \
	SipTransport.o

libsipparser.a: $(LIB_OBJ)
	ar crv $@ $(LIB_OBJ)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

