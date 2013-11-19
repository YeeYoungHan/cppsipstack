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

#include "SipUserAgent.h"

/**
 * @ingroup SipUserAgent
 * @brief RTP IP/Port 를 수정한다.
 * @param pszIp IP 주소
 * @param iPort Port 번호
 */
void CSipCallRtp::SetIpPort( const char * pszIp, int iPort )
{
	m_strIp = pszIp;
	m_iPort = iPort;

#ifdef USE_MEDIA_LIST
	SDP_MEDIA_LIST::iterator itMedia;

	for( itMedia = m_clsMediaList.begin(); itMedia != m_clsMediaList.end(); ++itMedia )
	{
		itMedia->m_iPort = m_iPort;
		itMedia->DeleteAttribute( "rtcp" );
		break;
	}
#endif
}

/**
 * @ingroup SipUserAgent
 * @brief 미디어 개수를 리턴한다.
 * @returns 미디어 개수를 리턴한다.
 */
int CSipCallRtp::GetMediaCount( )
{
	int iCount = 1;

#ifdef USE_MEDIA_LIST
	iCount = m_clsMediaList.size();
	if( iCount == 0 ) iCount = 1;
#endif

	return iCount;
}
