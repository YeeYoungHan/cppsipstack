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

#include <string>

enum EDbType
{
	E_DT_XML = 0,
	E_DT_MYSQL
};

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

	/** SIP 통신을 위한 realm */
	std::string	m_strRealm;

	/** 로그인된 사용자에게 OPTIONS 메시지를 전송하는 주기 (초단위) */
	int					m_iSendOptionsPeriod;

	/** 사용자 계정 정보 저장 폴더 */
	std::string	m_strUserXmlFolder;

	/** DB 서버 IP 주소 */
	std::string	m_strDbHost;

	/** DB 서버 접속 사용자 아이디 */
	std::string m_strDbUserId;

	/** DB 서버 접속 비밀번호 */
	std::string m_strDbPassWord;

	/** DB 서버 데이터베이스 이름 */
	std::string m_strDbName;

	/** DB 서버 포트 번호 */
	int					m_iDbPort;

	/** 로그 폴더 */
	std::string	m_strLogFolder;

	/** 로그 레벨 */
	int					m_iLogLevel;

	/** 로그 파일의 최대 크기 */
	int					m_iLogMaxSize;

	/** SIP 통신을 위한 정보 저장 방법 */
	EDbType			m_eType;

	bool Read( const char * pszFileName );
};

extern CSipServerSetup gclsSetup;

#endif
