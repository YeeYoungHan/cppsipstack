# Makefile for SipLoadBalancer
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2013/08/16

include ../Makefile.mk

INC=-I../SipStack -I../SipParser -I../SipPlatform \
	-I../XmlParser -I../ServerPlatform $(OPENSSL_INC)
LIB=-L../SipStack -lsipstack \
	-L../SipParser -lsipparser \
	-L../XmlParser -lxmlparser \
	-L../SipPlatform -lsipplatform \
	-L../ServerPlatform -lserverplatform \
	$(OPENSSL_LIB) -lssl -lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: SipLoadBalancer.exe

SRV_OBJ=SipLoadBalancer.o SipServer.o \
	SipServerSetup.o \
	UserMap.o SipServerMap.o \
  Monitor.o

SipLoadBalancer.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

