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

#ifndef _LOG_FILE_H_
#define _LOG_FILE_H_

#include "SipUdp.h"
#include <string>

/**
 * @ingroup KSipServerLogAnalysis
 * @brief SIP 로그의 발신/수신 정보를 저장하는 클래스
 */
class CLogHeader
{
public:
	struct timeval	m_sttTime;
	char						m_szIp[INET6_ADDRSTRLEN];
	int							m_iPort;
	bool						m_bSend;

	void Clear();
};

/**
 * @ingroup KSipServerLogAnalysis
 * @brief SIP 로그 파일 읽기 클래스
 */
class CLogFile
{
public:
	CLogFile(void);
	~CLogFile(void);

	bool Open( const char * pszFileName );
	bool ReadSip( CLogHeader * psttLogHeader, char * pszBuf, int iBufSize );
	void Close( );

private:
	FILE	* m_sttFd;

	bool SaveLogHeader( const char * pszTemp, CLogHeader * psttLogHeader, char * pszBuf, int iBufSize, int & iPos );
	void CheckLineEnd( char * pszTemp );
};

#endif
