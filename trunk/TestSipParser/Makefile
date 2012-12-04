# Makefile for TestSipParser
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/07/12

CC=g++
CFLAGS=-Wall -O2 -D_REENTRANT -g #-pg
INC=-I../SipParser -I../SdpParser -I../XmlParser -I../SipPlatform
LIB=-L../SipParser -lsipparser -L../SdpParser -lsdpparser \
	-L../XmlParser -lxmlparser -L../SipPlatform -lsipplatform

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestSipParser.exe

SRV_OBJ=TestSipFrom.o TestSipMessage.o TestSipUri.o TestSipVia.o \
	TestSdp.o \
	TestXml.o \
	TestSipParser.o \
	TestInt.o \
	TestMd5.o

TestSipParser.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

