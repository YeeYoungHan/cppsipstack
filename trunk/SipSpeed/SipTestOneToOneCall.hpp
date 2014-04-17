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

void SipTestOneToOneCall( )
{
	CSipCallRtp clsRtp;
	CSipCallRoute clsRoute;
	std::string strCallId;

	clsRtp.m_strIp = gclsSipUserAgent.m_clsSipStack.m_clsSetup.m_strLocalIp;
	clsRtp.m_iPort = 4000;
	clsRtp.m_iCodec = 0;

	clsRoute.m_strDestIp = gclsSetup.m_strSipServerIp;
	clsRoute.m_iDestPort = gclsSetup.m_iSipServerPort;

	// ��ȭ ���� �׽�Ʈ
	for( int i = 0; i < gclsSetup.m_iCallTotalCount; ++i )
	{
		// ���� ��ȭ ������ ������ ������ ���ų� ū ��쿡�� ����Ѵ�.
		while( gclsSipUserAgent.GetCallCount() >= gclsSetup.m_iCallConcurrentCount )
		{
			Sleep(20);
			if( gbStopTestThread ) break;
		}

		if( gbStopTestThread ) break;

		if( gclsSipUserAgent.StartCall( gclsSetup.m_strCallerId.c_str(), gclsSetup.m_strCalleeId.c_str(), &clsRtp, &clsRoute, strCallId ) == false )
		{
			break;
		}
	}

	// ��� ��ȭ�� ����� ������ ����Ѵ�.
	while( gclsSipUserAgent.GetCallCount() > 0 )
	{
		Sleep(20);

		if( gbStopTestThread )
		{
			gclsSipUserAgent.StopCallAll();
			break;
		}
	}
}
