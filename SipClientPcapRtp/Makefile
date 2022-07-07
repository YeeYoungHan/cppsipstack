# Makefile for SipClientPcapRtp
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2022/07/07

include ../Makefile.mk

INC=-I../SipUserAgent -I../SipStack -I../SipParser -I../SdpParser \
	-I../XmlParser -I../SipPlatform $(OPENSSL_INC)
LIB=-L../SipUserAgent -lsipuseragent \
	-L../SipStack -lsipstack \
	-L../SipParser -lsipparser \
	-L../SdpParser -lsdpparser \
	-L../XmlParser -lxmlparser \
	-L../SipPlatform -lsipplatform \
	$(OPENSSL_LIB) -lssl -lcrypto -lasound -lpthread -lpcap

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: SipClientPcapRtp.exe

SRV_OBJ=SipClient.o SipClientMain.o SipClientSetup.o \
	RtpThread.o PacketHeader.o

SipClientPcapRtp.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

