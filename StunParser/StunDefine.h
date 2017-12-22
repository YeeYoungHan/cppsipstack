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

#ifndef _STUN_DEFINE_H_
#define _STUN_DEFINE_H_

// STUN 메시지 타입
#define STUN_MT_REQUEST						0x0000
#define STUN_MT_RESPONSE_SUCCESS	0x0100
#define STUN_MT_RESPONSE_ERROR		0x0110
#define STUN_MT_INDICATION				0x0010

#define STUN_MT_BINDING						0x0001

// IP family
#define STUN_FAMILY_IPV4			0x01
#define STUN_FAMILY_IPV6			0x02

// transport
#define STUN_TRANSPORT_UDP		17
#define STUN_TRANSPORT_TCP		6

// attribute
#define STUN_AT_MAPPED_ADDRESS				0x0001
#define STUN_AT_USERNAME							0x0006
#define STUN_AT_MESSAGE_INTEGRITY			0x0008

#define STUN_AT_XOR_MAPPED_ADDRESS		0x0020

#define STUN_AT_FINGER_PRINT					0x8028

#endif
