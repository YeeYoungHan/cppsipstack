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

#include "SdpMessage.h"

extern std::string gstrLocalIpPrefix;

bool GetLocalRtp( const char * pszSdp, CSipCallRtp & clsRtp )
{
	CSdpMessage clsSdp;
	SDP_MEDIA_LIST::iterator itML;
	SDP_ATTRIBUTE_LIST::iterator itAL;
	char szIp[21];
	int iPort = 0;
	bool bFound = false;

	if( clsSdp.Parse( pszSdp, strlen(pszSdp) ) == -1 ) return false;

	// TestWebRtc 프로그램이 실행한 호스트와 로컬 IP 주소 대역이 동일한 RTP IP 주소와 포트 번호를 가져온다.
	for( itML = clsSdp.m_clsMediaList.begin(); itML != clsSdp.m_clsMediaList.end(); ++itML )
	{
		if( !strcmp( itML->m_strMedia.c_str(), "audio" ) )
		{
			for( itAL = itML->m_clsAttributeList.begin(); itAL != itML->m_clsAttributeList.end(); ++itAL )
			{
				if( !strcmp( itAL->m_strName.c_str(), "candidate" ) )
				{
					if( sscanf( itAL->m_strValue.c_str(), "%*s %*s %*s %*s %s %d", szIp, &iPort ) == 2 )
					{
						if( !strncmp( szIp, gstrLocalIpPrefix.c_str(), gstrLocalIpPrefix.length() ) )
						{
							bFound = true;
							break;
						}
					}
				}
			}
		}

		if( bFound ) break;
	}

	if( bFound == false ) return false;

	clsRtp.m_iCodec = 0;
	clsRtp.m_iPort = iPort;
	clsRtp.m_strIp = szIp;

	return true;
}

