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

#ifndef _SIP_CALL_DUMP_VERSION_H_
#define _SIP_CALL_DUMP_VERSION_H_

#define SERVER_VERSION							"0.03"

/* 버전 설명

= 버전 0.03 ( 2019년 9월 9일 ) =
 * 에러가 발생하였지만 기존에 수신된 패킷들이 있다면 pcap 을 다시 시작한다.

= 버전 0.02 ( 2019년 8월 20일 ) =
 * ReINVITE 패킷도 패킷 덤프 파일에 저장한다.
 * ReINVITE 응답은 무시하고 패킷 덤프 파일에만 저장한다.

= 버전 0.01 ( 2019년 8월 9일 ) =
 * UDP 패킷이 fragment 된 경우, fragment 된 IP 패킷을 merge 하는 기능 추가함

*/

#endif
