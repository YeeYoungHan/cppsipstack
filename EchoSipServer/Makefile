# Makefile for EchoSipServer
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2019/04/05

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

all: EchoSipServer.exe

SRV_OBJ=CallMap.o EchoSipServer.o EchoSipServerSetup.o SipServer.o

EchoSipServer.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

