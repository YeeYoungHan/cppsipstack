# Makefile for SipStack
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/07/12

CC=g++
CFLAGS=-Wall -O2 -D_REENTRANT -g #-pg
INC=-I../SipParser
LIB=-L../SipParser -lsipparser

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestSipParser.exe

SRV_OBJ=TestSipFrom.o TestSipMessage.o TestSipUri.o TestSipVia.o \
	TestSipParser.o

TestSipParser.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.a
	rm -f *.o
	rm -f core.*

