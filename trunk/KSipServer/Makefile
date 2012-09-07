# Makefile for KSipServer
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/08/16

CC=g++
CFLAGS=-Wall -O2 -D_REENTRANT -g #-pg
INC=-I../SipUserAgent -I../SipStack -I../SipParser -I../SdpParser \
	-I../XmlParser -I/usr/include/mysql
LIB=-L../SipUserAgent -lsipuseragent \
	-L../SipStack -lsipstack \
	-L../SipParser -lsipparser \
	-L../SdpParser -lsdpparser \
	-L../XmlParser -lxmlparser \
	-L/usr/lib/mysql -lmysqlclient \
	-lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: KSipServer.exe

SRV_OBJ=KSipServer.o SipServer.o \
	SipServerSetup.o XmlUser.o XmlSipServer.o \
	Directory.o Log.o ServerUtility.o TimeString.o \
	CallMap.o NonceMap.o UserMap.o SipServerMap.o \
	DbMySQL.o DbInsertThread.o \
  MonitorThread.o

KSipServer.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

