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
 * @brief SIP MESSAGE 요청 메시지 수신 이벤트 핸들러
 * @param iThreadId		SIP stack 의 UDP 쓰레드 아이디
 * @param pclsMessage 수신된 SIP 요청 메시지
 * @returns 정상적으로 처리하면 true 를 리턴하고 실패하면 false 를 리턴한다.
 */
bool CSipUserAgent::RecvMessageRequest( int iThreadId, CSipMessage * pclsMessage )
{
	std::string strCallId;

	if( m_pclsCallBack )
	{
		if( m_pclsCallBack->EventIncomingRequestAuth( pclsMessage ) == false )
		{
			return true;
		}

		if( m_pclsCallBack->EventMessage( pclsMessage->m_clsFrom.m_clsUri.m_strUser.c_str(), pclsMessage->m_clsTo.m_clsUri.m_strUser.c_str(), pclsMessage ) )
		{
			gclsSipStack.SendSipMessage( pclsMessage->CreateResponse( SIP_OK ) );
			return true;
		}
	}

	gclsSipStack.SendSipMessage( pclsMessage->CreateResponse( SIP_DECLINE ) );

	return true;
}
