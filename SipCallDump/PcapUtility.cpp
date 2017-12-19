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
#include "PcapUtility.h"
#include "MemoryDebug.h"

/**
 * @ingroup SipCallDump
 * @brief 디바이스 설명에 대한 디바이스 이름을 검색한다.
 * @param pszDeviceDesscription 디바이스 설명
 * @param pszDeviceName					디바이스 이름을 저장할 변수
 * @param iDeviceNameSize				pszDeviceName 변수 크기
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool GetDeviceName( const char * pszDeviceDesscription, char * pszDeviceName, int iDeviceNameSize )
{
	pcap_if_t * psttAllDevice = NULL;
	pcap_if_t * psttDeviceEntry;
	char		szErrBuf[PCAP_ERRBUF_SIZE];
	bool		bFound = false;

	// Retrieve the device list
	if( pcap_findalldevs( &psttAllDevice, szErrBuf ) == -1 )
	{
		printf( "Error in pcap_findpsttAllDevice: %s", szErrBuf );
		return false;
	}
	
	if( psttAllDevice == NULL )
	{
		printf( "No interfaces found! Make sure WinPcap is installed." );
		return false;
	}
	
	// Jump to the selected adapter
	for( psttDeviceEntry = psttAllDevice; psttDeviceEntry; psttDeviceEntry = psttDeviceEntry->next )
	{
		if( strstr( psttDeviceEntry->description, pszDeviceDesscription ) )
		{
			printf( "Device Description is [%s]\n", psttDeviceEntry->description );
			snprintf( pszDeviceName, iDeviceNameSize, "%s", psttDeviceEntry->name );
			bFound = true;
			break;
		}
	}

	pcap_freealldevs( psttAllDevice );

	if( bFound == false )
	{
		printf( "device name is not correct\n" );
		return false;
	}

	printf( "Device Name is [%s]\n", pszDeviceName );

	return true;
}

/**
 * @ingroup SipCallDump
 * @brief 디바이스 이름을 출력한다.
 */
void PrintDeviceName()
{
	pcap_if_t * psttAllDevice = NULL;
	pcap_if_t * psttDeviceEntry;
	char		szErrBuf[PCAP_ERRBUF_SIZE];

	// Retrieve the device list
	if( pcap_findalldevs( &psttAllDevice, szErrBuf ) == -1 )
	{
		printf( "Error in pcap_findpsttAllDevice: %s", szErrBuf );
		return;
	}
	
	if( psttAllDevice == NULL )
	{
		printf( "No interfaces found! Make sure WinPcap is installed." );
		return;
	}
	
	// Jump to the selected adapter
	for( psttDeviceEntry = psttAllDevice; psttDeviceEntry; psttDeviceEntry = psttDeviceEntry->next )
	{
		printf( "[%s] [%s]\n", psttDeviceEntry->name, psttDeviceEntry->description );
	}

	pcap_freealldevs( psttAllDevice );
}
