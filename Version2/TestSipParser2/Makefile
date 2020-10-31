# Makefile for TestSipParser
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/07/12

include ../Makefile.mk

INC=-I../SipParser2 -I../SdpParser2 -I../../XmlParser -I../../SipPlatform
LIB=-L../SipParser2 -lsipparser -L../SdpParser2 -lsdpparser \
	-L../../XmlParser -lxmlparser -L../../SipPlatform -lsipplatform

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestSipParser.exe

SRV_OBJ=TestSipFrom.o TestSipMessage.o TestSipUri.o TestSipVia.o \
	TestSdp.o \
	TestSipParser.o \
	TestInt.o \
	TestMd5.o \
	TestString.o TestSipHeader.o TestSipReason.o TestSipCallId.o \
	TestSpeed.o TestTransactionSpeed.o TestFile.o

TestSipParser.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

