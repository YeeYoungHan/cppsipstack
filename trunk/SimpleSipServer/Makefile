# Makefile for SipClient
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/08/09

CC=g++
CFLAGS=-Wall -O2 -D_REENTRANT -g #-pg
INC=-I../SipStack -I../SipParser -I../SdpParser
LIB=-L../SipStack -lsipstack \
	-L../SipParser -lsipparser \
	-lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: SimpleSipServer.exe

SRV_OBJ=SimpleSipServer.o SipServer.o UserMap.o

SimpleSipServer.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

