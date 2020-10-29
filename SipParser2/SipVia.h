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

#ifndef _SIP_VIA_H_
#define _SIP_VIA_H_

#include "SipParameterList.h"

/**
 * @ingroup SipParser
 * @brief SIP Via 헤더 저장 클래스
 */
class CSipVia : public CSipParameterList
{
public:
	CSipVia();
	~CSipVia();

	/** 프로토콜 이름 ( SIP ) */
	std::string	m_strProtocolName;

	/** 프로토콜 버전 ( 2.0 ) */
	std::string	m_strProtocolVersion;

	/** transport ( UDP ) */
	std::string	m_strTransport;

	/** 호스트 이름 또는 IP 주소 */
	std::string	m_strHost;

	/** 호스트 포트 번호 */
	int					m_iPort;

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	void Clear();

private:
	int ParseSentProtocol( const char * pszText, int iTextLen );
	int ParseSentBy( const char * pszText, int iTextLen );
};

typedef std::list< CSipVia > SIP_VIA_LIST;

int ParseSipVia( SIP_VIA_LIST & clsList, const char * pszText, int iTextLen );

#endif
