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

#define KSIP_SERVER_VERSION	"0.06"

/* 버전 정보 

= 버전 0.06 ( 2012년 09월 13일 ) =
 * CDR 저장 기능을 추가함.
 * signal 처리 기능을 추가함.
 * 프로세스 종료할 때에 모든 통화를 종료시키는 기능을 추가함.
 * 기본 호 처리에 RTP relay 기능 추가함.
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
