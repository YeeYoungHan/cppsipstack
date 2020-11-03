# Makefile for KSipServer
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2020/11/03

include ../Makefile.mk

INC=-I../SipUserAgent3 -I../SipStack3 -I../../SipParser \
	-I../../SdpParser -I../../SipPlatform \
	-I../../XmlParser -I../../ServerPlatform $(OPENSSL_INC)
LIB=-L../SipUserAgent3 -lsipuseragent \
	-L../SipStack3 -lsipstack \
	-L../../SipParser -lsipparser \
	-L../../SdpParser -lsdpparser \
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

