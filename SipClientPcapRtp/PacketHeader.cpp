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

#include "SipPlatformDefine.h"
#include "PacketHeader.h"
#include <stdio.h>
#include "MemoryDebug.h"

/**
 * @ingroup SipCallDump
 * @brief UDP ��Ŷ���� �˻��Ѵ�.
 * @param psttIp4Header IPv4 ���
 * @returns UDP ��Ŷ�̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool IsUdpPacket( Ip4Header * psttIp4Header )
{
	if( ( psttIp4Header->ver_ihl & 0xF0 ) == 0x40 )
	{
		if( psttIp4Header->proto == IP_PROTO_UDP )
		{
			return true;
		}
	}
	else if( ( psttIp4Header->ver_ihl & 0xF0 ) == 0x60 )
	{
		Ip6Header	* psttIp6Header = ( Ip6Header * )psttIp4Header;

		if( psttIp6Header->next_header == IP_PROTO_UDP )
		{
			return true;
		}
	}

	return false;
}

/**
 * @ingroup SipCallDump
 * @brief TCP ��Ŷ���� �˻��Ѵ�.
 * @param psttIp4Header IPv4 ���
 * @returns TCP ��Ŷ�̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool IsTcpPacket( Ip4Header * psttIp4Header )
{
	if( ( psttIp4Header->ver_ihl & 0xF0 ) == 0x40 )
	{
		if( psttIp4Header->proto == IP_PROTO_TCP )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if( ( psttIp4Header->ver_ihl & 0xF0 ) == 0x60 )
	{
		Ip6Header	* psttIp6Header = ( Ip6Header * )psttIp4Header;

		if( psttIp6Header->next_header == IP_PROTO_TCP )
		{
			return true;
		}
	}

	return false;
}

/** IP ��� ���̸� �����Ѵ�.
 * @ingroup SipCallDump
 * @brief IP ��� ���̸� �����Ѵ�.
 * @param psttIp4Header IPv4 ���
 * @returns IP ��� ���̸� �����Ѵ�.
 */
int GetIpHeaderLength( Ip4Header * psttIp4Header )
{
	if( ( psttIp4Header->ver_ihl & 0xF0 ) == 0x40 )
	{
		return ( psttIp4Header->ver_ihl & 0x0F ) * 4;
	}
	else if( ( psttIp4Header->ver_ihl & 0xF0 ) == 0x60 )
	{
		return 40;
	}

	return 0;
}

/**
 * @ingroup SipCallDump
 * @brief network order IP �ּҸ� ���ڿ� IP �ּҿ� �����Ѵ�.
 * @param iIp			IP �ּ�
 * @param pszIp		[out] IP �ּ� ���� ����
 * @param iIpSize pszIp ������ ũ��
 */
void GetIpString( uint32_t iIp, char * pszIp, int iIpSize )
{
	snprintf( pszIp, iIpSize, "%d.%d.%d.%d", iIp & 0xFF, ( iIp >> 8 ) & 0xFF, ( iIp >> 16 ) & 0xFF, ( iIp >> 24 ) & 0xFF );
}

/**
 * @ingroup SipCallDump
 * @brief TCP ��� ���̸� �����Ѵ�.
 * @param psttTcpHeader TCP ���
 * @returns TCP ��� ���̸� �����Ѵ�.
 */
int GetTcpHeaderLength( TcpHeader * psttTcpHeader )
{
	return ( ( psttTcpHeader->hlen >> 4 ) & 0x0F ) * 4;
}

/**
 * @ingroup SipCallDump
 * @brief RTP ��Ŷ���� �˻��Ѵ�.
 * @param pszUdpBody	UDP body
 * @param iUdpBodyLen UDP body ����
 * @returns RTP ��Ŷ�̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool IsRtpPacket( char * pszUdpBody, int iUdpBodyLen )
{
	if( (uint8_t)pszUdpBody[0] == 0x80 || (uint8_t)pszUdpBody[0] == 0x81 )
	{
		return true;
	}

	return false;
}

/**
 * @ingroup SipCallDump
 * @brief SIP �޽������� �˻��Ѵ�.
 * @param pszUdpBody	UDP body
 * @param iUdpBodyLen UDP body ����
 * @returns SIP ��Ŷ�̸� true �� �����ϰ� �׷��� ������ false �� �����Ѵ�.
 */
bool IsSipPacket( char * pszUdpBody, int iUdpBodyLen )
{
	if( !strncmp( pszUdpBody, "INVITE ", 7 ) || 
			!strncmp( pszUdpBody, "CANCEL ", 7 ) ||
			!strncmp( pszUdpBody, "BYE ", 4 ) ||
			!strncmp( pszUdpBody, "ACK ", 4 ) ||
			!strncmp( pszUdpBody, "REFER ", 6 ) ||
			!strncmp( pszUdpBody, "NOTIFY ", 7 ) ||
			!strncmp( pszUdpBody, "PRACK ", 6 ) ||
			!strncmp( pszUdpBody, "INFO ", 5 ) ||
			!strncmp( pszUdpBody, "MESSAGE ", 8 ) ||
			!strncmp( pszUdpBody, "OPTIONS ", 8 ) ||
			!strncmp( pszUdpBody, "SUBSCRIBE ", 10 ) ||
			!strncmp( pszUdpBody, "SIP/2.0", 7 ) )
	{
		return true;
	}

	return false;
}
