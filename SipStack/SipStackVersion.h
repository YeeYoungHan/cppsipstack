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

#define SIP_STACK_VERSION "0.18"

/* 버전 정보

= 버전 0.18 ( 2013년 10월 31일 ) =
 * CallBackThreadEnd 를 순수 가상 함수에서 가상 함수로 수정함
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
