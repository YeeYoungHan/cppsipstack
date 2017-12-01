# Makefile for TestWebRtc
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2017/12/01

include ../Makefile.mk

INC=-I../SipUserAgent -I../SipStack -I../SipParser -I../SdpParser \
	-I../HttpStack -I../TcpStack -I../HttpParser -I../XmlParser -I../SipPlatform
LIB=-L../SipUserAgent -lsipuseragent \
	-L../SipStack -lsipstack \
	-L../SipParser -lsipparser \
	-L../SdpParser -lsdpparser \
	-L../HttpStack -lhttpstack \
	-L../TcpStack -ltcpstack \
	-L../HttpParser -lhttpparser \
	-L../XmlParser -lxmlparser \
	-L../SipPlatform -lsipplatform \
	-lssl -lcrypto -lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestWebRtc.exe

SRV_OBJ=Main.o \
	HttpCallBack.o \
	SipCallBack.o \
	UserMap.o CallMap.o

TestWebRtc.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

