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

static int giUdpPort = 10000;

/**
 * @ingroup TestWebRtc
 * @brief UDP 소켓을 생성한다.
 * @param hSocket [out] UDP 소켓
 * @param iPort		[out] UDP 포트 번호
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CreateUdpSocket( Socket & hSocket, int & iPort )
{
	int iStartPort = giUdpPort;

	while( giUdpPort < 65535 )
	{
		hSocket = UdpListen( giUdpPort, NULL );
		if( hSocket != INVALID_SOCKET ) 
		{
			iPort = giUdpPort;
			giUdpPort += 2;
			return true;
		}
		giUdpPort += 2;
	}

	for( giUdpPort = 10000; giUdpPort < iStartPort; giUdpPort += 2 )
	{
		hSocket = UdpListen( giUdpPort, NULL );
		if( hSocket != INVALID_SOCKET )
		{
			iPort = giUdpPort;
			giUdpPort += 2;
			return true;
		}
	}

	return false;
}

CRtpThreadArg::CRtpThreadArg() : m_hWebRtcUdp(INVALID_SOCKET), m_hPbxUdp(INVALID_SOCKET), m_iWebRtcUdpPort(0), m_iPbxUdpPort(0), m_bStop(false), m_bStartCall(false)
{
}

CRtpThreadArg::~CRtpThreadArg()
{
	Close();
}

/**
 * @ingroup TestWebRtc
 * @brief PBX RTP 및 WebRTC RTP 연동을 위한 UDP 소켓을 생성한다.
 * @returns 성공하면 true 를 리턴하고 그렇지 않으면 false 를 리턴한다.
 */
bool CRtpThreadArg::CreateSocket()
{
	if( CreateUdpSocket( m_hWebRtcUdp, m_iWebRtcUdpPort ) && CreateUdpSocket( m_hPbxUdp, m_iPbxUdpPort ) )
	{
		m_strIcePwd = "FNPRfT4qUaVOKa0ivkn64mMY";
		return true;
	}

	Close();

	return false;
}

/**
 * @ingroup TestWebRtc
 * @brief UDP 소켓을 close 한다.
 */
void CRtpThreadArg::Close()
{
	if( m_hWebRtcUdp != INVALID_SOCKET )
	{
		closesocket( m_hWebRtcUdp );
		m_hWebRtcUdp = INVALID_SOCKET;
	}

	if( m_hPbxUdp != INVALID_SOCKET )
	{
		closesocket( m_hPbxUdp );
		m_hPbxUdp = INVALID_SOCKET;
	}

	m_iWebRtcUdpPort = 0;
	m_iPbxUdpPort = 0;
}
