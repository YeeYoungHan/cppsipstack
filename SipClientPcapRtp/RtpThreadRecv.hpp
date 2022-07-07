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

THREAD_API RtpThreadRecv( LPVOID lpParameter )
{
	pollfd sttPoll[1];
	char	szPacket[320], szIp[21];
	int		iPacketLen;
	unsigned short sPort;

	gclsRtpThread.m_bRecvThreadRun = true;

	TcpSetPollIn( sttPoll[0], gclsRtpThread.m_hSocket );

	while( gclsRtpThread.m_bStopEvent == false )
	{
		if( poll( sttPoll, 1, 200 ) <= 0 )
		{
			continue;
		}

		iPacketLen = sizeof(szPacket);
		if( UdpRecv( gclsRtpThread.m_hSocket, szPacket, &iPacketLen, szIp, sizeof(szIp), &sPort ) == false )
		{
			continue;
		}

		if( strcmp( gclsRtpThread.m_strDestIp.c_str(), szIp ) || gclsRtpThread.m_iDestPort != sPort )
		{
			gclsRtpThread.m_strDestIp = szIp;
			gclsRtpThread.m_iDestPort = sPort;
		}
	}

	gclsRtpThread.m_bRecvThreadRun = false;

	return 0;
}
