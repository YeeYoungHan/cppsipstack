# Makefile for TestServerPlatform
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2019/12/02

include ../Makefile.mk

INC=-I../ServerPlatform -I../SipStack -I../SipPlatform
LIB=-L../ServerPlatform -lserverplatform -L../SipPlatform -lsipplatform -lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestServerPlatform.exe

SRV_OBJ=TestServerPlatform.o TestServerSignal.o

TestServerPlatform.exe: $(SRV_OBJ)
	$(CC) -rdynamic $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

