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

#include "SipCallDumpAnalysis.h"

std::string gstrPhone;

int main( int argc, char * argv[] )
{
	if( argc <= 1 )
	{
		printf( "[Usage] %s {pcap filename} {phone number}\n", argv[0] );
		return 0;
	}

	std::string strPcapFileName = argv[1];

	if( argc >= 3 )
	{
		gstrPhone = argv[2];
	}

	char szErrBuf[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr * psttHeader;
	const		u_char * pszData;
	int			n, iIpHeaderLen, iIpPos;

	Ip4Header		* psttIp4Header;	// IPv4 Header

	pcap_t * psttPcap = pcap_open_offline( strPcapFileName.c_str(), szErrBuf );
	if( psttPcap == NULL )
	{
		printf( "pcap_open_offline(%s) error(%d)\n", strPcapFileName.c_str(), errno );
		return 0;
	}

	while( 1 )
	{
		n = pcap_next_ex( psttPcap, &psttHeader, &pszData );
		if( n <= 0 ) break;
		if( psttHeader->caplen < MIN_PACKET_SIZE ) continue;

		if( pszData[12] == 0x81 )
		{
			iIpPos = 18;		// VLAN
		}
		else if( pszData[12] == 0x08 )
		{
			iIpPos = 14;		// IP
		}
		else
		{
			continue;
		}

		psttIp4Header = (Ip4Header *)( pszData + iIpPos );

		// IPv4 만 검사한다.
		if( ( psttIp4Header->ver_ihl & 0xF0 ) != 0x40 ) continue;

		iIpHeaderLen = GetIpHeaderLength( psttIp4Header );
		if( iIpHeaderLen == 0 ) continue;

		if( IsUdpPacket( psttIp4Header ) )
		{
			PacketDumpUdp( psttPcap, psttHeader, pszData, psttIp4Header, iIpPos, iIpHeaderLen );
		}
		else if( IsTcpPacket( psttIp4Header ) )
		{
			PacketDumpTcp( psttPcap, psttHeader, pszData, psttIp4Header, iIpPos, iIpHeaderLen );
		}
	}

	pcap_close( psttPcap );

	return 0;
}