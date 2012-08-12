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

#ifndef _SDP_MESSAGE_H_
#define _SDP_MESSAGE_H_

#include "SdpOrigin.h"
#include "SdpMedia.h"
#include "SdpTime.h"

typedef std::list< std::string > SDP_EMAIL_LIST;
typedef std::list< std::string > SDP_PHONE_LIST;

class CSdpMessage
{
public:
	CSdpMessage();
	~CSdpMessage();

	std::string	m_strVersion;
	std::string	m_strSessionName;
	std::string	m_strSessionInformation;
	std::string	m_strUri;
	std::string	m_strTimeZone;

	CSdpOrigin			m_clsOrigin;
	CSdpConnection	m_clsConnection;

	SDP_EMAIL_LIST	m_clsEmailList;
	SDP_PHONE_LIST	m_clsPhoneList;

	SDP_BANDWIDTH_LIST	m_clsBandWidthList;
	SDP_TIME_LIST				m_clsTimeList;
	SDP_ATTRIBUTE_LIST	m_clsAttributeList;
	SDP_MEDIA_LIST			m_clsMediaList;

	int Parse( const char * pszText, int iTextLen );
	int ToString( char * pszText, int iTextSize );
	void Clear();
};

#endif
