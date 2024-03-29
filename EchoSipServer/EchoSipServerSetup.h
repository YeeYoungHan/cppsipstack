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

#ifndef _ECHO_SIP_SERVER_SETUP_H_
#define _ECHO_SIP_SERVER_SETUP_H_

#include "XmlElement.h"
#include "StringMap.h"
#include <string>
#include <list>

/**
 * @ingroup EchoSipServer
 * @brief EchoSipServer 설정 파일의 내용을 저장하는 클래스
 */
class CEchoSipServerSetup
{
public:
	CEchoSipServerSetup();
	~CEchoSipServerSetup();

	/** SIP 통신을 위한 UDP 포트 번호 */
	int					m_iUdpPort;

	/** SIP 통신을 위한 로컬 IP 주소 */
	std::string	m_strLocalIp;

	/** SIP 통신을 위한 UDP 수신 쓰레드 개수 */
	int					m_iUdpThreadCount;

	/** SIP 통신을 위한 TCP 포트 번호 */
	int					m_iTcpPort;

	/** SIP 통신을 위한 TCP 수신 쓰레드 개수 */
	int					m_iTcpThreadCount;

	/** SIP 메시지 수신 callback 처리를 위한 TCP 쓰레드 개수. 
			본 개수가 0 이면 TCP 수신 쓰레드에서 callback 을 호출하고 0 보다 크면 tcp callback 쓰레드에서 callback 을 호출한다. */
	int					m_iTcpCallBackThreadCount;

	/** SIP 통신을 위한 TCP 수신 최대 대기 시간 ( 초단위 ) */
	int					m_iTcpRecvTimeout;

	/** SIP 통신을 위한 TLS 포트 번호 */
	int					m_iTlsPort;

	/** SIP 통신을 위한 TLS handshake timeout 시간 (초단위) */
	int					m_iTlsAcceptTimeout;

	/** TLS 프로토콜을 위한 서버 인증서 + 키를 포함한 PEM 파일 full path */ 
	std::string	m_strCertFile;

	/** TLS 세션으로 연결한 클라이언트 인증을 위한 인증 기관 인증서 PEM 파일 */
	std::string m_strCaCertFile;

	// ================================================================
	// 로그 기능

	/** 로그 폴더 */
	std::string	m_strLogFolder;

	/** 로그 레벨 */
	int					m_iLogLevel;

	/** 로그 파일의 최대 크기 */
	int					m_iLogMaxSize;

	bool Read( const char * pszFileName );
	bool Read( );
	
	bool IsChange();

private:
	bool Read( CXmlElement & clsXml );
	void SetFileSizeTime( );

	std::string	m_strFileName;	// 설정 파일 이름
	time_t			m_iFileTime;		// 설정 파일 저장 시간
	int					m_iFileSize;		// 설정 파일 크기
};

extern CEchoSipServerSetup gclsSetup;

#endif
