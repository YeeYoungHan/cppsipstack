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

#include "CallMap.h"
#include "RoomMap.h"
#include "RtpThread.h"
#include "SdpMessage.h"
#include "SipStack.h"

CCallMap gclsCallMap;
extern CSipStack	gclsSipStack;

CCallMap::CCallMap() : m_iUdpStartPort(10000), m_iUdpStopPort(20000), m_iUdpPort(0)
{
}

CCallMap::~CCallMap()
{
}

bool CCallMap::Insert( CSipMessage * pclsMessage )
{
	std::string	strKey;
	CALL_MAP::iterator	itMap;
	bool	bRes = false;

	GetKey( pclsMessage, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap == m_clsMap.end() )
	{
		std::string strRoomId = pclsMessage->m_clsTo.m_clsUri.m_strUser;
		CCallInfo	clsCallInfo;

		// 회의방이 존재하지 않으면 생성한다.
		if( gclsRoomMap.Select( strRoomId.c_str() ) == false )
		{
			gclsRoomMap.Insert( strRoomId.c_str() );
		}

		// RTP relay 에 사용할 로컬 RTP 포트를 생성한다.
		if( CreatePort( clsCallInfo ) == false )
		{
			
		}
		else if( StartRtpThread( strRoomId.c_str() ) == false )
		{
			clsCallInfo.CloseSocket();
		}
		else
		{
			clsCallInfo.m_strRoomId = strRoomId;
			SetRemoteRtp( clsCallInfo, pclsMessage );

			m_clsMap.insert( CALL_MAP::value_type( strKey, clsCallInfo ) );
			gclsRoomMap.Insert( strRoomId.c_str(), strKey.c_str() );

			bRes = true;
		}
	}
	else
	{
		SetRemoteRtp( itMap->second, pclsMessage );
	}
	m_clsMutex.release();

	return bRes;
}

bool CCallMap::Select( CSipMessage * pclsMessage, CCallInfo & clsCallInfo )
{
	std::string	strKey;

	GetKey( pclsMessage, strKey );

	return Select( strKey.c_str(), clsCallInfo );
}

bool CCallMap::Select( const char * pszKey, CCallInfo & clsCallInfo )
{
	CALL_MAP::iterator	itMap;
	bool	bRes = false;

	m_clsMutex.acquire();
	itMap = m_clsMap.find( pszKey );
	if( itMap != m_clsMap.end() )
	{
		clsCallInfo = itMap->second;
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

bool CCallMap::Delete( CSipMessage * pclsMessage )
{
	std::string	strKey;
	CALL_MAP::iterator	itMap;
	bool	bRes = false;

	GetKey( pclsMessage, strKey );

	m_clsMutex.acquire();
	itMap = m_clsMap.find( strKey );
	if( itMap != m_clsMap.end() )
	{
		gclsRoomMap.Delete( itMap->second.m_strRoomId.c_str(), strKey.c_str() );

		m_clsMap.erase( itMap );
		bRes = true;
	}
	m_clsMutex.release();

	return bRes;
}

void CCallMap::GetKey( CSipMessage * pclsMessage, std::string & strKey )
{
	pclsMessage->m_clsCallId.ToString( strKey );
}

bool CCallMap::CreatePort( CCallInfo & clsCallInfo )
{
	bool bRes = false;

	if( m_iUdpPort == 0 ) m_iUdpPort = m_iUdpStartPort;

	if( CreatePort( clsCallInfo, m_iUdpPort, m_iUdpStopPort ) )
	{
		bRes = true;
	}
	else if( m_iUdpPort > m_iUdpStartPort )
	{
		if( CreatePort( clsCallInfo, m_iUdpStartPort, m_iUdpPort ) )
		{
			bRes = true;
		}
	}

	if( bRes )
	{
		m_iUdpPort += 4;
		if( m_iUdpPort >= m_iUdpStopPort ) m_iUdpPort = m_iUdpStartPort;
	}

	return bRes;
}

bool CCallMap::CreatePort( CCallInfo & clsCallInfo, int sStart, int sEnd )
{
	for( int sPort = sStart; sPort < sEnd; sPort += 4 )
	{
		clsCallInfo.m_hAudioRtp = UdpListen( sPort, NULL );
		if( clsCallInfo.m_hAudioRtp == INVALID_SOCKET ) continue;

		clsCallInfo.m_hAudioRtcp = UdpListen( sPort + 1, NULL );
		if( clsCallInfo.m_hAudioRtcp == INVALID_SOCKET )
		{
			clsCallInfo.CloseSocket();
			continue;
		}

		clsCallInfo.m_hVideoRtp = UdpListen( sPort + 2, NULL );
		if( clsCallInfo.m_hVideoRtp == INVALID_SOCKET )
		{
			clsCallInfo.CloseSocket();
			continue;
		}

		clsCallInfo.m_hVideoRtcp =  UdpListen( sPort + 3, NULL );
		if( clsCallInfo.m_hVideoRtcp == INVALID_SOCKET )
		{
			clsCallInfo.CloseSocket();
			continue;
		}

		clsCallInfo.m_clsLocalRtp.m_iAudioPort = sPort;
		clsCallInfo.m_clsLocalRtp.m_iVideoPort = sPort + 2;

		return true;
	}

	return false;
}

bool CCallMap::SetRemoteRtp( CCallInfo & clsCallInfo, CSipMessage * pclsMessage )
{
	if( pclsMessage->m_clsContentType.IsEqual( "application", "sdp" ) && pclsMessage->m_iContentLength > 0 )
	{
		CSdpMessage clsSdpMessage;

		if( clsSdpMessage.Parse( pclsMessage->m_strBody.c_str(), (int)pclsMessage->m_strBody.length() ) > 0 )
		{
			clsSdpMessage.m_clsConnection.m_strAddr = gclsSipStack.m_clsSetup.m_strLocalIp;

			SDP_MEDIA_LIST::iterator	itList;

			for( itList = clsSdpMessage.m_clsMediaList.begin(); itList != clsSdpMessage.m_clsMediaList.end(); ++itList )
			{
				if( !strcmp( itList->m_strMedia.c_str(), "audio" ) )
				{
					clsCallInfo.m_clsRemoteRtp.m_iAudioPort = itList->m_iPort;
					itList->m_iPort = clsCallInfo.m_clsLocalRtp.m_iAudioPort;
				}
				else if( !strcmp( itList->m_strMedia.c_str(), "video" ) )
				{
					clsCallInfo.m_clsRemoteRtp.m_iVideoPort = itList->m_iPort;
					itList->m_iPort = clsCallInfo.m_clsLocalRtp.m_iVideoPort;
				}
				else
				{
					itList->m_iPort = 0;
				}

				itList->m_clsConnection.m_strAddr = gclsSipStack.m_clsSetup.m_strLocalIp;
			}

			char	szSdp[4192];
			int		iSdpLen;

			memset( szSdp, 0, sizeof(szSdp) );

			iSdpLen = clsSdpMessage.ToString( szSdp, sizeof(szSdp) );
			if( iSdpLen > 0 )
			{
				pclsMessage->m_strBody = szSdp;
				pclsMessage->m_iContentLength = iSdpLen;
			}
		}
	}

	return true;
}
