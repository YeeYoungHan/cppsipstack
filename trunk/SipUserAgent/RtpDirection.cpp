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

#include "SipStackDefine.h"
#include "RtpDirection.h"

/**
 * @ingroup SipUserAgent
 * @brief enum direction 을 문자열로 리턴한다.
 * @param eDirection enum direction
 * @returns enum direction 을 문자열로 리턴한다.
 */
const char * GetRtpDirectionString( ERtpDirection eDirection )
{
	switch( eDirection )
	{
	case E_RTP_SEND_RECV:
	case E_RTP_NULL:
		return "sendrecv";
	case E_RTP_SEND:
		return "sendonly";
	case E_RTP_RECV:
		return "recvonly";
	case E_RTP_INACTIVE:
		return "inactive";
	}

	return "sendrecv";
}

/**
 * @ingroup SipUserAgent
 * @brief 문자열 direction 을 enum direction 으로 리턴한다.
 * @param pszDirection 문자열 direction
 * @returns 문자열 direction 을 enum direction 으로 리턴한다.
 */
ERtpDirection GetRtpDirectionEnum( const char * pszDirection )
{
	if( !strcmp( pszDirection, "sendrecv" ) )
	{
		return E_RTP_SEND_RECV;
	}
	else if( !strcmp( pszDirection, "sendonly" ) )
	{
		return E_RTP_SEND;
	}
	else if( !strcmp( pszDirection, "recvonly" ) )
	{
		return E_RTP_RECV;
	}
	else if( !strcmp( pszDirection, "inactive" ) )
	{
		return E_RTP_INACTIVE;
	}

	return E_RTP_NULL;
}
