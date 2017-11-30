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

#ifndef _HTTP_URI_H_
#define _HTTP_URI_H_

#include <string>

/**
 * @ingroup HttpParser
 * @brief HTTP URL 파서 클래스
 */
class CHttpUri
{
public:
	CHttpUri();
	~CHttpUri();

	int Parse( const char * pszText, int iTextLen );
	void Clear();

	/** 프로토콜 */
	std::string m_strProtocol;

	/** 호스트 도메인 또는 IP 주소 */
	std::string m_strHost;

	/** 포트 번호 */
	int					m_iPort;

	/** 경로 */
	std::string m_strPath;
};

#endif
