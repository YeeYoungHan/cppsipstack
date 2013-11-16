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
 * @ingroup KSipServer
 * @brief SIP Call Pick up 통화 요청 수신 이벤트 핸들러
 * @param	pszCallId	SIP Call-ID
 * @param pszFrom		SIP From 사용자 아이디
 * @param pszTo			SIP To 사용자 아이디
 * @param pclsRtp		RTP 정보 저장 객체
 */
void CSipServer::PickUp( const char * pszCallId, const char * pszFrom, const char * pszTo, CSipCallRtp * pclsRtp )
{
	CXmlUser	xmlFrom;
	bool bCallPickup = false;

	CLog::Print( LOG_DEBUG, "EventIncomingCall CallPickup" );

	if( SelectUser( pszFrom, xmlFrom ) )
	{
		USER_ID_LIST	clsUserIdList;

		if( gclsUserMap.SelectGroup( xmlFrom.m_strGroupId.c_str(), clsUserIdList ) )
		{
			USER_ID_LIST::iterator	itUIL;
			std::string strOldCallId;

			for( itUIL = clsUserIdList.begin(); itUIL != clsUserIdList.end(); ++itUIL )
			{
				if( gclsCallMap.SelectToRing( itUIL->c_str(), strOldCallId ) )
				{
					

					break;
				}
			}
		}
	}
	
	if( bCallPickup == false )
	{
		CLog::Print( LOG_DEBUG, "EventIncomingCall CallPickup from(%s) is not found", pszFrom );
		StopCall( pszCallId, SIP_NOT_FOUND );
	}
}
