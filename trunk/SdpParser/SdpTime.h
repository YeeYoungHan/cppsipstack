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

#ifndef _SDP_TIME_H_
#define _SDP_TIME_H_

#include <string>
#include <list>

typedef std::list< std::string > SDP_REPEAT_TIME_LIST;

/**
 * @ingroup SdpParser
 * @brief SDP time �� �����ϴ� Ŭ����
 */
class CSdpTime
{
public:
	CSdpTime();
	~CSdpTime();

	std::string	m_strStartTime;
	std::string	m_strStopTime;

	SDP_REPEAT_TIME_LIST m_clsRepeatTimeList;

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	void Clear();
	bool Empty();

private:
	void SetData( const char * pszData, int iLen, int iType );
};

typedef std::list< CSdpTime > SDP_TIME_LIST;

#endif
