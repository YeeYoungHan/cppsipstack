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

#ifndef _HTTP_CLIENT_H_
#define _HTTP_CLIENT_H_

#include "HttpPacket.h"

/**
 * @ingroup HttpStack
 * @brief HTTP 클라이언트 클래스
 */
class CHttpClient
{
public:
	CHttpClient();
	~CHttpClient();

	bool DoGet( const char * pszUrl, std::string & strOutputContentType, std::string & strOutputBody );
	bool DoGet( const char * pszUrl, const char * pszInputContentType, const char * pszInputBody, std::string & strOutputContentType, std::string & strOutputBody );
	bool DoPost( const char * pszUrl, const char * pszInputContentType, const char * pszInputBody, std::string & strOutputContentType, std::string & strOutputBody );
	bool DoPost( const char * pszUrl, HTTP_HEADER_LIST * pclsHeaderList, const char * pszInputContentType, const char * pszInputBody, std::string & strOutputContentType, std::string & strOutputBody );
	bool DoSoap( const char * pszUrl, const char * pszSoapAction, const char * pszInputBody, std::string & strOutputBody );
	bool DoSoap( const char * pszUrl, const char * pszSoapAction, const char * pszInputContentType, const char * pszInputBody, std::string & strOutputBody );

	void SetRecvTimeout( int iRecvTimeout );
	int GetStatusCode();

private:
	int m_iRecvTimeout;
	int m_iStatusCode;

	bool Execute( CHttpUri * pclsUri, CHttpMessage * pclsRequest, CHttpPacket * pclsPacket );
};

#endif
