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

#ifndef _SIP_USER_AGENT_VERSION_H_
#define _SIP_USER_AGENT_VERSION_H_

#define SIP_USER_AGENT_VERSION "0.25"

/* 버전 정보

= 버전 0.25 ( 2015년 8월 11일 ) =
 * UDP 포트가 존재하지 않아도 정상적으로 REGISTER 전송할 수 있다.

= 버전 0.21 ( 2014년 6월 10일 ) =
 * TCP / TLS 클라이언트로 로그인하는 기능을 추가함
 * PRACK 메시지를 전송/수신하는 기능을 추가함
 * 100rel 기능을 추가함
 * OPTIONS 요청에 대한 200 응답 전송 기능을 추가함
 * 모니터링 문자열에 CMonitorString 를 사용하도록 수정함

= 버전 0.19 ( 2014년 4월 19일 ) =
 * CreateCall, StartCall 로 통화 호출할 수 있는 기능을 추가함
 * Stop 호출 시간을 줄임
 * RecvRegisterResponse 에서 사용자 정보가 삭제되었을 때에 true 를 리턴하지 않는 버그를 패치함
 * SIP register thread 를 종료할 때에 비정상 종료되는 버그를 패치함
 * CSipCallRtp 에서 audio, video port 를 가져오는 메소드를 추가함
 * call id 로 dialog 의 from id 를 가져오는 메소드를 추가함
 * 미디어 당 생성 소켓 개수를 응용에서 설정할 수 있도록 수정함
 * N 개의 미디어 처리를 위한 기능을 추가함

= 버전 0.18 ( 2013년 10월 31일 ) =
 * 2개 이상의 CSipUserAgent 를 실행할 수 있는 기능을 추가함
 * EventThreadEnd 를 순수 가상 함수에서 가상 함수로 수정함

= 버전 0.17 ( 2013년 10월 26일 ) =
 * FreeSwitch 에 REGISTER 되지 않는 버그를 패치함
 * StartCall 입력 인자 분석 기능을 추가함
 * SIP REGISTER 응답 메시지에 Expires 가 존재하면 이를 저장하는 기능을 추가함
 * SK 브로드밴드 IP-PBX 와 연동하기 위한 기능을 추가함
 * 통화 개수를 리턴하는 메소드를 추가함
 * 전체 통화를 종료하는 기능을 추가함
 * 전체 통화의 Call-ID 를 가져오는 기능을 추가함

= 버전 0.16 ( 2013년 9월 4일 ) =
 * SIP INVITE 메시지의 SDP 가 비정상적인 경우, 이에 대한 응답 메시지를 전송 시도에서 메모리 오류가 발생하는 버그를 패치함
 * Start 메소드가 호출된 경우에만 정상적으로 Stop 메소드가 처리되도록 수정함
 * callback 인터페이스를 Start 메소드의 인자로 추가함
 * callback 쓰레드가 종료할 때의 이벤트 메소드를 추가함
 * 차단 UserAgent 기능을 security 인터페이스로 수정함
 * SipUserAgent 가 종료될 때에 SIP 서버 정보를 모두 삭제하는 기능을 추가함
 * 로그아웃 처리되는 과정에서 메모리 오류가 발생할 수 있는 버그를 패치함
 * CSipCallRtp 객체가 존재하지 않아서 비정상 종료되는 버그를 패치함
 * SIP REGISTER 에 대한 401 응답을 수신한 후, 인증 정보를 포함한 SIP REGISTER 메시지를 전송할 때에 to tag 를 포함한다.
 * credential 의 uri 에 IP-PBX 도메인 주소를 입력하도록 수정함.

= 버전 0.13 ( 2013년 5월 10일 ) =
 * ACK 전송 timeout 이 발생하면 해당 전화 연결을 종료하는 기능을 추가함

= 버전 0.12 ( 2013년 4월 27일 ) =
 * Delete 메소드 리턴값이 부정확한 오류를 수정함
 * Via 헤더를 추가할 때에 전송 프로토콜에 대한 로컬 포트를 정확하게 입력하도록 수정함
 * Stop 메소드 실행시 로그아웃처리하는 기능을 추가함
 * G.723 코덱을 사용할 수 있는 기능을 추가함

= 버전 0.09 ( 2012년 12월 04일 ) =
 * TCP 프로토콜로 SIP 메시지 전송/수신 기능 추가 완료

= 버전 0.08 =
 * SIP MESSAGE 전송/수신 기능을 추가함.

*/

#endif
