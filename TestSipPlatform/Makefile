# Makefile for TestSipPlatform
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2014/06/06

include ../Makefile.mk

INC=-I../SipPlatform
LIB=-L../SipPlatform -lsipplatform -lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestSipPlatform.exe

SRV_OBJ=TestSipPlatform.o TestSipMutex.o TestDirectory.o TestStringSort.o \
	TestServerUtility.o TestStringUtility.o TestLog.o TestRandom.o \
	TestFileUtility.o

TestSipPlatform.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

