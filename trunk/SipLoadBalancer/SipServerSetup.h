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

#ifndef _SIP_SERVER_SETUP_H_
#define _SIP_SERVER_SETUP_H_

#include "StringMap.h"
#include "XmlElement.h"

typedef std::list< std::string > CLIENT_IP_LIST;

/**
 * @ingroup SipLoadBalancer
 * @brief SipLoadBalancer 설정 파일의 내용을 저장하는 클래스
 */
class CSipServerSetup
{
public:
	CSipServerSetup();
	~CSipServerSetup();

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

	/** SIP 통신을 위한 TCP 수신 최대 대기 시간 ( 초단위 ) */
	int					m_iTcpRecvTimeout;

	/** SIP 통신을 위한 TLS 포트 번호 */
	int					m_iTlsPort;

	/** SIP 통신을 위한 TLS handshake timeout 시간 (초단위) */
	int					m_iTlsAcceptTimeout;

	/** TLS 프로토콜을 위한 서버 인증서 + 키를 포함한 PEM 파일 full path */ 
	std::string	m_strCertFile;

	/** 로그 폴더 */
	std::string	m_strLogFolder;

	/** 로그 레벨 */
	int					m_iLogLevel;

	/** 로그 파일의 최대 크기 */
	int					m_iLogMaxSize;

	/** 모니터링 TCP 포트 번호 */
	int					m_iMonitorPort;

	/** 모니터링 TCP 포트에 접속 허용할 IP 주소 리스트 */
	CLIENT_IP_LIST m_clsMonitorIpList;

	/** SIP transaction list 에 저장하지 않을 SIP User Agent 맵 */
	CStringMap		 m_clsDenySipUserAgentMap;

	bool Read( const char * pszFileName );
	bool ReadSipServer( const char * pszFileName );
	bool IsMonitorIp( const char * pszIp );

private:
	bool ReadSipServer( CXmlElement & clsXml );
};

extern CSipServerSetup gclsSetup;

#endif
