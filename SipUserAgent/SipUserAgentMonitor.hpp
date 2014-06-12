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

/**
 * @ingroup SipUserAgent
 * @brief Dialog 자료구조 모니터링을 위한 문자열을 저장한다.
 * @param strBuf 자료구조 모니터링을 위한 문자열 변수
 */
void CSipUserAgent::GetDialogString( CMonitorString & strBuf )
{
	SIP_DIALOG_MAP::iterator		itMap;

	strBuf.Clear();

	m_clsDialogMutex.acquire();
	for( itMap = m_clsDialogMap.begin(); itMap != m_clsDialogMap.end(); ++itMap )
	{
		strBuf.AddCol( itMap->first );
		strBuf.AddCol( itMap->second.m_strFromId );
		strBuf.AddCol( itMap->second.m_strToId );
		strBuf.AddCol( itMap->second.m_strContactIp, itMap->second.m_iContactPort );

		if( itMap->second.m_iLocalRtpPort > 0 )
		{
			strBuf.AddCol( itMap->second.m_strLocalRtpIp, itMap->second.m_iLocalRtpPort );
		}
		else
		{
			strBuf.AddCol( "" );
		}

		if( itMap->second.m_iRemoteRtpPort > 0 )
		{
			strBuf.AddCol( itMap->second.m_strRemoteRtpIp, itMap->second.m_iRemoteRtpPort );
		}
		else
		{
			strBuf.AddCol( "" );
		}

		strBuf.AddCol( (time_t)itMap->second.m_sttInviteTime.tv_sec );

		if( itMap->second.m_sttStartTime.tv_sec )
		{
			strBuf.AddCol( (time_t)itMap->second.m_sttStartTime.tv_sec );
		}
		else
		{
			strBuf.AddCol( "" );
		}

		if( itMap->second.m_sttEndTime.tv_sec )
		{
			strBuf.AddRow( (time_t)itMap->second.m_sttEndTime.tv_sec );
		}
		else
		{
			strBuf.AddRow( "" );
		}
	}
	m_clsDialogMutex.release();
}

/**
 * @ingroup SipUserAgent
 * @brief IP-PBX 로그인 자료구조 모니터링을 위한 문자열을 저장한다.
 * @param strBuf 자료구조 모니터링을 위한 문자열 변수
 */
void CSipUserAgent::GetServerString( CMonitorString & strBuf )
{
	SIP_SERVER_INFO_LIST::iterator	itList;
	std::string strCallId;

	strBuf.Clear();

	m_clsRegisterMutex.acquire();
	for( itList = m_clsRegisterList.begin(); itList != m_clsRegisterList.end(); ++itList )
	{
		itList->m_clsCallId.ToString( strCallId );

		strBuf.AddCol( itList->m_strIp, itList->m_iPort );
		strBuf.AddCol( itList->m_strDomain );
		strBuf.AddCol( itList->m_strUserId );
		strBuf.AddCol( itList->m_iLoginTimeout );
		strBuf.AddCol( SipGetTransport( itList->m_eTransport ) );
		strBuf.AddCol( itList->m_iNatTimeout );
		strBuf.AddCol( itList->m_bLogin ? "login" : "logout" );
		strBuf.AddCol( itList->m_iLoginTime );
		strBuf.AddCol( itList->m_iSendTime );
		strBuf.AddCol( itList->m_iNextSendTime );
		strBuf.AddCol( strCallId );
		strBuf.AddCol( itList->m_iSeqNo );
		strBuf.AddRow( itList->m_bDelete ? "delete" : "" );
	}
	m_clsRegisterMutex.release();
}
