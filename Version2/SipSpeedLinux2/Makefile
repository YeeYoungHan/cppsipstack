# Makefile for SipSpeedLinux
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2014/08/18

include ../Makefile.mk

INC=-I../SipUserAgent2 -I../SipStack2 -I../SipParser2 -I../SdpParser2 \
	-I../../XmlParser -I../../SipPlatform $(OPENSSL_INC)
LIB=-L../SipUserAgent2 -lsipuseragent \
	-L../SipStack2 -lsipstack \
	-L../SipParser2 -lsipparser \
	-L../SdpParser2 -lsdpparser \
	-L../../XmlParser -lxmlparser \
	-L../../SipPlatform -lsipplatform \
	$(OPENSSL_LIB) -lssl -lcrypto -lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: SipSpeedLinux.exe

SRV_OBJ=SipClient.o SipSpeedLinux.o SipSpeedSetup.o

SipSpeedLinux.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

