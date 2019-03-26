# Makefile for TestNetwork
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2019/03/27

include ../Makefile.mk

INC=-I../SipPlatform
LIB=-L../SipPlatform -lsipplatform -lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestNetwork.exe

SRV_OBJ=TestNetwork.o TestTcpSendBlocking.o

TestNetwork.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

