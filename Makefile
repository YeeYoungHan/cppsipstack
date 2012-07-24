# Makefile for all of C++ SipStack
#
# programmer : yee young han ( websearch@naver.com )
#            : http://blog.naver.com/websearch
# start date : 2012/07/12

all:
	cd SipParser && make
	cd SipStack && make
	cd SipUserAgent && make
	cd TestSipParser && make
	cd SipClient && make

clean:
	cd SipParser && make clean
	cd SipStack && make clean
	cd SipUserAgent && make clean
	cd TestSipParser && make clean
	cd SipClient && make clean

install:

