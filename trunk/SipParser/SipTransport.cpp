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

#include "SipParserDefine.h"
#include "SipTransport.h"
#include "MemoryDebug.h"

/**
 * @ingroup SipParser
 * @brief SIP transport �������ݿ� ���� ���ڿ��� �����Ѵ�.
 * @param eTransport SIP transport
 * @returns SIP transport �������ݿ� ���� ���ڿ��� �����Ѵ�.
 */
const char * SipGetTransport( ESipTransport eTransport )
{
	if( eTransport == E_SIP_UDP )
	{
		return "UDP";
	}
	
	if( eTransport == E_SIP_TCP )
	{
		return "TCP";
	}
	
	if( eTransport == E_SIP_TLS )
	{
		return "TLS";
	}

	return "UDP";
}

/**
 * @ingroup SipParser
 * @brief SIP �������ݿ� ���� ���ڿ��� �����Ѵ�.
 * @param eTransport SIP transport
 * @returns SIP �������ݿ� ���� ���ڿ��� �����Ѵ�.
 */
const char * SipGetProtocol( ESipTransport eTransport )
{
	if( eTransport == E_SIP_TLS )
	{
		return "sips";
	}

	return SIP_PROTOCOL;
}
