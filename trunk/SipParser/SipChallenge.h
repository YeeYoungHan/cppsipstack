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

#ifndef _SIP_CHALLENGE_H_
#define _SIP_CHALLENGE_H_

#include "SipParameter.h"

/**
 * @ingroup SipParser
 * @brief WWW-Authenticate, Proxy-Authenticate 헤더를 저장하는 클래스
 */
class CSipChallenge
{
public:
	CSipChallenge();
	~CSipChallenge();

	/** type */
	std::string	m_strType;

	/** realm */
	std::string	m_strRealm;

	/** domain */
	std::string	m_strDomain;

	/** nonce */
	std::string	m_strNonce;

	/** opaque */
	std::string	m_strOpaque;

	/** stale */
	std::string	m_strStale;

	/** algorithm */
	std::string	m_strAlgorithm;

	/** qop */
	std::string	m_strQop;

	/** parameter 리스트 */
	SIP_PARAMETER_LIST m_clsParamList;

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	void Clear();

	static void DeQuoteString( std::string & strInput, std::string & strOutput );
	static bool SetString( char * pszText, int & iTextPos, int iTextSize, const char * pszName, std::string & strValue );
	static bool SetQuoteString( char * pszText, int & iTextPos, int iTextSize, const char * pszName, std::string & strValue );
};

/** WWW-Authenticate/Proxy-Authenticate 헤더 리스트 */
typedef std::list< CSipChallenge > SIP_CHALLENGE_LIST;

int ParseSipChallenge( SIP_CHALLENGE_LIST & clsList, const char * pszText, int iTextLen );

#endif
