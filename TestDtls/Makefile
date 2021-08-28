# Makefile for TestWebRtc
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2017/12/01

include ../Makefile.mk

INC=-I../SipStack -I../SipParser -I../SipPlatform \
	-I../openssl/include \
	-I../openssl/linux/$(SYSTEM_BIT)bit/include
LIB=-L../SipStack -lsipstack \
	-L../SipParser -lsipparser \
	-L../SipPlatform -lsipplatform \
	-L../openssl/linux/$(SYSTEM_BIT)bit/lib -lssl -lcrypto -ldl \
	-lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestDtls.exe

SRV_OBJ=TestDtls.cpp

TestDtls.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

