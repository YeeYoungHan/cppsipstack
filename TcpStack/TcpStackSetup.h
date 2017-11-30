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

#ifndef _TCP_STACK_SETUP_H_
#define _TCP_STACK_SETUP_H_

#include "XmlElement.h"

/**
 * @ingroup TcpStack
 * @brief TCP stack 설정
 */
class CTcpStackSetup
{
public:
	CTcpStackSetup();
	~CTcpStackSetup();

	bool Parse( CXmlElement & clsXml );

	/** TCP listen IP 주소 - any 이면 공백으로 입력한다. */
	std::string m_strListenIp;

	/** TCP listen 포트 번호 - TCP listen 하지 않는 경우에는 설정하지 않으면 된다. */
	int m_iListenPort;

	/** 선 실행 Thread 개수 - 최소 쓰레드 개수 */
	int m_iThreadInitCount;

	/** 최대 실행 Thread 개수 - 0 보다 큰 정수로 설정하면 해당 개수만큼만 쓰레드를 생성한다. 0 이면 시스템이 허락하는 만큼 쓰레드를 생성한다. */
	int m_iThreadMaxCount;

	/** 하나의 Thread 에서 사용할 수 있는 최대 소켓 개수 */
	int m_iMaxSocketPerThread;

	/** TCP 수신 timeout */
	int m_iTcpRecvTimeout;

	/** TCP 연결 후, 최초 패킷이 수신되지 않은 경우의 timeout */
	int m_iTcpNoPacketTimeout;

	/** TCP 연결 timeout */
	int m_iTcpConnectTimeout;

	/** TLS 를 사용하는지 설정한다. true 로 입력하면 TLS 를 사용하는 것이고 false 로 입력하면 평문 통신한다.
			본 항목을 true 로 설정하면 m_strCertFile 을 설정해야 한다. */
	bool m_bUseTls;

	/** TLS 서버를 위한 개인키/인증서 PEM 저장 파일 이름 ( full path ) */
	std::string m_strCertFile;

	/** 쓰레드와 통신에 pipe 를 사용할 것인지 설정한다. 쓰레드와 통신을 pipe 로 한다는 것은 thread pool 을 사용한다는 것을 의미한다. 
			thread pool 을 이용할 경우 true 로 입력하고 그렇지 않으면 false 로 입력한다. */
	bool m_bUseThreadPipe;
};

#endif
