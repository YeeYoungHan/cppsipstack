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
#include "SipUdp.h"
#include "SipUtility.h"
#include <string.h>
#include <stdio.h>
#include <string>
#include "TimeUtility.h"

bool TestMd5()
{
	unsigned char	szInput[10] = { 0x04, 0x20, 0xC4, 0x14, 0x61, 0xC8, 0x24, 0xA2, 0xCC, 0x34 };
	char	szOutput[20];

	SipMakePrintString( szInput, sizeof(szInput), szOutput, sizeof(szOutput) );

	if( strcmp( szOutput, "AbBcCdDeEfFgG" ) )
	{
		return false;
	}

	/*
	char	szTemp[255], szMd5[255];
	std::string strLocalIp;
	int iLocalUdpPort = 5060;
	struct timeval sttTime;

	GetLocalIp( strLocalIp );
	gettimeofday( &sttTime, NULL );

	int iLen = snprintf( szTemp, sizeof(szTemp), "%s:%d:%d:%d", strLocalIp.c_str(), iLocalUdpPort, (int)sttTime.tv_sec, (int)sttTime.tv_usec );

	SipMd5String21( szTemp, szMd5 );

	printf( "[%s]\n", szMd5 );
	*/

	return true;
}
