# Makefile for TestSipParser
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/07/12

CC=g++
CFLAGS=-Wall -O2 -D_REENTRANT -g #-pg
INC=-I../SipParser -I../SdpParser
LIB=-L../SipParser -lsipparser -L../SdpParser -lsdpparser

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestSipParser.exe

SRV_OBJ=TestSipFrom.o TestSipMessage.o TestSipUri.o TestSipVia.o \
	TestSdp.o \
	TestSipParser.o

TestSipParser.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

