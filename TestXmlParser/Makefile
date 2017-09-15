# Makefile for TestXmlParser
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2017/09/15

include ../Makefile.mk

INC=-I../XmlParser -I../SipPlatform
LIB=-L../XmlParser -lxmlparser -L../SipPlatform -lsipplatform

.SUFFIXES: .o .cpp
%.o : %.cpp
	$(CC) $(CFLAGS) -c $< $(INC)

all: TestXmlParser.exe

SRV_OBJ=TestXmlAttribute.o TestXmlElement.o \
	TestXmlInsert.o TestXmlParseToString.o TestXmlSearch.o TestXmlUpdate.o \
	TestXmlParser.o

TestXmlParser.exe: $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SRV_OBJ) $(INC) $(LIB)

clean:
	rm -f *.exe
	rm -f *.o
	rm -f core.*

