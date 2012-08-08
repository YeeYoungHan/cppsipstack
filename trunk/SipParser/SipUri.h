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

#ifndef _SIP_URI_H_
#define _SIP_URI_H_

#include "SipParameter.h"

/**
 * @ingroup SipParser
 * @brief SIP URI 정보를 저장하는 클래스
 */
class CSipUri
{
public:
	CSipUri();
	~CSipUri();

	/** 프로토콜 이름 */
	std::string	m_strProtocol;

	/** 사용자 아이디 또는 전화번호 */
	std::string	m_strUser;

	/** 호스트 이름 또는 IP 주소 */
	std::string	m_strHost;

	/** 호스트 포트 번호 */
	int					m_iPort;

	/** SIP URI parameter 리스트 */
	SIP_PARAMETER_LIST m_clsUriParamList;

	/** Header 리스트. ? 이후에 저장된 parameter 리스트 */
	SIP_PARAMETER_LIST m_clsHeaderList;

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	void Clear();
	bool Empty();

	void Set( const char * pszProtocol, const char * pszUser, const char * pszHost, int iPort = 0 );

private:
	int ParseProtocol( const char * pszText, int iTextLen );
	int ParseUser( const char * pszText, int iTextLen );
	int ParseHost( const char * pszText, int iTextLen );
};

#endif
