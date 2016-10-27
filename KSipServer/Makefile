# Makefile for KSipServer
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/08/16

include ../Makefile.mk

INC=-I../SipUserAgent -I../SipStack -I../SipParser -I../SdpParser -I../SipPlatform \
	-I../XmlParser -I../ServerPlatform $(OPENSSL_INC)
LIB=-L../SipUserAgent -lsipuseragent \
	-L../SipStack -lsipstack \
	-L../SipParser -lsipparser \
	-L../SdpParser -lsdpparser \
	-L../XmlParser -lxmlparser \
	-L../ServerPlatform -lserverplatform \
	-L../SipPlatform -lsipplatform \
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

