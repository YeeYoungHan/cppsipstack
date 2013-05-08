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

#ifndef _SIP_FROM_H_
#define _SIP_FROM_H_

#include "SipParameterList.h"
#include "SipUri.h"

/**
 * @ingroup SipParser
 * @brief SIP From/To/Contact/Route/Record-Route ��� ���� ���� Ŭ����
 */
class CSipFrom : public CSipParameterList
{
public:
	CSipFrom();
	~CSipFrom();

	/** display name */
	std::string m_strDisplayName;

	/** SIP Uri */
	CSipUri	m_clsUri;

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	void InsertTag();

	void Clear();
	bool Empty();
};

/** SIP From ��� ����Ʈ */
typedef std::list< CSipFrom > SIP_FROM_LIST;

int ParseSipFrom( SIP_FROM_LIST & clsList, const char * pszText, int iTextLen );

#endif
