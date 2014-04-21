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

#include "XmlElement.h"
#include "StringMap.h"
#include <string>
#include <list>

/**
 * @ingroup KSipServer
 * @brief KSipServer 설정 파일의 내용을 저장하는 클래스
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

	/** Call Pickup 을 위한 아이디 ( 전화번호 ) */
	std::string	m_strCallPickupId;

	/** SIP stack 실행 주기 (ms 단위) */
	int					m_iStackExecutePeriod;

	/** timer D 만료시간 (ms 단위) */
	int					m_iTimerD;

	/** timer J 만료시간 (ms 단위) */
	int					m_iTimerJ;

	/** SIP 통신을 위한 realm */
	std::string	m_strRealm;

	/** SIP REGISTER timeout 최소 시간 */
	int					m_iMinRegisterTimeout;

	/** RTP relay 기능 사용 여부 */
	bool				m_bUseRtpRelay;

	/** RTP relay 에 사용할 시작 포트 번호 */
	int					m_iBeginRtpPort;

	/** RTP relay 에 사용할 종료 포트 번호 */
	int					m_iEndRtpPort;

	/** 로그인된 사용자에게 OPTIONS 메시지를 전송하는 주기 (초단위) */
	int					m_iSendOptionsPeriod;

	/** 사용자 계정 정보 저장 폴더 - 본 항목이 설정되어 있지 않으면 MySQL DB 를 사용한다. */
	std::string	m_strUserXmlFolder;

	/** IP-PBX 정보 저장 폴더 - m_strUserXmlFolder 가 설정되어 있지 않으면 본 항목은 사용되지 않는다. */
	std::string	m_strSipServerXmlFolder;

	/** CDR 파일 저장 폴더 - m_strUserXmlFolder 가 설정되어 있지 않으면 본 항목은 사용되지 않는다. */
	std::string	m_strCdrFolder;

	// ================================================================
	// 로그 기능

	/** 로그 폴더 */
	std::string	m_strLogFolder;

	/** 로그 레벨 */
	int					m_iLogLevel;

	/** 로그 파일의 최대 크기 */
	int					m_iLogMaxSize;

	// ================================================================
	// 모니터링 기능

	/** 모니터링 TCP 포트 번호 */
	int					m_iMonitorPort;

	/** 모니터링 TCP 포트에 접속 허용할 IP 주소 맵 */
	CStringMap	m_clsMonitorIpMap;

	// ================================================================
	// 보안 기능

	/** SIP transaction list 에 저장하지 않을 SIP User Agent 맵 */
	CStringMap	m_clsDenySipUserAgentMap;

	/** SIP transaction list 에 저장할 SIP User Agent 맵 */
	CStringMap	m_clsAllowSipUserAgentMap;

	/** 로그인을 허용하는 클라이언트 IP 주소 맵 */
	CStringMap	m_clsAllowClientIpMap;

	bool Read( const char * pszFileName );
	bool Read( );

	bool IsCallPickupId( const char * pszId );

	bool IsMonitorIp( const char * pszIp );

	bool IsAllowUserAgent( const char * pszSipUserAgent );
	bool IsDenyUserAgent( const char * pszSipUserAgent );

	bool IsAllowClientIp( const char * pszClientIp );
	
	bool IsChange();

private:
	bool Read( CXmlElement & clsXml );
	void SetFileSizeTime( );

	std::string	m_strFileName;	// 설정 파일 이름
	time_t			m_iFileTime;		// 설정 파일 저장 시간
	int					m_iFileSize;		// 설정 파일 크기
};

extern CSipServerSetup gclsSetup;

#endif
