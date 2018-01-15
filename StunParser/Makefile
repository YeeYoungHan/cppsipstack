# Makefile for StunParser
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2018/01/15

include ../Makefile.mk

INC=-I../SipPlatform

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: libstunparser.a

LIB_OBJ=StunAttribute.o \
	StunDecode.o StunEncode.o \
	StunHeader.o StunMessage.o

libstunparser.a: $(LIB_OBJ)
	ar crv $@ $(LIB_OBJ)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

