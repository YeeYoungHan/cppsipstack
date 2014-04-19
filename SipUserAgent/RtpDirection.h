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

#ifndef _RTP_DIRECTION_H_
#define _RTP_DIRECTION_H_

/**
 * @ingroup SipUserAgent
 * @brief RTP 전송/수신
 */
enum ERtpDirection
{
	E_RTP_SEND_RECV = 0,
	E_RTP_SEND,
	E_RTP_RECV,
	E_RTP_INACTIVE,
	E_RTP_NULL
};

const char * GetRtpDirectionString( ERtpDirection eDirection );
ERtpDirection GetRtpDirectionEnum( const char * pszDirection );

#endif
