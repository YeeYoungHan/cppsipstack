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

#ifndef _K_SIP_SERVER_VERSION_H_
#define _K_SIP_SERVER_VERSION_H_

#define KSIP_SERVER_VERSION	"0.19"

/* 버전 정보 

= 버전 0.21 ( 2014년 6월 10일 ) =
 * 모니터링 문자열에 CMonitorString 를 사용하도록 수정함

= 버전 0.19 ( 2014년 4월 19일 ) =
 * SIP INVITE 요청 메시지에 대한 응답을 수신하기 전에 통화 자료구조를 저장하지 못 하여서 통화 연결 처리가 정상적으로 되지 않는 버그를 패치함

= 버전 0.18 ( 2013년 11월 18일 ) =
 * Call Pickup 기능을 추가함

= 버전 0.17 ( 2013년 10월 26일 ) =
 * MySQL 을 사용하지 않는 경우 빌드 오류가 발생하는 버그를 패치함

= 버전 0.16 ( 2013년 9월 4일 ) =
 * 로그레벨, 로그파일크기도 실시간으로 수정 가능하다.
 * 클라이언트 IP 필터링 기능을 추가함
 * 허용 User Agent 기능을 추가함
 * SIP 서버 파일 경로를 설정하지 않았을 경우 SipServerMap Load 디버그 로그가 출력되지 않도록 수정함
 * 로그아웃시키지 못 하는 버그를 패치함

= 버전 0.15 ( 2013년 8월 21일 ) =
 * ServerPlatform 라이브러리를 이용하도록 수정함

= 버전 0.14 ( 2013년 5월 10일 ) =
 * SIP REGISTER timeout 시간을 설정하는 기능을 추가함

= 버전 0.13 ( 2013년 5월 10일 ) =
 * SIP transaction list 에 저장하지 않을 SIP User Agent 를 설정하는 기능을 추가함

= 버전 0.12 ( 2013년 4월 27일 ) =
 * 연동 IP-PBX 설정 XML 파일에 Use Element 를 추가하여서 사용 여부를 설정할 수 있다
 * KSipServer 설정 파일에서 TLS 연동 설정을 추가함

= 버전 0.11 ( 2012년 12월 04일 ) =
 * TCP 프로토콜로 SIP 메시지 전송/수신 기능 추가 완료

= 버전 0.10 ( 2012년 11월 21일 ) =
 * SIP REGISTER 응답 메시지의 Contact 헤더에 로그인된 SIP 클라이언트 정보를 저장하는 기능을 추가함.

= 버전 0.09 ( 2012년 10월 30일 ) =
 * TCP 프로토콜로 SIP 메시지를 수신하기 위한 설정 정보를 추가함.

= 버전 0.08 ( 2012년 10월 08일 ) =
 * 1분 간격으로 추가/수정/삭제된 IP-PBX 정보를 자료구조에 반영하는 기능을 추가함.
 * SIP MESSAGE 전달 기능을 추가함.

= 버전 0.07 ( 2012년 09월 22일 ) =
 * RTP relay 기능으로 Blind Transfer 가 성공한 후, ReINVITE 전송시 SDP 에 RTP 포트를 잘 못 저장하는 버그를 패치함.
 * RTP 자료구조에 대한 모니터링 기능을 추가함.
 * MS 윈도우 서비스로 등록/삭제할 수 있는 기능을 추가함.

= 버전 0.06 ( 2012년 09월 13일 ) =
 * CDR 저장 기능을 추가함.
 * signal 처리 기능을 추가함.
 * 프로세스 종료할 때에 모든 통화를 종료시키는 기능을 추가함.
 * RTP relay 기능 추가함.
 * hold / resume 기능에서 ReINVITE 메시지가 무한 전송되는 버그를 패치함.

= 버전 0.05 ( 2012년 09월 06일 ) =
 * 인증되지 않은 사용자의 INVITE 메시지에 대한 401 응답 메시지를 전송한 후, 180 응답 메시지를 전송하는 버그를 패치함.
 * IP-PBX 정보를 XML 파일에서 읽어서 로그인하는 기능을 추가함.
 * IP-PBX 정보를 저장한 XML 파일에 Call Routing 을 위한 전화번호 prefix 리스트를 설정할 수 있는 기능을 추가함.
 * 사용자 자료구조에 존재하지 않는 수신자 전화번호에 대한 INVITE 메시지를 수신하였을 때에 IP-PBX 의 Call Route Prefix 리스트를 검사하는 기능을 추가함.
 * 설정파일의 LocalIp 를 읽어서 SIP stack 을 정상적으로 시작하도록 수정함.

= 버전 0.04 ( 2012년 08월 25일 ) =
 * Blind Transfer 기능을 추가함.
 * 로그인된 사용자에게 주기적으로 OPTIONS 메시지를 전송하는 기능을 추가함.
 * 로그인된 사용자에게 OPTIONS 메시지를 전송하는 주기를 설정 XML 에서 읽어 오도록 수정함.

= 버전 0.03 ( 2012년 08월 22일 ) =
 * Screened / Unscreened Transfer 기능을 추가함.

= 버전 0.02 ( 2012년 08월 18일 ) =
 * MySQL 과 연동하는 기능을 추가함.
 * 만료된 사용자 삭제 기능을 추가함.
 * SIP UserAgent 헤더에 KSipServer 를 추가함.

*/

#endif
