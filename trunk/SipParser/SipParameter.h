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

#ifndef _SIP_PARAMETER_H_
#define _SIP_PARAMETER_H_

#include <string>
#include <list>

/**
 * @ingroup SipParser
 * @brief 
 */
class CSipParameter
{
public:
	CSipParameter();
	~CSipParameter();

	std::string	m_strName;
	std::string	m_strValue;

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	void Clear();
};

typedef std::list< CSipParameter > SIP_PARAMETER_LIST;

int ParseSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszText, int iTextLen );
bool SearchSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszName, std::string & strValue );
bool AddSipParameter( SIP_PARAMETER_LIST & clsList, const char * pszName, const char * pszValue );
int MakeSipParameterString( SIP_PARAMETER_LIST & clsList, char * pszText, int iTextSize );

#endif
