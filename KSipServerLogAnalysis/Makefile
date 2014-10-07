# Makefile for KSipServerLogAnalysis
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/10/19

include ../Makefile.mk

INC=-I../SipParser -I../SipStack -I../SipPlatform \
	-I../XmlParser
LIB=-L../SipParser -lsipparser \
	-L../XmlParser -lxmlparser \
	-L../SipPlatform -lsipplatform \
	-lpthread

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: KSipServerLogAnalysis.exe

SRV_OBJ=KSipServerLogAnalysis.o LogAnalysisSetup.o LogFile.o \
  StatsSipMethod.o StatsSipMethodIp.o StatsSipMethodUserAgent.o \
  StatsSipReSend.o

KSipServerLogAnalysis.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

