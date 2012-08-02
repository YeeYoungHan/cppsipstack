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

#ifndef _SIP_CREDENTIAL_H_
#define _SIP_CREDENTIAL_H_

#include "SipParameter.h"

/**
 * @ingroup SipParser
 * @brief 
 */
class CSipCredential
{
public:
	CSipCredential();
	~CSipCredential();

	std::string	m_strType;
	std::string	m_strUserName;
	std::string	m_strRealm;
	std::string	m_strNonce;
	std::string	m_strUri;
	std::string	m_strResponse;
	std::string	m_strAlgorithm;
	std::string	m_strCnonce;
	std::string	m_strOpaque;
	std::string	m_strQop;
	std::string	m_strNonceCount;

	SIP_PARAMETER_LIST m_clsParamList;

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	void Clear();
};

typedef std::list< CSipCredential > SIP_CREDENTIAL_LIST;

int ParseSipCredential( SIP_CREDENTIAL_LIST & clsList, const char * pszText, int iTextLen );

#endif
