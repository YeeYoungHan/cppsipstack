# Makefile for McuControlServer
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2013/01/24

CC=g++
CFLAGS=-Wall -O2 -D_REENTRANT -g #-pg
INC=-I../SipStack -I../SipParser -I../SdpParser -I../SipPlatform
LIB=-L../SipStack -lsipstack \
	-L../SipParser -lsipparser \
	-L../SipPlatform -lsipplatform \
	-lssl -lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: McuControlServer.exe

SRV_OBJ=McuControlServer.o SipServer.o UserMap.o

McuControlServer.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

