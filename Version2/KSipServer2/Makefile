# Makefile for KSipServer
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/08/16

include ../Makefile.mk

INC=-I../SipUserAgent2 -I../SipStack2 -I../SipParser2 -I../SdpParser2 -I../../SipPlatform \
	-I../../XmlParser -I../../ServerPlatform $(OPENSSL_INC)
LIB=-L../SipUserAgent2 -lsipuseragent \
	-L../SipStack2 -lsipstack \
	-L../SipParser2 -lsipparser \
	-L../SdpParser2 -lsdpparser \
	-L../../XmlParser -lxmlparser \
	-L../../ServerPlatform -lserverplatform \
	-L../../SipPlatform -lsipplatform \
	$(OPENSSL_LIB) -lssl -lcrypto -lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: KSipServer.exe

SRV_OBJ=KSipServer.o SipServer.o \
	SipServerSetup.o XmlUser.o XmlSipServer.o \
	CallMap.o NonceMap.o UserMap.o SipServerMap.o RtpMap.o \
  RtpThread.o Monitor.o

KSipServer.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

