# Makefile for TestWebRtc
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2017/12/01

include ../Makefile.mk

INC=-I../HttpStack -I../TcpStack -I../HttpParser \
	-I../SipUserAgent -I../SipStack -I../SipParser -I../SdpParser \
	-I../XmlParser -I../StunParser -I../SipPlatform \
	-I../opensrtp/linux/64bit/include \
	-I../openssl/linux/64bit/include
LIB=-L../HttpStack -lhttpstack \
	-L../TcpStack -ltcpstack \
	-L../HttpParser -lhttpparser \
	-L../SipUserAgent -lsipuseragent \
	-L../SipStack -lsipstack \
	-L../SipParser -lsipparser \
	-L../SdpParser -lsdpparser \
	-L../XmlParser -lxmlparser \
	-L../StunParser -lstunparser \
	-L../SipPlatform -lsipplatform \
	-L../opensrtp/linux/64bit/lib -lsrtp \
	-L../openssl/linux/64bit/lib -lssl -lcrypto -ldl \
	-lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestWebRtc.exe

SRV_OBJ=Main.o RtpThread.o \
	HttpCallBack.o \
	SipCallBack.o \
	UserMap.o CallMap.o

TestWebRtc.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

