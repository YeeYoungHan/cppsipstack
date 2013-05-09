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

#ifndef _SIP_ACCEPT_DATA_H_
#define _SIP_ACCEPT_DATA_H_

#include "SipParameterList.h"

/**
 * @ingroup SipParser
 * @brief SIP Accept-Encoding, Accept-Language ����� �����ϴ� Ŭ����
 */
class CSipAcceptData : public CSipParameterList
{
public:
	CSipAcceptData();
	~CSipAcceptData();

	/** �̸� */
	std::string		m_strName;

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	void Clear();
};

/** Accept-Encoding/Accept-Language ��� ����Ʈ */
typedef std::list< CSipAcceptData > SIP_ACCEPT_DATA_LIST;

int ParseSipAcceptData( SIP_ACCEPT_DATA_LIST & clsList, const char * pszText, int iTextLen );

#endif
