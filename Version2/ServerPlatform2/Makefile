# Makefile for ServerPlatform
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2013/08/21

include ../Makefile.mk

INC=-I../SipStack2 -I../SipParser2 -I../../SipPlatform

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: libserverplatform.a

LIB_OBJ=ServerMain.o ServerSignal.o ServerService.o \
	ServerThread.o MonitorThread.o

libserverplatform.a: $(LIB_OBJ)
	ar crv $@ $(LIB_OBJ)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

