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

#ifndef _PACKET_HEADER_H_
#define _PACKET_HEADER_H_

#include "SipPlatformDefine.h"

// IP Protocol
#define IP_PROTO_TCP			0x06
#define IP_PROTO_UDP			0x11
#define IP_PROTO_ICMP			0x01
#define IP_PROTO_ICMP_V6	0x3A

// TCP flags 
#define TCP_FIN 0x01 
#define TCP_SYN 0x02 
#define TCP_RST 0x04 
#define TCP_PSH 0x08 
#define TCP_ACK 0x10 
#define TCP_URG 0x20 
#define TCP_ECE 0x40 
#define TCP_CWR 0x80 

// TLS Content Type
#define TLS_CT_CHANGE_CIPHER_SPEC	20
#define TLS_CT_ALERT							21
#define TLS_CT_HANDSHAKE					22
#define TLS_CT_APPLICATION_DATA		23

// TLS Handshake message type
#define TLS_HMT_CLIENT_HELLO				1
#define TLS_HMT_SERVER_HELLO				2
#define TLS_HMT_CERTIFICATE					11
#define TLS_HMT_SERVER_KEY_EXCHANGE	12
#define TLS_HMT_CERTIFICATE_REQUEST	13
#define TLS_HMT_SERVER_HELLO_DONE		14
#define TLS_HMT_CERTIFICATE_VERIFY	15
#define TLS_HMT_CLIENT_KEY_EXCHANGE	16
#define TLS_HMT_FINISHED						30

#define MIN_PACKET_SIZE		42
#define MIN_UDP_BODY_SIZE	22

/**
 * @ingroup SipCallDump
 * @brief IPv4 header
 */
typedef struct _Ip4Header_
{
	uint8_t		ver_ihl;				// Version (4 bits) + Internet header length (4 bits)
	uint8_t		tos;						// Type of service 
	uint16_t	tlen;						// Total length = IPv4 header 를 포함한 패킷 길이
	uint16_t	identification; // Identification
	uint16_t	flags_fo;				// Flags (3 bits) + Fragment offset (13 bits)
	uint8_t		ttl;						// Time to live
	uint8_t		proto;					// Protocol
	uint16_t	crc;						// Header checksum
	uint32_t	saddr;					// Source address
	uint32_t	daddr;					// Destination address
	uint32_t	op_pad;					// Option + Padding
} Ip4Header;

/**
 * @ingroup SipCallDump
 * @brief IPv6 base header
 */
typedef struct _Ip6Header_
{
	uint8_t		ver_ihl;
	uint8_t		flow_level[3];
	uint16_t	payload_length;		// Payload length : IPv6 header 를 제외한 패킷 길이
	uint8_t		next_header;			// Protocol
	uint8_t		hop_limit;
	uint8_t		saddr[16];				// Source address
	uint8_t		daddr[16];				// Destination address
} Ip6Header;

/**
 * @ingroup SipCallDump
 * @brief IPv6 option extension header
 */
typedef struct _Ip6OptionHeader_
{
	uint8_t		next_header;			// Protocol
	uint8_t		header_len;				// option header length
} Ip6OptionHeader;

/**
 * @ingroup SipCallDump
 * @brief UDP header
 */
typedef struct _UdpHeader_
{
	uint16_t sport;		// Source port
	uint16_t dport;		// Destination port
	uint16_t len;			// Datagram length
	uint16_t crc;			// Checksum
} UdpHeader;

/**
 * @ingroup SipCallDump
 * @brief TCP Header
 */
typedef struct _TcpHeader_ 
{
	uint16_t sport;	// Source port
	uint16_t dport;	// Destination port
	uint32_t seqnum; // Sequence Number
	uint32_t acknum; // Acknowledgement number
	uint8_t	 hlen;		// Header length - 상위 4bit 만 header 길이이다.
	uint8_t	 flags;	// packet flags
	uint16_t win;		// Window size
	uint16_t crc;		// Header Checksum
	uint16_t urgptr; // Urgent pointer...still don't know what this is...
} TcpHeader;

typedef struct _TlsRecordHeader_
{
	uint8_t		cType;
	uint8_t		sttVersion[2];
	uint8_t		sttLen[2];
} TlsRecordHeader;

bool IsUdpPacket( Ip4Header * psttIp4Header );
bool IsTcpPacket( Ip4Header * psttIp4Header );
int GetIpHeaderLength( Ip4Header * psttIp4Header );
void GetIpString( uint32_t iIp, char * pszIp, int iIpSize );
int GetTcpHeaderLength( TcpHeader * psttTcpHeader );
bool IsRtpPacket( char * pszUdpBody, int iUdpBodyLen );
bool IsSipPacket( char * pszUdpBody, int iUdpBodyLen );

#endif
