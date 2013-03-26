# Makefile for KSipServer
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/08/16

include ../Makefile.mk

INC=-I../SipUserAgent -I../SipStack -I../SipParser -I../SdpParser -I../SipPlatform \
	-I../XmlParser -I/usr/include/mysql
LIB=-L../SipUserAgent -lsipuseragent \
	-L../SipStack -lsipstack \
	-L../SipParser -lsipparser \
	-L../SdpParser -lsdpparser \
	-L../XmlParser -lxmlparser \
	-L../SipPlatform -lsipplatform \
	-L/usr/lib/mysql -lmysqlclient \
	-lssl -lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: KSipServer.exe

SRV_OBJ=KSipServer.o SipServer.o \
	SipServerSetup.o XmlUser.o XmlSipServer.o \
	CallMap.o NonceMap.o UserMap.o SipServerMap.o RtpMap.o \
	DbMySQL.o DbInsertThread.o \
  ServerThread.o MonitorThread.o RtpThread.o

KSipServer.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

