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

bool CSipUserAgent::RecvInviteRequest( int iThreadId, CSipMessage * pclsMessage )
{

	return false;
}

bool CSipUserAgent::RecvInviteResponse( int iThreadId, CSipMessage * pclsMessage )
{
	if( SetInviteResponse( pclsMessage ) )
	{
		std::string	strCallId;

		pclsMessage->GetCallId( strCallId );

		if( pclsMessage->m_iStatusCode == SIP_TRYING || pclsMessage->m_iStatusCode == SIP_UNAUTHORIZED || pclsMessage->m_iStatusCode == SIP_PROXY_AUTHENTICATION_REQUIRED )
		{
			
		}
		else if( pclsMessage->m_iStatusCode > 100 && pclsMessage->m_iStatusCode < 200 )
		{
			CSipCallRtp clsRtp;

			if( GetSipCallRtp( pclsMessage, clsRtp ) )
			{
				m_pclsCallBack->EventCallRing( strCallId.c_str(), pclsMessage->m_iStatusCode, &clsRtp );
			}
			else
			{
				m_pclsCallBack->EventCallRing( strCallId.c_str(), pclsMessage->m_iStatusCode, NULL );
			}
		}
		else if( pclsMessage->m_iStatusCode >= 200 && pclsMessage->m_iStatusCode < 300 )
		{
			CSipCallRtp clsRtp;

			if( GetSipCallRtp( pclsMessage, clsRtp ) )
			{
				m_pclsCallBack->EventCallStart( strCallId.c_str(), &clsRtp );
			}
			else
			{
				m_pclsCallBack->EventCallStart( strCallId.c_str(), NULL );
			}
		}
		else
		{
			m_pclsCallBack->EventCallEnd( strCallId.c_str(), pclsMessage->m_iStatusCode );
		}
	}

	return true;
}
