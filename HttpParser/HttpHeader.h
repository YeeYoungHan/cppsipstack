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

#ifndef _HTTP_HEADER_H_
#define _HTTP_HEADER_H_

#include <string>
#include <list>

/**
 * @ingroup HttpParser
 * @brief SIP 헤더 정보를 저장하는 클래스
 */
class CHttpHeader
{
public:
	CHttpHeader();
	CHttpHeader( const char * pszName, const char * pszValue );
	~CHttpHeader();

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );

	void Set( const char * pszName, const char * pszValue );
	void Clear();

	/** 이름 */
	std::string	m_strName;

	/** 값 */
	std::string	m_strValue;
};

typedef std::list< CHttpHeader > HTTP_HEADER_LIST;

#endif
