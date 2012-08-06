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

#ifndef _SDP_MEDIA_H_
#define _SDP_MEDIA_H_

#include "SdpConnection.h"
#include "SdpBandWidth.h"
#include "SdpAttribute.h"

typedef std::list< std::string > SDP_FMT_LIST;

/**
 * @ingroup SdpParser
 * @brief SDP media 를 저장하는 클래스
 */
class CSdpMedia
{
public:
	CSdpMedia();
	~CSdpMedia();

	std::string	m_strMedia;
	int					m_iPort;
	int					m_iNumOfPort;
	std::string	m_strProtocol;
	SDP_FMT_LIST	m_clsFmtList;

	std::string	m_strTitle;
	CSdpConnection	m_clsConnection;

	SDP_BANDWIDTH_LIST	m_clsBandWidthList;
	SDP_ATTRIBUTE_LIST	m_clsAttributeList;

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	void Clear();

private:
	void SetData( const char * pszData, int iLen, int iType );
};

typedef std::list< CSdpMedia > SDP_MEDIA_LIST;

#endif
