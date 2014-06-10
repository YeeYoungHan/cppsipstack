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

#ifndef _MONITOR_CALLBACK_H_
#define _MONITOR_CALLBACK_H_

#include "MonitorString.h"

/**
 * @brief 서버 모니터링 요청 이벤트 callback 인터페이스
 */
class IMonitorCallBack
{
public:
	virtual ~IMonitorCallBack(){};

	/**
	 * @brief 서버 모니터링 요청 명령 수신 이벤트 핸들러
	 * @param pszRequest	요청 명령 문자열
	 * @param strResponse 응답 문자열 저장 변수
	 * @returns 성공하면 true 를 리턴하고 실패하면 false 를 리턴한다.
	 */
	virtual bool RecvRequest( const char * pszRequest, CMonitorString & strResponse ) = 0;

	/**
	 * @brief 허용된 클라이언트인가?
	 * @param pszIp 클라이언트 IP 주소
	 * @returns 허용된 클라이언트이면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
	 */
	virtual bool IsMonitorIp( const char * pszIp ) = 0;

	int m_iMonitorPort;
};

#endif
