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

#ifndef _XML_SIP_SERVER_H_
#define _XML_SIP_SERVER_H_

#include <string>
#include "SipServerInfo.h"

#define FLAG_NULL				0x00
#define FLAG_INSERT			0x01
#define FLAG_UPDATE			0x02
#define FLAG_DELETE			0x04
#define FLAG_NO_CHANGE	0x10

typedef std::list< std::string > ROUTE_PREFIX_LIST;

class CXmlSipServer : public CSipServerInfo
{
public:
	CXmlSipServer();
	~CXmlSipServer();

	int						m_iFlag;
	ROUTE_PREFIX_LIST	m_clsRoutePrefixList;

	bool Parse( const char * pszFileName );
	void Clear();
};

#endif
