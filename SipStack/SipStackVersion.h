/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#ifndef _SIP_STACK_VERSION_H_
#define _SIP_STACK_VERSION_H_

#define SIP_STACK_VERSION "0.28"

/* 버전 정보

= 버전 0.28 ( 2017년 1월 3일 ) =
 * m_iTcpCallBackThreadCount 설정 기능을 추가함 - 하나의 TCP 세션에 대해서 N 개의 callback 쓰레드로 동작할 수 있다.

= 버전 0.27 ( 2016년 9월 20일 ) =
 * TLS 수신 버퍼 크기를 16384 로 수정함
 * TCP/TLS SIP 메시지 버퍼 크기를 16384 로 수정함

= 버전 0.26 ( 2015년 8월 18일 ) =
 * stateful stateless 기능을 설정할 수 있다.
 * 삭제된 TLS 세션으로 TLS 메시지를 전송하지 못 하도록 수정함

= 버전 0.25 ( 2015년 8월 11일 ) =
 * UDP 포트를 설정하지 않고 사용할 수 있는 기능을 추가함

= 버전 0.24 ( 2015년 7월 28일 ) =
 * Stack Stop 할 때에 TCP 소켓 맵 자료구조를 초기화시킨다.
 * Stack Stop 할 때에 TCP 쓰레드 자료구조를 초기화시킨다.

= 버전 0.23 ( 2015년 6월 8일 ) =
 * TCP/TLS 세션 timeout 된 이후에 TCP/TLS 세션 자료구조에서 삭제하는 기능을 추가함
 * TCP/TLS 세션 수신 쓰레드에 소켓이 골고루 저장되는 기능을 추가함

= 버전 0.22 ( 2014년 10월 29일 ) =
 * IPv6 기능을 추가함

= 버전 0.21 ( 2014년 6월 10일 ) =
 * TCP / TLS 클라이언트로 연결하는 기능을 추가함
 * 모니터링 문자열에 CMonitorString 를 사용하도록 수정함

= 버전 0.20 ( 2014년 4월 22일 ) =
 * timer D, J 대기 시간 수정 기능을 추가함
 * 사용 완료된 SIP 메시지를 CSipDeleteQueue 에 저장하지 않는 기능을 추가함

= 버전 0.19 ( 2014년 4월 19일 ) =
 * SipStack 실행 주기를 설정할 수 있는 기능을 추가함
 * callback 인터페이스 삭제 기능을 추가함
 * IST 에 200 응답이 저장된 후, 180 응답이 저장될 수 있는 버그를 패치함
 * IST 리스트에 ACK 메시지를 저장할 때에 SIP Call-ID 와 CSCeq 가 일치하는지 검사하는 기능을 추가함

= 버전 0.18 ( 2013년 10월 31일 ) =
 * ThreadEnd 를 순수 가상 함수에서 가상 함수로 수정함
 * SipStack.Stop 메소드를 호출하면 SipStack callback 리스트를 초기화시킨다.

= 버전 0.16 ( 2013년 9월 4일 ) =
 * SIP stack 을 종료한 후, 대기 SIP 메시지 큐의 SIP 메시지를 제거하는 기능을 추가함
 * callback 쓰레드가 종료할 때의 이벤트 메소드를 추가함
 * SIP stack 에 허용하지 않을 클라이언트 IP 주소 필터링 기능을 추가함
 * 차단 UserAgent 기능을 security 인터페이스로 수정함
 * openssl 없이 빌드할 수 있는 기능을 추가함

= 버전 0.13 ( 2013년 5월 10일 ) =
 * SIP transaction list 에 저장하지 않을 SIP User Agent 리스트 설정 기능을 추가함
 * SIP 메시지를 생성할 때에 compact form 으로 생성할지 설정하는 기능을 추가함

= 버전 0.12 ( 2013년 4월 27일 ) =
 * ACK 수신 timeout callback 기능을 추가함
 * SipStack 이 stack 에서 제거될 때에 전역변수가 먼저 제거되어서 발생하는 비정상 종료 버그를 패치함
 * 전송 프로토콜에 대한 로컬 포트 번호를 가져오는 메소드를 추가함
 * Via 헤더를 추가할 때에 전송 프로토콜에 대한 로컬 포트를 정확하게 입력하도록 수정함.
 * IST, NICT, NIST 에 포함된 call-id 문자열을 가져오는 기능을 추가함
 * SIP Reason 헤더 파서/생성 클래스를 추가함
 * SIP 메시지를 전송한 클라이언트 IP / Port 를 SIP 메시지에 저장하는 기능을 추가함
 * AddSipParameter 메소드를 InsertSipParameter 메소드로 이름을 수정함
 * UpdateSipParameter 메소드를 추가함
 * TLS 기반으로 SIP 요청에 대한 응답 메시지가 전송되지 않는 버그를 패치함
 * TLS 세션이 연결될 수 있도록 수정함

= 버전 0.07 ( 2012년 12월 04일 ) =
 * TCP 프로토콜로 SIP 메시지 전송/수신 기능 추가 완료

= 버전 0.06 ( 2012년 10월 30일 ) =
 * TCP 프로토콜로 SIP 메시지 전송/수신 기능 추가중

= 버전 0.05 ( 2012년 10월 21일 ) =
 * VIA branch 에 time 정보를 넣어서 SIP 메시지가 일치할 확률을 줄임.
 * VIA branch 및 Call-Id 에 system unique string 을 넣는 기능을 추가함.

= 버전 0.04 ( 2012년 08월 21일 ) =
 * SendSipMessage 메소드에서 SIP stack 에 CSipMessage 를 입력하지 못 하면 CSipMessage 를 삭제하도록 수정함.
 * SendSipMessage 메소드 호출 실패시에 CSipMessage 를 삭제하는 코드를 제거함.

= 버전 0.03 ( 2012년 08월 18일 ) =
 * UserAgent 헤더를 응용 프로그램에서 수정할 수 있는 기능을 추가함
 * SIP 메시지를 전송하기 전에 2번 SIP 메시지를 점검하는 기능을 1번만 점검하도록 수정함.

*/

#endif
