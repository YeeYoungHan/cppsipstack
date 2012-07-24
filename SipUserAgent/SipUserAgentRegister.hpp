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

bool CSipUserAgent::RecvRegisterResponse( int iThreadId, CSipMessage * pclsMessage )
{
	SIP_SERVER_INFO_LIST::iterator itSL;
	const char * pszUserId = pclsMessage->m_clsFrom.m_clsUri.m_strUser.c_str();

	for( itSL = m_clsRegisterList.begin(); itSL != m_clsRegisterList.end(); ++itSL )
	{
		if( !strcmp( itSL->m_strUserId.c_str(), pszUserId ) )
		{
			int iStatusCode = pclsMessage->m_iStatusCode;

			if( iStatusCode == SIP_OK )
			{
				if( itSL->m_iLoginTimeout == 0 )
				{
					itSL->ClearLogin();
				}
				else
				{
					itSL->m_bLogin = true;
					time( &itSL->m_iLoginTime );
				}

				if( m_pclsCallBack ) m_pclsCallBack->EventRegister( *itSL, iStatusCode );
			}
			else if( iStatusCode == SIP_UNAUTHORIZED || iStatusCode == SIP_PROXY_AUTHENTICATION_REQUIRED )
			{
				CSipMessage * pclsRequest = itSL->GetRegisterMessage( pclsMessage );
				gclsSipStack.SendSipMessage( pclsRequest );
			}
			else
			{
				itSL->ClearLogin();
				if( m_pclsCallBack ) m_pclsCallBack->EventRegister( *itSL, iStatusCode );
			}

			return true;
		}
	}

	return false;
}
